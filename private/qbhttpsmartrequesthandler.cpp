#include "qbhttpsmartrequesthandler_p.h"
#include "qbhttplogging_p.h"

#include "qbhttputil_p.h"


#include <QDir>
#include <QPair>
#include <QMimeType>
#include <QFileInfo>
#include <QMetaObject>
#include <QMimeDatabase>
#include <QJsonDocument>
#include <QStandardPaths>

//#include <cstdio>
//#include <iostream>


namespace QbHttp {

QbHttpSmartRequestHandler::QbHttpSmartRequestHandler(QbHttpServer *server):
    HttpRequestHandler(server)
{
    qCDebug(QbHttp_Native())<< "QbHttpSmartRequestHandler created.";

    this->m_server = server;
    this->m_encoding = this->m_server->encoding();
    this->m_staticRootFolderMap = this->m_server->getStaticRootFolderMap();
    this->m_serverMode = this->m_server->serverMode();
    this->m_docRoot = this->m_server->docRoot();
    this->m_templateFilePath = this->m_server->templateFilePath();
    if(this->m_templateFilePath.startsWith("file:///")){
        this->m_templateFilePath = this->m_templateFilePath.replace(0,8,"");
    }
    else if(this->m_templateFilePath.startsWith("file://")){
        this->m_templateFilePath = this->m_templateFilePath.replace(0,7,"");
    }
    if(this->m_templateFilePath.startsWith("qrc:")){
        this->m_templateFilePath = this->m_templateFilePath.replace(0,3,"");
    }

    if(this->m_docRoot.isEmpty()){
        this->m_docRoot = QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    }

    //this->connect(this,&QbHttpSmartRequestHandler::logMessage,this->m_server,&QbHttpServer::emitLogMessage);
}

QbHttpSmartRequestHandler::~QbHttpSmartRequestHandler()
{
    qCDebug(QbHttp_Native())<< "QbHttpSmartRequestHandler destroyed.";
}

void QbHttpSmartRequestHandler::service(HttpRequest &request, HttpResponse &response)
{
    QString path = request.getPath();
    QByteArray parameter = request.getParameter("variant");
    //qDebug() << parameter;
    //qDebug() << request.getMethod();
    response.setHeader(QByteArray("Server"),QByteArray("QbServer/1.0.0"));

    qCDebug(QbHttp_Native())<<"CLIENT IP:"<<request.getPeerAddress().toString();
    qCDebug(QbHttp_Native())<<"REQUESTED PATH: "<<path;
    qCDebug(QbHttp_Native())<<"PARAMETERS: "<<request.getParameterMap();

    if(path.contains("/..")){
        //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"403","Forbidden"));
        response.setHeader("Content-Type", qPrintable("text/html; charset="+this->m_encoding));
        response.setStatus(403,"Forbidden");
        response.write("403 Forbidden",true);
    }
    else{
        if(this->m_serverMode == QbHttpServer::QbHttpServerMode::STATIC){
            if(QFileInfo(this->m_docRoot+path).isDir()){
                //NOTE: Here we'll list the directory contents
                //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"200","OK"));
                response.setStatus(200,"OK");
                if(parameter == QByteArray("json")){
                    QJsonDocument jdoc = QJsonDocument::fromVariant(listDirectory(this->m_docRoot,path));
                    response.setHeader("Content-Type",qPrintable("application/json; charset="+this->m_encoding));
                    response.write(jdoc.toJson(QJsonDocument::Compact),true);
                }
                else{
                    QVariantMap context;
                    context.insert("path",path);
                    context.insert("files",listDirectory(this->m_docRoot,path));
                    QString data = renderTemplate(this->m_templateFilePath,context);
                    response.setHeader("Content-Type", qPrintable("text/html; charset="+this->m_encoding));
                    response.write(data.toLocal8Bit(),true);
                }
            }
            else{
                QFile file(this->m_docRoot+path);
                if(file.open(QIODevice::ReadOnly)){
                    //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"200","OK"));
                    response.setStatus(200,"OK");
                    setContentType(path,response);
                    while (!file.atEnd() && !file.error())
                    {
                        response.write(file.read(65536));
                    }
                    file.close();
                }
                else{
                    if(file.exists()){
                        //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"403","Forbidden"));
                        response.setHeader("Content-Type", qPrintable("text/html; charset="+this->m_encoding));
                        response.setStatus(403,"forbidden");
                        response.write("403 forbidden",true);
                    }
                    else{
                        //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"404","Not Found"));
                        response.setHeader("Content-Type", qPrintable("text/html; charset="+this->m_encoding));
                        response.setStatus(404,"not found");
                        response.write("404 not found",true);
                    }
                }
            }
        }
        else{
            //MULTI-FOLDER-STATIC
            if(this->m_staticRootFolderMap.isEmpty()){
                response.setHeader("Content-Type", qPrintable("text/html; charset="+this->m_encoding));
                response.setStatus(200,"OK");
                response.write("Nothing to serve.");
                //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"200","OK"));
            }
            else{
                // We will show Mapped Keys
                if(path == QLatin1String("/")){
                    //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"200","OK"));
                    response.setStatus(200,"OK");
                    if(parameter == QByteArray("json")){
                        //JSON
                        QVariantList folderList;
                        foreach(QString s, this->m_staticRootFolderMap.keys()){
                            folderList << s;
                        }
                        QJsonDocument jdoc = QJsonDocument::fromVariant(folderList);
                        response.setHeader("Content-Type",qPrintable("application/json; charset="+this->m_encoding));
                        response.write(jdoc.toJson(QJsonDocument::Compact),true);
                    }
                    else{
                        //HTML
                        QVariantMap context;
                        context.insert("path",path);
                        QVariantList lst;
                        foreach (const QString &key, this->m_staticRootFolderMap.keys()) {
                            QVariantMap map;
                            map.insert("name",key);
                            map.insert("path",QLatin1String("/")+key);
                            map.insert("type",QLatin1String("folder"));
                            lst.append(map);
                        }
                        context.insert("files",lst);
                        QString data = renderTemplate(this->m_templateFilePath,context);
                        response.setHeader("Content-Type", qPrintable("text/html; charset="+this->m_encoding));
                        response.write(data.toLocal8Bit(),true);
                    }
                }
                else{
                    bool matchedFolder = false;
                    QString matchedDocRootPath;
                    QString matchedDocRootKey;
                    foreach (const QString &key, this->m_staticRootFolderMap.keys()) {
                        if(path.startsWith((QLatin1String("/")+key))){
                            matchedFolder = true;
                            matchedDocRootKey = QLatin1String("/")+key;
                            matchedDocRootPath = this->m_staticRootFolderMap.value(key).toString();
                        }
                    }

                    if(matchedFolder){
                        if(path == matchedDocRootKey){
                            //serve root as list since exact match
                            //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"200","OK"));
                            response.setStatus(200,"OK");

                            //qDebug() << listDirectory(matchedDocRootPath,path);
                            if(parameter == QByteArray("json")){
                                QJsonDocument jdoc = QJsonDocument::fromVariant(listDirectory2(matchedDocRootPath,"/",matchedDocRootKey));
                                response.setHeader("Content-Type",qPrintable("application/json; charset="+this->m_encoding));
                                response.write(jdoc.toJson(QJsonDocument::Compact),true);
                            }
                            else{
                                QVariantMap context;
                                context.insert("path",path);
                                context.insert("files",listDirectory2(matchedDocRootPath,"/",matchedDocRootKey));
                                QString data = renderTemplate(this->m_templateFilePath,context);
                                response.setHeader("Content-Type", qPrintable("text/html; charset="+this->m_encoding));
                                response.write(data.toLocal8Bit(),true);
                            }

                        }
                        else{
                            //server static data or list the directory
                            QString npath = path;
                            //drop the matched key from the link
                            npath = npath.replace(0,matchedDocRootKey.length(),"");

                            if(QFileInfo(matchedDocRootPath+npath).isDir()){
                                //NOTE: Here we'll list the directory contents
                                //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"200","OK"));
                                response.setStatus(200,"OK");
                                if(parameter == QByteArray("json")){
                                    QJsonDocument jdoc = QJsonDocument::fromVariant(listDirectory2(matchedDocRootPath,npath,matchedDocRootKey));
                                    response.setHeader("Content-Type",qPrintable("application/json; charset="+this->m_encoding));
                                    response.write(jdoc.toJson(QJsonDocument::Compact),true);
                                }
                                else{
                                    QVariantMap context;
                                    context.insert("path",path);
                                    context.insert("files",listDirectory2(matchedDocRootPath,npath,matchedDocRootKey));
                                    QString data = renderTemplate(this->m_templateFilePath,context);
                                    response.setHeader("Content-Type", qPrintable("text/html; charset="+this->m_encoding));
                                    response.write(data.toLocal8Bit(),true);
                                }
                            }
                            else{
                                QFile file(matchedDocRootPath+npath);
                                if(file.open(QIODevice::ReadOnly)){
                                    //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"200","OK"));
                                    response.setStatus(200,"OK");
                                    setContentType(path,response);
                                    while (!file.atEnd() && !file.error())
                                    {
                                        response.write(file.read(65536));
                                    }
                                    file.close();
                                }
                                else{
                                    if(file.exists()){
                                        //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"403","Forbidden"));
                                        response.setHeader("Content-Type", qPrintable("text/html; charset="+this->m_encoding));
                                        response.setStatus(403,"forbidden");
                                        response.write("403 forbidden",true);
                                    }
                                    else{
                                        //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"404","Not Found"));
                                        response.setHeader("Content-Type", qPrintable("text/html; charset="+this->m_encoding));
                                        response.setStatus(404,"not found");
                                        response.write("404 not found",true);
                                    }
                                }
                            }

                        }
                    }
                    else{
                        //emit logMessage(QString("%1 - STATUS: %2 - MSG: %3").arg(path,"404","Not Found"));
                        response.setStatus(404,"Not Found");
                        response.setHeader("Content-Type", qPrintable("text/html; charset="+this->m_encoding));
                        response.write("Not Found",true);
                    }
                }
            }
        }
    }
    qCDebug(QbHttp_Native())<<"REQUEST COMPLETE.";
}

void QbHttpSmartRequestHandler::setContentType(QString _fileName, HttpResponse &response) const
{
    QString fileName = _fileName.toLower();
    response.setHeader("Content-Type", QMimeDatabase().mimeTypeForFile(fileName).name().toLatin1());
}

}
