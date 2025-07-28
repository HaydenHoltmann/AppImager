#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMap>

class Settings
{
public:
    Settings();
    void writeSettings(QMap<QString, QString> sMap);
    QMap<QString, QString> readSettings();
    void setSettings(QMap<QString, QString> sMap);

private:
    QMap<QString, QString> settingsMap;
};

#endif // SETTINGS_H
