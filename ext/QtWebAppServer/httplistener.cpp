/**
  @file
  @author Stefan Frings
*/

#include "httplistener.h"
#include "httpconnectionhandler.h"
#include "httpconnectionhandlerpool.h"
#include <QCoreApplication>
#include "httpserverlogging.h"

using namespace stefanfrings;

HttpListener::HttpListener(QSettings* settings, HttpRequestHandler* requestHandler, QObject *parent)
    : QTcpServer(parent)
{
    Q_ASSERT(settings!=0);
    Q_ASSERT(requestHandler!=0);
    pool=NULL;
    this->settings=settings;
    this->requestHandler=requestHandler;
    // Reqister type of socketDescriptor for signal/slot handling
    qRegisterMetaType<tSocketDescriptor>("tSocketDescriptor");
    // Start listening
    //listen();
    //qCDebug(HttpServer_Native()) << this->settings->value("host").toString();
}


HttpListener::~HttpListener()
{
    close();
    qCDebug(HttpServer_Native(),"HttpListener: destroyed");
}


void HttpListener::listen()
{
    if (!pool)
    {
        pool=new HttpConnectionHandlerPool(settings,requestHandler);
    }
    QString host = settings->value("host").toString();
    int port=settings->value("port").toInt();
    QTcpServer::listen(host.isEmpty() ? QHostAddress::Any : QHostAddress(host), port);
    if (!isListening())
    {
        qCCritical(HttpServer_Native(),"HttpListener: Cannot bind on port %i: %s",port,qPrintable(errorString()));
    }
    else {
        qCDebug(HttpServer_Native(),"HttpListener: Listening on port %i",port);
    }
}


void HttpListener::close() {
    QTcpServer::close();
    qCDebug(HttpServer_Native(),"HttpListener: closed");
    if (pool) {
        delete pool;
        pool=NULL;
    }
}

void HttpListener::incomingConnection(tSocketDescriptor socketDescriptor) {
#ifdef SUPERVERBOSE
    qCDebug(HttpServer_Native(),"HttpListener: New connection");
#endif

    HttpConnectionHandler* freeHandler=NULL;
    if (pool)
    {
        freeHandler=pool->getConnectionHandler();
    }

    // Let the handler process the new connection.
    if (freeHandler)
    {
        // The descriptor is passed via event queue because the handler lives in another thread
        QMetaObject::invokeMethod(freeHandler, "handleConnection", Qt::QueuedConnection, Q_ARG(tSocketDescriptor, socketDescriptor));
    }
    else
    {
        // Reject the connection
        qCDebug(HttpServer_Native(),"HttpListener: Too many incoming connections");
        QTcpSocket* socket=new QTcpSocket(this);
        socket->setSocketDescriptor(socketDescriptor);
        connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
        socket->write("HTTP/1.1 503 too many connections\r\nConnection: close\r\n\r\nToo many connections\r\n");
        socket->disconnectFromHost();
    }
}
