#include "qbhttpserver_p.h"
#include "qbhttplogging_p.h"
#include "qbhttprequesthandler_p.h"
#include "qbhttpsmartrequesthandler_p.h"

#include <QDateTime>
#include <QHostAddress>
#include <QStandardPaths>
#include <QNetworkInterface>

namespace QbHttp {

QbHttpServer::QbHttpServer(QObject *parent) : QObject(parent)
{
    qCDebug(QbHttp_Native())<< "QbHttpServer created.";
    this->m_listener = Q_NULLPTR;
    this->m_settings = Q_NULLPTR;
    this->m_handler = Q_NULLPTR;

    this->m_host = QLatin1String("127.0.0.1");
    this->m_port = 8080;
    this->m_minThreads = 1;
    this->m_maxThreads = 5;
    this->m_cleanupInterval = 60000;
    this->m_readTimeout = 60000;
    this->m_maxRequestSize = 16000;
    this->m_maxMultiPartSize = 10000000;

    this->m_docRoot = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    this->m_encoding = QLatin1String("UTF-8");
    this->m_cacheMaxAge = 60000;
    this->m_cacheTime = 60000;
    this->m_cacheSize = 1000000;

    this->m_serverMode = QbHttpServerMode::DYNAMIC;
}

QbHttpServer::~QbHttpServer()
{
    qCDebug(QbHttp_Native())<< "QbHttpServer deleted.";
}

void QbHttpServer::classBegin()
{
    qCDebug(QbHttp_Native())<< "QbHttpServer classBegin().";
}

void QbHttpServer::componentComplete()
{
    qCDebug(QbHttp_Native())<< "QbHttpServer componentComplete().";
    this->m_engine = qmlEngine(this);
}

void QbHttpServer::listen()
{
    if(this->m_listener!=Q_NULLPTR){
        delete this->m_listener;
        this->m_listener = Q_NULLPTR;
    }

    if(this->m_settings!=Q_NULLPTR){
        delete this->m_settings;
        this->m_settings = Q_NULLPTR;
    }

    if(this->m_handler!=Q_NULLPTR){
        delete this->m_handler;
        this->m_handler = Q_NULLPTR;
    }


    this->m_settings = new QbHttpSettings(this);
    if(this->m_serverMode == QbHttpServerMode::DYNAMIC){
        this->m_handler = new QbHttpRequestHandler(this->m_serverCode,this->m_serverJsPath,this->m_callBack,this);
    }
    else{
        this->m_handler = new QbHttpSmartRequestHandler(this);
    }
    qCDebug(QbHttp_Native())<< "Creating HttpListener at PORT:"<< this->m_settings->value("port").toInt();
    this->m_listener = new stefanfrings::HttpListener(this->m_settings,this->m_handler,this);
    qCDebug(QbHttp_Native())<< "HttpListener created.";



    this->m_settings->setValue("host",this->m_host);
    this->m_settings->setValue("port",this->m_port);
    this->m_settings->setValue("minThreads",this->m_minThreads);
    this->m_settings->setValue("maxThreads",this->m_maxThreads);
    this->m_settings->setValue("cleanupInterval",this->m_cleanupInterval);
    this->m_settings->setValue("readTimeout",this->m_readTimeout);
    this->m_settings->setValue("maxRequestSize",this->m_maxRequestSize);
    this->m_settings->setValue("maxMultiPartSize",this->m_maxMultiPartSize);

    this->m_settings->setValue("path",this->m_docRoot);
    this->m_settings->setValue("encoding",this->m_encoding);
    this->m_settings->setValue("maxAge",this->m_cacheMaxAge);
    this->m_settings->setValue("cacheTime", this->m_cacheTime);
    this->m_settings->setValue("cacheSize", this->m_cacheSize);
    this->m_settings->setValue("maxCachedFileSize", this->m_maxCachedFileSize);

    //qDebug() << this->m_settings->value("path");

    this->m_listener->listen();

    if(this->m_host!=QLatin1String("127.0.0.1")){
        QList<QHostAddress> list = QNetworkInterface::allAddresses();
        bool found = false;
        for(int nIter=0; nIter<list.count(); nIter++)
        {

            if(!list[nIter].isLoopback())
                if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol ){
                    QString ipStr = list[nIter].toString()+QLatin1String(":")+QString::number(this->m_port);
                    emit listenIpAddress(ipStr);
                    emitLogMessage(QString("Listening on IP: ")+ipStr);
                    qCDebug(QbHttp_Native())<< "Listening on IP: "<<ipStr;
                    found = true;

                }
        }
        if(!found){
            QString ipStr = QLatin1String("127.0.0.1:")+QString::number(this->m_port);
            emit listenIpAddress(ipStr);
            emitLogMessage(QString("Listening on IP: ")+ipStr);
            qCDebug(QbHttp_Native())<< "Listening on IP: "<<ipStr;
        }
    }
    else{
        QString ipStr = QLatin1String("127.0.0.1:")+QString::number(this->m_port);
        emit listenIpAddress(ipStr);
        emitLogMessage(QString("Listening on IP: ")+ipStr);
        qCDebug(QbHttp_Native())<< "Listening on IP: "<<ipStr;
    }

}

void QbHttpServer::stop()
{
    if(this->m_listener!=Q_NULLPTR){
        this->m_listener->close();
        emitLogMessage("Server stopped.");
    }
}

void QbHttpServer::setCallBack(QString callBack)
{
    this->m_callBack = callBack;
}

void QbHttpServer::addStaticRootFolder(const QString &key, const QString &value)
{
    if(this->m_staticRootFolderMap.contains(key)) return;

    this->m_staticRootFolderMap.insert(key,value);
}

void QbHttpServer::clearStaticRootFolder()
{
    this->m_staticRootFolderMap.clear();
}

void QbHttpServer::emitLogMessage(const QString &message)
{
    QString msg = QDateTime::currentDateTime().toString("dd MMM yyyy - hh:mm:ss A");
    msg.append(" - ");
    msg.append(message);
    emit logMessage(msg);
}

QVariantMap QbHttpServer::getStaticRootFolderMap()
{
    return this->m_staticRootFolderMap;
}

void QbHttpServer::registerMe()
{
    qmlRegisterType<QbHttpServer>("Qb.Http",1,0,"QbHttpServer");
    //qmlRegisterUncreatableType<QbHttpRequest>("Qb.Http", 1, 0, "QbHttpRequest", "Do not create QbHttpRequest directly.");
    //qmlRegisterUncreatableType<QbHttpResponse>("Qb.Http", 1, 0, "QbHttpResponse", "Do not create QbHttpResponse directly.");

    qCDebug(QbHttp_Native())<< "QbHttpServer QML types registered.";
}

}
