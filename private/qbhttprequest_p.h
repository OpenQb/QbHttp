#ifndef QBHTTPREQUEST_P_H
#define QBHTTPREQUEST_P_H

#include <QObject>
#include <QSharedPointer>

#include <httprequest.h>

namespace QbHttp {

class QbHttpRequest: public QObject
{
    Q_OBJECT
private:
    stefanfrings::HttpRequest *m_request;

public:
    explicit QbHttpRequest(stefanfrings::HttpRequest *request);

    ~QbHttpRequest();

public slots:
    int getStatus();
    QByteArray getMethod() const;
    QByteArray getPath() const;
    QByteArray getRawPath() const;
    QByteArray getVersion() const;
    QByteArray getHeader(const QByteArray &name);
    QByteArray getParameter(const QByteArray &name);
    QByteArray getBody();
    QByteArray urlDecode(const QByteArray &source);
    QByteArray getCookie(const QByteArray &name) const;
    QByteArray getPeerAddress() const;

private:
    void done();
};

}

#endif // QBHTTPREQUEST_P_H
