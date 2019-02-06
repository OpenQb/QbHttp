#include "qbhttputil_p.h"
#include "qbhttplogging_p.h"

#include <QDebug>
#include <QFileInfo>

//#include <cstdio>
//#include <iostream>

QVariantMap dumpObject(const QObject *object)
{
    const QMetaObject *metaObject = object->metaObject();
    QVariantMap props;
    for (int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
        const char *key = metaObject->property(i).name();
        props.insert(key, object->property(key));
    }
    return props;
}


QVariant evaluate(const QString &input, const QVariantMap &context)
{
    const QStringList bits = input.split(".");
    QVariant value = context;
    foreach (const QString &bit, bits) {
        value = value.toMap().value(bit);
    }
    //qCDebug(QbHttp_Native(),"evaluate(%s): %s", qPrintable(input), qPrintable(value.toString()));
    return value;
}

QString substitute(const QString &input, const QVariantMap &context)
{
    QRegExp valRx("\\{\\{ +([a-z_\\.]+) +\\}\\}");

    QString output;
    int pos = 0;
    int lastPos = 0;
    while ((pos = valRx.indexIn(input, lastPos)) != -1) {
        output += input.mid(lastPos, pos - lastPos);
        lastPos = pos + valRx.matchedLength();
        output += evaluate(valRx.cap(1), context).toString();
    }
    output += input.mid(lastPos);
    return output;
}

QList<Node> tokenize(const QString &input)
{
    QList<Node> output;
    QRegExp tagRx("\\{% +([^%]+) +%\\}");
    int pos = 0;
    int lastPos = 0;
    while ((pos = tagRx.indexIn(input, lastPos)) != -1) {
        if (pos > lastPos)
            output << qMakePair(false, input.mid(lastPos, pos - lastPos));

        lastPos = pos + tagRx.matchedLength();
        output << qMakePair(true, tagRx.cap(1));
    }
    output << qMakePair(false, input.mid(lastPos));
    return output;
}

static int findBalancing(const QList<Node> nodes, const QString &closeTag, int pos, int *elsePos = 0)
{
    const QString openTag = nodes[pos].second.split(" ").first();
    int level = 0;
    if (elsePos)
        *elsePos = -1;
    for (pos = pos + 1; pos < nodes.size(); ++pos) {
        if (nodes[pos].first && nodes[pos].second.startsWith(openTag)) {
            level++;
        }
        else if (nodes[pos].first && nodes[pos].second == closeTag) {
            if (!level)
                return pos;
            level--;
        } else if (!level && nodes[pos].second == "else") {
            if (elsePos)
                *elsePos = pos;
        }
    }
    qCWarning(QbHttp_Native(),"Could not find %s tag", qPrintable(closeTag));
    return -1;
}

