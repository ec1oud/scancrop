// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
class MainImageScene;

class Tool : public QObject
{
	Q_OBJECT
public:
	Tool(MainImageScene* scn);
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * ev) = 0;
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * ev) = 0;
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * ev) = 0;
	virtual void mouseLeftClickEvent ( QGraphicsSceneMouseEvent * ev) = 0;

protected:
	MainImageScene* scene;
};

#endif // TOOL_H
