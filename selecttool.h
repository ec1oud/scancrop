#ifndef SelectTOOL_H
#define SelectTOOL_H

#include "tool.h"
#include "rectangle.h"
#include <QGraphicsRectItem>

class SelectTool : public Tool
{
	Q_OBJECT
public:
	SelectTool(PDFScene* scene);
	void mouseMoveEvent ( QGraphicsSceneMouseEvent * ev);
	void mousePressEvent ( QGraphicsSceneMouseEvent * ev);
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * ev);
	void mouseLeftClickEvent ( QGraphicsSceneMouseEvent * ev);

signals:
	void selectionChanged();

private:
	QGraphicsRectItem* rubberBand;
	Rectangle* resizing;
	Rectangle* dragging;
	Rectangle::ResizeHandleIdx resizingHandle;
};

#endif // SelectTOOL_H
