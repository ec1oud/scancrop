// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MAINIMAGESCENE_H
#define MAINIMAGESCENE_H

#include "boxtool.h"
#include "selecttool.h"
#include "tool.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include <QKeyEvent>

class MainImageScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MainImageScene();
    BoxTool *boxTool;
    SelectTool *selectTool;
    QColor colorAt(int x, int y);
    QVector<QRectF> detectPhotoBoundaries();
    QGraphicsPixmapItem &imageView() { return mainImageView; }

signals:
    void cursorPos(QPointF pos);
    void imageOpened(const QString &path);

public slots:
    void image(QImage pm);
    void image(int /* pnum */, QImage pm) { image(pm); }
    QImage image();
    void setTool(Tool *t) { tool = t; }
    bool openImage(QString path);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
    void keyPressEvent(QKeyEvent *ev);

private:
    QImage mainImage;
    QGraphicsPixmapItem mainImageView;
    Tool *tool;
};

#endif // MAINIMAGESCENE_H
