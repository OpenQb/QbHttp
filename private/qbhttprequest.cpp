#include "qbhttprequest_p.h"
#include "qbhttplogging_p.h"

namespace QbHttp {


QbHttpRequest::QbHttpRequest(stefanfrings::HttpRequest *request)
{
    qCDebug(QbHttp_Native())<<"QbHttpRequest created.";
    this->m_request = request;
}

QbHttpRequest::~QbHttpRequest()
{
    qCDebug(QbHttp_Native)<<"QbHttpRequest complete and destroying.";
}

int QbHttpRequest::getStatus()
{
    if(this->m_request){
        return static_cast<int>(this->m_request->getStatus());
    }
    return -1;
}

QByteArray QbHttpRequest::getMethod() const
{
    if(this->m_request){
        return this->m_request->getMethod();
    }
    else{
        return QByteArray();
    }
}

QByteArray QbHttpRequest::getPath() const
{
    if(this->m_request){
        return this->m_request->getPath();
    }
    else{
        return QByteArray();
    }
}

QByteArray QbHttpRequest::getRawPath() const
{
    if(this->m_request){
        return this->m_request->getRawPath();
    }
    else{
        return QByteArray();
    }
}

QByteArray QbHttpRequest::getVersion() const
{
    if(this->m_request){
        return this->m_request->getVersion();
    }
    else{
        return QByteArray();
    }
}

QByteArray QbHttpRequest::getHeader(const QByteArray &name)
{
    if(this->m_request){
        return this->m_request->getHeader(name);
    }
    else{
        return QByteArray();
    }
}

QByteArray QbHttpRequest::getParameter(const QByteArray &name)
{
    if(this->m_request){
        return this->m_request->getParameter(name);
    }
    else{
        return QByteArray();
    }
}

QByteArray QbHttpRequest::getBody()
{
    if(this->m_request){
        return this->m_request->getBody();
    }
    else{
        return QByteArray();
    }
}

QByteArray QbHttpRequest::urlDecode(const QByteArray &source)
{
    if(this->m_request){
        return this->m_request->urlDecode(source);
    }
    return QByteArray();
}

QByteArray QbHttpRequest::getCookie(const QByteArray &name) const
{
    if(this->m_request){
        return this->m_request->getCookie(name);
    }
    else{
        return QByteArray();
    }
}

QByteArray QbHttpRequest::getPeerAddress() const
{
    if(this->m_request){
        QHostAddress a = this->m_request->getPeerAddress();
        return a.toString().toLocal8Bit();
    }
    else{
        return QByteArray();
    }
}


void QbHttpRequest::done()
{
    this->deleteLater();
}



}
