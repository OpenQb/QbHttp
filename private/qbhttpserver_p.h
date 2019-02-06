#ifndef QBHTTPSERVER_P_H
#define QBHTTPSERVER_P_H

#include <QObject>
#include <QJSValue>
#include <QSettings>
#include <QQmlParserStatus>
#include <QQmlApplicationEngine>

#include "qbhttpsettings_p.h"
#include "qbhttprequest_p.h"
#include "qbhttpresponse_p.h"
#include "qbhttprequesthandler_p.h"

#include <httplistener.h>

namespace QbHttp {

class QbHttpServer;
class QbHttpRequestHandler;
class QbHttpSmartRequestHandler;

class QbHttpServer : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    //listener
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(qint32 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(qint32 minThreads READ minThreads WRITE setMinThreads NOTIFY minThreadsChanged)
    Q_PROPERTY(qint32 maxThreads READ maxThreads WRITE setMaxThreads NOTIFY maxThreadsChanged)
    Q_PROPERTY(qint32 cleanupInterval READ cleanupInterval WRITE setCleanupInterval NOTIFY cleanupIntervalChanged)
    Q_PROPERTY(qint32 readTimeout READ readTimeout WRITE setReadTimeout NOTIFY readTimeoutChanged)
    Q_PROPERTY(qint32 maxRequestSize READ maxRequestSize WRITE setMaxRequestSize NOTIFY maxRequestSizeChanged)
    Q_PROPERTY(qint32 maxMultiPartSize READ maxMultiPartSize WRITE setMaxMultiPartSize NOTIFY maxMultiPartSizeChanged)

    //docroot
    Q_PROPERTY(QString docRoot READ docRoot WRITE setDocRoot NOTIFY docRootChanged)
    Q_PROPERTY(QString encoding READ encoding WRITE setEncoding NOTIFY encodingChanged)
    Q_PROPERTY(qint32 cacheMaxAge READ cacheMaxAge WRITE setCacheMaxAge NOTIFY cacheMaxAgeChanged)
    Q_PROPERTY(qint32 cacheTime READ cacheTime WRITE setCacheTime NOTIFY cacheTimeChanged)
    Q_PROPERTY(qint32 cacheSize READ cacheSize WRITE setCacheSize NOTIFY cacheSizeChanged)
    Q_PROPERTY(qint32 maxCachedFileSize READ maxCachedFileSize WRITE setMaxCachedFileSize NOTIFY maxCachedFileSizeChanged)

    //other settings
    Q_PROPERTY(QString callBack READ callBack WRITE setCallBack NOTIFY callBackChanged)
    Q_PROPERTY(QString serverJsPath READ serverJsPath WRITE setServerJsPath NOTIFY serverJsPathChanged)
    Q_PROPERTY(QString templateFilePath READ templateFilePath WRITE setTemplateFilePath NOTIFY templateFilePathChanged)
    Q_PROPERTY(QString serverCode READ serverCode WRITE setServerCode NOTIFY serverCodeChanged)

    Q_PROPERTY(QbHttpServerMode serverMode READ serverMode WRITE setServerMode NOTIFY serverModeChanged)

public:
    enum QbHttpServerMode { DYNAMIC,STATIC,MULTI_FOLDER_STATIC};
    Q_ENUM(QbHttpServerMode)

private:
    friend class QbHttpSmartRequestHandler;

private:
    //listener
    QString m_host;
    qint32 m_port;
    qint32 m_minThreads;
    qint32 m_maxThreads;
    qint32 m_cleanupInterval;
    qint32 m_readTimeout;
    qint32 m_maxRequestSize;
    qint32 m_maxMultiPartSize;

    //docroot
    QString m_docRoot;
    QString m_encoding;
    qint32 m_cacheMaxAge;
    qint32 m_cacheTime;
    qint32 m_cacheSize;
    qint32 m_maxCachedFileSize;

    QSettings *m_settings;
    stefanfrings::HttpRequestHandler *m_handler;
    stefanfrings::HttpListener *m_listener;

    QString m_callBack;
    QString m_serverJsPath;
    QString m_serverCode;
    QQmlEngine *m_engine;
    QbHttpServerMode m_serverMode;

    QVariantMap m_staticRootFolderMap;

    QString m_templateFilePath;

public:
    explicit QbHttpServer(QObject *parent = Q_NULLPTR);

    ~QbHttpServer();

    void classBegin();
    void componentComplete();



    QVariantMap getStaticRootFolderMap();

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief registerMe
    ///
    /// Register all types to qml namespace
    ///////////////////////////////////////////////////////////////////////////////////////////////
    static void registerMe();

public slots:
    void listen();
    void stop();

    void setCallBack(QString callBack);

    void addStaticRootFolder(const QString &key, const QString &value);
    void clearStaticRootFolder();

private slots:
    void emitLogMessage(const QString &message);

signals:
    void listenIpAddress(const QString &ipaddress);
    void logMessage(const QString &message);

public:
    qint32 port() const
    {
        return m_port;
    }

    QString docRoot() const
    {
        return m_docRoot;
    }

    QString host() const
    {
        return m_host;
    }

    qint32 minThreads() const
    {
        return m_minThreads;
    }

    qint32 maxThreads() const
    {
        return m_maxThreads;
    }

    qint32 cleanupInterval() const
    {
        return m_cleanupInterval;
    }

    qint32 readTimeout() const
    {
        return m_readTimeout;
    }

    qint32 maxRequestSize() const
    {
        return m_maxRequestSize;
    }

    qint32 maxMultiPartSize() const
    {
        return m_maxMultiPartSize;
    }

    QString encoding() const
    {
        return m_encoding;
    }

    qint32 cacheMaxAge() const
    {
        return m_cacheMaxAge;
    }

    qint32 cacheTime() const
    {
        return m_cacheTime;
    }

    qint32 cacheSize() const
    {
        return m_cacheSize;
    }

    qint32 maxCachedFileSize() const
    {
        return m_maxCachedFileSize;
    }

    QString callBack() const
    {
        return m_callBack;
    }

    QString serverJsPath() const
    {
        return m_serverJsPath;
    }

    QbHttpServerMode serverMode() const
    {
        return m_serverMode;
    }

    QString templateFilePath() const
    {
        return m_templateFilePath;
    }

    QString serverCode() const
    {
        return m_serverCode;
    }

signals:
    void portChanged(qint32 port);

    void docRootChanged(QString docRoot);

    void hostChanged(QString host);

    void minThreadsChanged(qint32 minThreads);

    void maxThreadsChanged(qint32 maxThreads);

    void cleanupIntervalChanged(qint32 cleanupInterval);

    void readTimeoutChanged(qint32 readTimeout);

    void maxRequestSizeChanged(qint32 maxRequestSize);

    void maxMultiPartSizeChanged(qint32 maxMultiPartSize);

    void encodingChanged(QString encoding);

    void cacheMaxAgeChanged(qint32 cacheMaxAge);

    void cacheTimeChanged(qint32 cacheTime);

    void cacheSizeChanged(qint32 cacheSize);

    void maxCachedFileSizeChanged(qint32 maxCachedFileSize);

    void callBackChanged(QJSValue callBack);

    void serverJsPathChanged(QString serverJsPath);

    void serverModeChanged(QbHttpServerMode serverMode);

    void templateFilePathChanged(QString templateFilePath);

    void serverCodeChanged(QString serverCode);

public slots:
    void setPort(qint32 port)
    {
        if (m_port == port)
            return;

        m_port = port;
        emit portChanged(m_port);
    }
    void setDocRoot(QString docRoot)
    {
        if (m_docRoot == docRoot)
            return;

        m_docRoot = docRoot;
        emit docRootChanged(m_docRoot);
    }
    void setHost(QString host)
    {
        if (m_host == host)
            return;

        m_host = host;
        emit hostChanged(m_host);
    }
    void setMinThreads(qint32 minThreads)
    {
        if (m_minThreads == minThreads)
            return;

        m_minThreads = minThreads;
        emit minThreadsChanged(m_minThreads);
    }
    void setMaxThreads(qint32 maxThreads)
    {
        if (m_maxThreads == maxThreads)
            return;

        m_maxThreads = maxThreads;
        emit maxThreadsChanged(m_maxThreads);
    }
    void setCleanupInterval(qint32 cleanupInterval)
    {
        if (m_cleanupInterval == cleanupInterval)
            return;

        m_cleanupInterval = cleanupInterval;
        emit cleanupIntervalChanged(m_cleanupInterval);
    }
    void setReadTimeout(qint32 readTimeout)
    {
        if (m_readTimeout == readTimeout)
            return;

        m_readTimeout = readTimeout;
        emit readTimeoutChanged(m_readTimeout);
    }
    void setMaxRequestSize(qint32 maxRequestSize)
    {
        if (m_maxRequestSize == maxRequestSize)
            return;

        m_maxRequestSize = maxRequestSize;
        emit maxRequestSizeChanged(m_maxRequestSize);
    }
    void setMaxMultiPartSize(qint32 maxMultiPartSize)
    {
        if (m_maxMultiPartSize == maxMultiPartSize)
            return;

        m_maxMultiPartSize = maxMultiPartSize;
        emit maxMultiPartSizeChanged(m_maxMultiPartSize);
    }
    void setEncoding(QString encoding)
    {
        if (m_encoding == encoding)
            return;

        m_encoding = encoding;
        emit encodingChanged(m_encoding);
    }
    void setCacheMaxAge(qint32 cacheMaxAge)
    {
        if (m_cacheMaxAge == cacheMaxAge)
            return;

        m_cacheMaxAge = cacheMaxAge;
        emit cacheMaxAgeChanged(m_cacheMaxAge);
    }
    void setCacheTime(qint32 cacheTime)
    {
        if (m_cacheTime == cacheTime)
            return;

        m_cacheTime = cacheTime;
        emit cacheTimeChanged(m_cacheTime);
    }
    void setCacheSize(qint32 cacheSize)
    {
        if (m_cacheSize == cacheSize)
            return;

        m_cacheSize = cacheSize;
        emit cacheSizeChanged(m_cacheSize);
    }
    void setMaxCachedFileSize(qint32 maxCachedFileSize)
    {
        if (m_maxCachedFileSize == maxCachedFileSize)
            return;

        m_maxCachedFileSize = maxCachedFileSize;
        emit maxCachedFileSizeChanged(m_maxCachedFileSize);
    }
    void setServerJsPath(QString serverJsPath)
    {
        if (m_serverJsPath == serverJsPath)
            return;

        m_serverJsPath = serverJsPath;
        emit serverJsPathChanged(m_serverJsPath);
    }
    void setServerMode(QbHttpServerMode serverMode)
    {
        if (m_serverMode == serverMode)
            return;

        m_serverMode = serverMode;
        emit serverModeChanged(m_serverMode);
    }
    void setTemplateFilePath(QString templateFilePath)
    {
        if (m_templateFilePath == templateFilePath)
            return;

        m_templateFilePath = templateFilePath;
        emit templateFilePathChanged(m_templateFilePath);
    }
    void setServerCode(QString serverCode)
    {
        if (m_serverCode == serverCode)
            return;

        m_serverCode = serverCode;
        emit serverCodeChanged(m_serverCode);
    }
};

}

#endif // QBHTTPSERVER_P_H
