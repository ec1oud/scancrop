#include "boxtool.h"
#include "pdfscene.h"

BoxTool::BoxTool(PDFScene* scene) :
	Tool(scene),
	box(nullptr)
{
}

void BoxTool::mousePressEvent ( QGraphicsSceneMouseEvent * ev)
{
//	box = scene->addRect(ev->scenePos().x(), ev->scenePos().y(), 10, 10);
	box = new Rectangle(ev->scenePos().x(), ev->scenePos().y(), 10, 10);
	scene->addItem(box);
	box->setZValue(1.0);
}

void BoxTool::mouseMoveEvent ( QGraphicsSceneMouseEvent * ev)
{
	if (box)
	{
		QRectF rect = box->polygon().boundingRect();
		rect.setBottomRight(ev->scenePos());
		box->setPolygon(QPolygonF(rect));
	}
}

void BoxTool::mouseReleaseEvent ( QGraphicsSceneMouseEvent * /* ev */)
{
//	if (box)
//		box->setRect(box->rect().normalized());
	box = nullptr;
}
