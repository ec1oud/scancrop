// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QRectF>
#include <QSettings>
#include <QStringList>

/**
 */
class Settings : public QSettings
{
    Q_OBJECT

public:
    static Settings *instance();

    int intOrDefault(QString group, QString key, int defaultVal);
    void setInt(QString group, QString key, int val);
    QString stringOrDefault(QString group, QString key, QString defaultVal = QString());
    void setString(QString group, QString key, QString val);
    QRectF scanGeometry(QString mediaType);
    void setScanGeometry(QString mediaType, QRectF geom);
    /**
        Matrix dimensions: how many columns (the width), how many rows (the height).
        QSize is used just because it's a convenient pre-existing class with 2 ints,
        but the units are floppies (or other media) rather than pixels.
    */
    QStringList allMediaTypes() { return m_allMediaTypes; }
    QString chosenScanner() { return stringOrDefault("main", "scanner"); }
    void chosenScanner(QString v) { setString("main", "scanner", v); }

protected:
    Settings(QObject *parent = NULL);
    ~Settings();
    QRectF parseGeometryString(QString geom);
    QSize parseSizeString(QString geom);
    QString toString(QRectF r);
    QStringList m_allMediaTypes;
};

#endif // SETTINGS_H
