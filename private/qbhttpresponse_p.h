#ifndef QBHTTPRESPONSE_P_H
#define QBHTTPRESPONSE_P_H

#include <QObject>
#include <QPointer>
#include <QSharedPointer>

#include <httpresponse.h>

namespace QbHttp {

class QbHttpResponse: public QObject
{
    Q_OBJECT
private:
    stefanfrings::HttpResponse *m_response;
    QVariantMap m_headerMap;
    QVariantMap m_cookieMap;
public:
    explicit QbHttpResponse(stefanfrings::HttpResponse *response);

    ~QbHttpResponse();

public slots:
    void setHeader(QByteArray name, QByteArray value);
    QVariantMap getHeaders();
    QVariantMap getCookies();
    void setStatus(int statusCode, QByteArray description=QByteArray());
    int getStatusCode() const;
    void write(QByteArray data, bool lastPart=false);
    bool hasSentLastPart() const;

    void setCookie(const QByteArray name, const QByteArray value, const int maxAge, const QByteArray path="/", const QByteArray comment=QByteArray(), const QByteArray domain=QByteArray(), const bool secure=false, const bool httpOnly=false);

    void redirect(const QByteArray& url);

    bool isConnected() const;

private:
    void done();

};

}

#endif // QBHTTPRESPONSE_P_H
