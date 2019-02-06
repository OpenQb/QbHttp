#include "qbhttpresponse_p.h"
#include "qbhttplogging_p.h"
#include <QThread>

namespace QbHttp {

QbHttpResponse::QbHttpResponse(stefanfrings::HttpResponse *response)
{
    qCDebug(QbHttp_Native())<<"QbHttpResponse created.";
    this->m_response = response;
}

QbHttpResponse::~QbHttpResponse()
{
    qCDebug(QbHttp_Native)<<"QbHttpResponse complete and destroying.";
}

void QbHttpResponse::setHeader(QByteArray name, QByteArray value)
{
    if(this->m_response){
        this->m_response->setHeader(name,value);
    }
}

QVariantMap QbHttpResponse::getHeaders()
{
    this->m_headerMap.clear();
    if(this->m_response){
        QMapIterator<QByteArray, QByteArray> i(this->m_response->getHeaders());
        while (i.hasNext()) {
            this->m_headerMap.insert(i.key(),i.value());
        }
    }

    return this->m_headerMap;
}

QVariantMap QbHttpResponse::getCookies()
{
    this->m_cookieMap.clear();

    if(this->m_response){
        QMapIterator<QByteArray, stefanfrings::HttpCookie> i(this->m_response->getCookies());
        while (i.hasNext()) {
            i.next();
            stefanfrings::HttpCookie c = i.value();
            QByteArray key = i.key();
            QVariantMap nmap;
            nmap["name"] = c.getName();
            nmap["value"] = c.getValue();
            nmap["maxAge"] = c.getMaxAge();
            nmap["path"] = c.getPath();
            nmap["comment"] = c.getComment();
            nmap["domain"] = c.getDomain();
            nmap["secure"] = c.getSecure();
            nmap["httpOnly"] = c.getHttpOnly();
            this->m_cookieMap.insert(key,nmap);
        }
    }
    return this->m_cookieMap;
}

void QbHttpResponse::setStatus(int statusCode, QByteArray description)
{
    if(this->m_response){
        this->m_response->setStatus(statusCode,description);
    }
}

int QbHttpResponse::getStatusCode() const
{
    if(this->m_response){
        return this->m_response->getStatusCode();
    }
    else{
        return 0;
    }
}

void QbHttpResponse::write(QByteArray data, bool lastPart)
{
    if(this->m_response){
        this->m_response->write(data,lastPart);
    }
}

bool QbHttpResponse::hasSentLastPart() const
{
    if(this->m_response){
        return this->m_response->hasSentLastPart();
    }
    else{
        return true;
    }
}

void QbHttpResponse::setCookie(const QByteArray name, const QByteArray value, const int maxAge, const QByteArray path, const QByteArray comment, const QByteArray domain, const bool secure, const bool httpOnly)
{
    if(this->m_response){
        stefanfrings::HttpCookie c(name,value,maxAge,path,comment,domain,secure,httpOnly);
        this->m_response->setCookie(c);
    }
}

void QbHttpResponse::redirect(const QByteArray &url)
{
    if(this->m_response){
        this->m_response->redirect(url);
    }
}

bool QbHttpResponse::isConnected() const
{
    if(this->m_response){
        return this->m_response->isConnected();
    }
    else{
        return false;
    }
}

void QbHttpResponse::done()
{
    this->deleteLater();
}

}
