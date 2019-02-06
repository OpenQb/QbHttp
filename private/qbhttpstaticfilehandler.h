#ifndef QBHTTPSTATICFILEHANDLER_H
#define QBHTTPSTATICFILEHANDLER_H

#include <httprequest.h>
#include <httpresponse.h>
#include <httpconnectionhandler.h>


using namespace stefanfrings;

namespace QbHttp {

class QbHttpStaticFileHandler : public HttpRequestHandler
{
    //Q_OBJECT
    Q_DISABLE_COPY(QbHttpStaticFileHandler)
public:
    QbHttpStaticFileHandler();
    ~QbHttpStaticFileHandler() Q_DECL_OVERRIDE;
    virtual void service(HttpRequest& request, HttpResponse& response) Q_DECL_OVERRIDE;
};

}
#endif // QBHTTPSTATICFILEHANDLER_H
