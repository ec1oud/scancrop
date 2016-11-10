#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
class PDFScene;

class Tool : public QObject
{
	Q_OBJECT
public:
	Tool(PDFScene* scn);
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * ev) = 0;
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * ev) = 0;
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * ev) = 0;
	virtual void mouseLeftClickEvent ( QGraphicsSceneMouseEvent * ev) = 0;

protected:
	PDFScene* scene;
};

#endif // TOOL_H
