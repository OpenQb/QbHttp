#include "qbhttprequesthandler_p.h"

#include "qbhttprequest_p.h"
#include "qbhttpresponse_p.h"
#include "qbhttplogging_p.h"

#include <QQmlEngine>
#include <QJSEngine>

namespace QbHttp {
QbHttpRequestHandler::QbHttpRequestHandler(QString serverCode, QString serverJsPath, QString callBack, QObject *parent):
    HttpRequestHandler(parent)
{
    qCDebug(QbHttp_Native())<< "QbHttpRequestHandler created.";
    this->m_serverJsPath = serverJsPath;
    this->m_callBack = callBack;
    this->m_serverCode = serverCode;
    if(this->m_serverJsPath.startsWith("file:///")){
        this->m_serverJsPath = this->m_serverJsPath.replace("file:///","");
    }
    else if(this->m_serverJsPath.startsWith("file://")){
        this->m_serverJsPath = this->m_serverJsPath.replace("file://","");
    }
    if(this->m_serverJsPath.startsWith("qrc:")){
        this->m_serverJsPath = this->m_serverJsPath.replace("qrc:",":");
    }
    this->m_engine.installExtensions(QJSEngine::AllExtensions);
}

QbHttpRequestHandler::~QbHttpRequestHandler()
{
    qCDebug(QbHttp_Native())<< "QbHttpRequestHandler destroyed.";
}

void QbHttpRequestHandler::service(HttpRequest &request,HttpResponse &response)
{    
    qCDebug(QbHttp_Native()) << "PATH: "<<request.getPath();
    if(this->m_serverJsPath.isEmpty() && this->m_callBack.isEmpty() && this->m_serverCode.isEmpty()){
       qCDebug(QbHttp_Native()) << "Please provide serverJsPath or callBack or serverCode function as string";
       return;
    }

    //qDebug() << "Engine Thread:"<<this->m_engine.thread();
    //qDebug() << "Request Handler Thread:"<<this->thread();
    qCDebug(QbHttp_Native())<< "serverJsPath:"<<this->m_serverJsPath;

    QString code;
    if(!this->m_serverCode.isEmpty()){
        code = this->m_serverCode;
    }
    else if(!this->m_serverJsPath.isEmpty()){
        code = this->data(this->m_serverJsPath);
    }
    else if(!this->m_callBack.isEmpty()){
        code = this->m_callBack;
    }



    QJSValue callBack = this->m_engine.evaluate(code);

    if(callBack.isCallable()){
        QbHttpRequest *qreq = new QbHttpRequest(&request);
        QbHttpResponse *qrep = new QbHttpResponse(&response);
        QJSValueList args;
        QJSValue a1 = this->m_engine.newQObject(qreq);
        QJSValue a2 = this->m_engine.newQObject(qrep);
        args<<a1;
        args<<a2;
        callBack.call(args);
        qCDebug(QbHttp_Native())<< "Headers:"<<response.getHeaders();

        delete qreq;
        delete qrep;
    }
    else{
        response.setStatus(500,QByteArray("Internal Server Error"));
        response.write("Failed to run server code",true);
    }

}

QByteArray QbHttpRequestHandler::data(const QString &path)
{
    QByteArray data;
    QFile f(path);
    if(f.open(QIODevice::ReadOnly|QIODevice::Text)){
        data = f.readAll();
    }
    f.close();

    return data;
}
}
