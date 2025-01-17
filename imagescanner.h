// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IMAGESCANNER_H
#define IMAGESCANNER_H
#include <sane/sane.h>

#include <QFileInfo>
#include <QImage>
#include <QObject>
#include <QThread>

class ImageScanner : public QThread
{
    Q_OBJECT
public:
    static ImageScanner *instance();
    void scan(QRectF bounds);
    QStringList allScannerNames();
    QString scannerDev(int idx);
    QSizeF maxSize() { return m_maxSize; }

    /**
        @param path	to directory
    */
    void setScanDir(const QString &path);

    /**
        @param start inclusive starting index
        @param end  exclusive ending index, e.g. if it's 1 then
                    foo0000.jpg will be created but foo0001.jpg will not.
    */
    void setSequence(int start = 0, int end = 1);

    QFileInfo nextImageOutput();

public slots:

signals:
    void progress(int val);
    void progressRange(int zero, int max);
    void done(QImage img);

protected:
    void run() override;

private:
    ImageScanner();
    void setOptions(SANE_Handle dev);
    void getOptions(SANE_Handle dev);
    QString unitName(SANE_Unit unit);

    QList<const SANE_Device *> m_allDevices;
    SANE_Handle m_scanner;
    QSizeF m_maxSize;
    QRectF m_bounds;
    int m_sequenceStart;
    int m_sequenceEnd;
    int m_sequence;
    // QFileInfo m_outputImagePath;
};

#endif // IMAGESCANNER_H
