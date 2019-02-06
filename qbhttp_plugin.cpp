#include "qbhttp_plugin.h"
#include <qqml.h>

#include <QbHttpServer>

void QbHttpPlugin::registerTypes(const char *uri)
{
    // @uri Qb.Http
    Q_UNUSED(uri);
    QbHttp::QbHttpServer::registerMe();
}

