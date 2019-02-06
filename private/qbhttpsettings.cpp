#include "qbhttpsettings_p.h"
#include "qbhttplogging_p.h"

namespace QbHttp {

QbHttpSettings::QbHttpSettings(QObject *parent):
    QSettings(parent)
{
    qCDebug(QbHttp_Native) << "QbHttpSettings created.";
}

QbHttpSettings::~QbHttpSettings()
{
    qCDebug(QbHttp_Native) << "QbHttpSettings deleted.";
}

QStringList QbHttpSettings::allKeys()
{
    QMutexLocker locker(&this->m_mutex);
    return this->m_map.keys();
}

void QbHttpSettings::setValue(const QString &key, const QVariant &value)
{
    QMutexLocker locker(&this->m_mutex);
    this->m_map.insert(key,value);
}

QVariant QbHttpSettings::value(const QString &key, const QVariant &defaultValue) const
{
    if(this->m_map.contains(key)) return this->m_map.value(key);
    return defaultValue;
}

}
