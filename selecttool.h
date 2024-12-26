// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SelectTOOL_H
#define SelectTOOL_H

#include "rectangle.h"
#include "tool.h"
#include <QGraphicsRectItem>

class SelectTool : public Tool
{
    Q_OBJECT
public:
    SelectTool(MainImageScene *scene);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
    void mouseLeftClickEvent(QGraphicsSceneMouseEvent *ev);
    void setConstrainedResize(bool c) { m_constrainedResize = c; }

signals:
    void selectionChanged();

private:
    QGraphicsRectItem *rubberBand;
    Rectangle *resizing;
    Rectangle *dragging;
    Rectangle::ResizeHandleIdx resizingHandle;
    bool m_constrainedResize = true;
};

#endif // SelectTOOL_H
