// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    ImageWidget(QWidget *parent = NULL);

public slots:
    void image(QImage pm);
    void image(int pnum, QImage pm) { image(pm); }
    void lookBusy(bool b);

signals:
    void dimensions(QSize s);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

protected:
    QImage m_image;
};

#endif // IMAGEWIDGET_H
