#include "csettings.h"

CSettings::CSettings() : m_pSetings(nullptr)
{
    m_pSetings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "client");
}

CSettings::~CSettings()
{
    delete m_pSetings;
}

bool CSettings::writeSetting(const QString &key, const QString &value)
{
    bool isWritted = false;

    if (m_pSetings != nullptr)
    {
        m_pSetings->setValue(key, value);
        isWritted = true;
    }

    return isWritted;
}

const QString CSettings::readSetting(const QString &key) const
{
    return m_pSetings->value(key).toString();
}
