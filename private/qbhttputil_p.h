#ifndef QBHTTPUTIL_P_H
#define QBHTTPUTIL_P_H

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QVariantMap>
#include <QMetaObject>
#include <QVariantList>
#include <QMetaProperty>

typedef QPair<bool, QString> Node;

QVariantMap dumpObject(const QObject *object);
QVariant evaluate(const QString &input, const QVariantMap &context);
QString substitute(const QString &input, const QVariantMap &context);
QList<Node> tokenize(const QString &input);
QString renderTemplate(const QString &name, const QVariantMap &context);
QString render(const QString &name, const QList<Node> &nodes, const QVariantMap &context);

QVariantList listDirectory(const QString &root,const QString &path);

QVariantList listDirectory2(const QString &root, const QString &path, const QString &startString);

#endif // QBHTTPUTIL_P_H
