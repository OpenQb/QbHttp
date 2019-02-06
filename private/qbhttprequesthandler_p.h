#ifndef QBHTTPREQUESTHANDLER_H
#define QBHTTPREQUESTHANDLER_H


#include <QJSValue>
#include <QJSEngine>

#include <httprequest.h>
#include <httpresponse.h>
#include <httpconnectionhandler.h>

#include "qbhttpserver_p.h"



using namespace stefanfrings;


namespace QbHttp {
class QbHttpServer;
class QbHttpRequestHandler;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The QbHttpRequestHandler class
///
/// Request Handler for dynamic contents
///////////////////////////////////////////////////////////////////////////////////////////////////

class QbHttpRequestHandler : public HttpRequestHandler
{
    //Q_OBJECT
    Q_DISABLE_COPY(QbHttpRequestHandler)
private:
    QString m_callBack;
    QString m_serverJsPath;
    QString m_serverCode;
    QJSEngine m_engine;

public:
    explicit QbHttpRequestHandler(QString serverCode,QString serverJsPath,QString callBack, QObject *parent);

    ~QbHttpRequestHandler();

    virtual void service(HttpRequest& request, HttpResponse& response) Q_DECL_OVERRIDE;

    QByteArray data(const QString &path);
};

}
#endif // QBHTTPREQUESTHANDLER_H
