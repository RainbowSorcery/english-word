#include "readconfigutils.h"

#include <QSettings>

ReadConfigUtils::ReadConfigUtils()
{
}

QString ReadConfigUtils::getUrl() {
    QSettings * configIniRead = new QSettings("connection.conf",QSettings::IniFormat);

    QString url = configIniRead->value("settings/url").toString();

    delete configIniRead;

    return url;
}
