#ifndef BOXTOOL_H
#define BOXTOOL_H

#include "tool.h"
#include "rectangle.h"
#include <QGraphicsRectItem>

class BoxTool : public Tool
{
public:
	BoxTool(PDFScene* scene);
	void mouseMoveEvent ( QGraphicsSceneMouseEvent * ev);
	void mousePressEvent ( QGraphicsSceneMouseEvent * ev);
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * ev);
    void mouseLeftClickEvent ( QGraphicsSceneMouseEvent * /* ev */) {};

private:
	Rectangle* box;
};

#endif // BOXTOOL_H
