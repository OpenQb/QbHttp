#ifndef QBHTTPSETTINGS_P_H
#define QBHTTPSETTINGS_P_H

#include <QMap>
#include <QMutex>
#include <QObject>
#include <QSettings>
#include <QStringList>
#include <QVariantMap>
#include <QMutexLocker>

namespace QbHttp {

class QbHttpSettings : public QSettings
{
private:
    QVariantMap m_map;
    QMutex m_mutex;

public:
    explicit QbHttpSettings(QObject *parent = Q_NULLPTR);
    ~QbHttpSettings();

    QStringList allKeys();
    void setValue(const QString &key,const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
};

}
#endif // QBHTTPSETTINGS_P_H
