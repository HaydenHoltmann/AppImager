#include "settings.h"
#include <QFile>
#include <QXmlStreamWriter>

Settings::Settings() {}

void Settings::writeSettings(QMap<QString, QString> sMap)
{
    QFile file("/home/hayden/");

    QXmlStreamWriter writer(&file);

    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    for(int i = 0; i < sMap.size(); i++)
    {
        //writer.writeStartElement(sMap);
    }
}

QMap<QString, QString> Settings::readSettings()
{

}
