// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BOXTOOL_H
#define BOXTOOL_H

#include "rectangle.h"
#include "tool.h"
#include <QGraphicsRectItem>

class BoxTool : public Tool
{
public:
    BoxTool(MainImageScene *scene);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
    void mouseLeftClickEvent(QGraphicsSceneMouseEvent * /* ev */) {};

private:
    Rectangle *box;
};

#endif // BOXTOOL_H