QString render(const QString &name, const QList<Node> &nodes, const QVariantMap &context)
{
    QRegExp forRx("for ([a-z_]+) in ([a-z_\\.]+)");
    QRegExp includeRx("include \"([^\"]+)\"");

    QString output;
    for (int i = 0; i < nodes.size(); ++i) {
        const Node &node = nodes[i];
        if (node.first) {
            //qDebug("Processing tag %s", qPrintable(node.second));
            QStringList tagArgs = node.second.split(" ");
            const QString tagName = tagArgs.takeFirst();
            if (node.second == "comment") {
                const int endPos = findBalancing(nodes, "endcomment", i++);
                if (endPos < 0)
                    return output;
                i = endPos;
            } else if (forRx.exactMatch(node.second)) {
                const int endPos = findBalancing(nodes, "endfor", i++);
                if (endPos < 0)
                    return output;

                const QVariantList list = evaluate(forRx.cap(2), context).toList();
                QVariantMap forLoop;
                int counter0 = 0;
                foreach (const QVariant &val, list) {
                    forLoop.insert("counter", counter0 + 1);
                    forLoop.insert("counter0", counter0);
                    if (!counter0)
                        forLoop.insert("first", true);

                    QVariantMap subContext = context;
                    subContext.insert(forRx.cap(1), val);
                    subContext.insert("forloop", forLoop);
                    output += render(name,nodes.mid(i, endPos - i), subContext);
                    counter0++;
                }
                i = endPos;
            } else if (tagName == "if") {
                int elsePos = -1;
                const int endPos = findBalancing(nodes, "endif", i++, &elsePos);
                if (endPos < 0)
                    return output;

                bool isTrue = false;
                QRegExp ifRx("if ([a-z_\\.]+) (!=|==) \"([^\"]*)\"");
                if (ifRx.exactMatch(node.second)) {
                    const QVariant value = evaluate(ifRx.cap(1), context);
                    const QString op = ifRx.cap(2);
                    const QString opValue = ifRx.cap(3);
                    if ((op == "==" && value.toString() == opValue) ||
                            (op == "!=" && value.toString() != opValue)) {
                        isTrue = true;
                    }
                } else if (tagArgs.size() == 1) {
                    const QVariant value = evaluate(tagArgs[0], context);
                    if (value.toList().size() || value.toMap().size() || value.toString().size())
                        isTrue = true;
                }
                if (isTrue) {
                    output += render(name,nodes.mid(i, (elsePos > 0 ? elsePos : endPos) - i), context);
                } else if (elsePos > 0) {
                    output += render(name,nodes.mid(elsePos, endPos - elsePos), context);
                }
                i = endPos;
            } else if (includeRx.exactMatch(node.second)) {
                QString nname = QFileInfo(name).canonicalPath();
                output += renderTemplate(nname + includeRx.cap(1), context);
            }
        } else {
            output += substitute(node.second, context);
        }
    }
    return output;
}

QString renderTemplate(const QString &name, const QVariantMap &context)
{
    //std::cout<<"insideRenderTemplate: "<<name.toStdString();

    QFile templ(name);
    if (templ.open(QIODevice::ReadOnly)) {
        const QString data = QString::fromUtf8(templ.readAll());
        //std::cout<<"insideRenderTemplate: "<<data.toStdString();

        return render(name,tokenize(data), context);
    }
    return QString();
}




QVariantList listDirectory(const QString &root, const QString &path)
{
    QString npath = QDir::toNativeSeparators(root);
    QVariantList data;

    QDir d(root+path);
    QFileInfoList fList =  d.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot,QDir::Time|QDir::DirsFirst);
    foreach (const QFileInfo &fi, fList) {
        QVariantMap map;
        map.insert("name",fi.fileName());
        if(fi.isDir()){
            map.insert("type","folder");
        }
        else if(fi.isFile()){
            map.insert("type","file");
        }
        else{
            map.insert("type","unknown");
        }

        QString fPath = fi.filePath();
        fPath = fPath.replace(npath,"");
        map.insert("path",fPath);
        data.append(map);
    }
    return data;
}

QVariantList listDirectory2(const QString &root, const QString &path, const QString &startString)
{
    QString npath = QDir::toNativeSeparators(root);
    QVariantList data;
    QString nstartString = startString;
    nstartString = nstartString.replace("//","/");

    QDir d(root+path);
    QFileInfoList fList =  d.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot,QDir::Time|QDir::DirsFirst);
    foreach (const QFileInfo &fi, fList) {
        QVariantMap map;
        map.insert("name",fi.fileName());
        if(fi.isDir()){
            map.insert("type","folder");
        }
        else if(fi.isFile()){
            map.insert("type","file");
        }
        else{
            map.insert("type","unknown");
        }

        QString fPath = QDir::toNativeSeparators(fi.filePath());
        fPath = fPath.replace(npath,"");
        fPath = fPath.replace("//","/");
        //qDebug() << fPath;
        if(!fPath.startsWith("/")&&!nstartString.endsWith("/")){
            fPath = QLatin1String("/") + fPath;
        }

        QString np = nstartString+fPath;
        np = np.replace("\\","/");
        np = np.replace("\\","/");
        np = np.replace("//","/");
        np = np.replace("//","/");
        map.insert("path",np);
        data.append(map);
    }
    return data;
}
