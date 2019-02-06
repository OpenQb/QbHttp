#include "qbhttpstaticfilehandler.h"

namespace QbHttp {

QbHttpStaticFileHandler::QbHttpStaticFileHandler()
{

}

QbHttpStaticFileHandler::~QbHttpStaticFileHandler()
{

}

void QbHttpStaticFileHandler::service(HttpRequest &request, HttpResponse &response)
{
    Q_UNUSED(request);
    Q_UNUSED(response);
}

}
