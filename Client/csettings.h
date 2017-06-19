#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <QSettings>

class CSettings
{
public:
    CSettings();
    CSettings(const CSettings&)             = delete;
    CSettings& operator =(const CSettings&) = delete;
    ~CSettings();

    bool writeSetting(const QString& key, const QString& value);
    const QString readSetting(const QString& key) const;

private:
    QSettings* m_pSetings;
};

#endif // CSETTINGS_H
