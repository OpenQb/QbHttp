#ifndef QBHTTPSMARTREQUESTHANDLER_P_H
#define QBHTTPSMARTREQUESTHANDLER_P_H

#include <QObject>
#include <QString>
#include <QSettings>

#include <httprequest.h>
#include <httpresponse.h>
#include <httprequesthandler.h>

#include "qbhttpserver_p.h"

using namespace stefanfrings;

namespace QbHttp {
class QbHttpServer;

class QbHttpSmartRequestHandler : public HttpRequestHandler
{
    //Q_OBJECT
    Q_DISABLE_COPY(QbHttpSmartRequestHandler)
private:
    QbHttpServer *m_server;

    QVariantMap m_staticRootFolderMap;
    QString m_encoding;

    QString m_docRoot;
    QString m_templateFilePath;

    QbHttpServer::QbHttpServerMode m_serverMode;

public:
    QbHttpSmartRequestHandler(QbHttpServer *server);

    ~QbHttpSmartRequestHandler();

    virtual void service(HttpRequest& request, HttpResponse& response) Q_DECL_OVERRIDE;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief setContentType
    /// \param _fileName
    /// \param response
    ///
    /// Set content type depending on the file ending
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void setContentType(QString _fileName, HttpResponse& response) const;

//signals:
//    void logMessage(const QString &message);
};
}

#endif // QBHTTPSMARTREQUESTHANDLER_P_H
