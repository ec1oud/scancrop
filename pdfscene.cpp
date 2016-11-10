#include "pdfscene.h"
#include "boxtool.h"

#define MOUSE_CLICK_MAX_MOVEMENT 3

PDFScene::PDFScene() :
	QGraphicsScene(),
	boxTool(new BoxTool(this)),
	selectTool(new SelectTool(this)),
	tool(selectTool)
{
	pdfView.setZValue(0);
	pdfView.setFlag(QGraphicsItem::ItemIsSelectable, false);
	pdfView.setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	addItem(&pdfView);
}

void PDFScene::image(QImage pm)
{
	mainImage = pm;
	pdfView.setPixmap(QPixmap::fromImage(pm));
//	pdfView.setFlags(0);
//	setSceneRect(0, 0, pm.width(), pm.height());
	pdfView.setFlag(QGraphicsItem::ItemIsSelectable, false);
}

QColor PDFScene::colorAt(int x, int y)
{
	return mainImage.pixel(x, y);
}

void PDFScene::mouseMoveEvent ( QGraphicsSceneMouseEvent * ev)
{
	if (tool)
		tool->mouseMoveEvent(ev);
	else
		QGraphicsScene::mouseMoveEvent(ev);
	emit cursorPos(ev->scenePos());
}

void PDFScene::mousePressEvent ( QGraphicsSceneMouseEvent * ev)
{
	if (tool)
		tool->mousePressEvent(ev);
	else
		QGraphicsScene::mousePressEvent(ev);
}

void PDFScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * ev)
{
//	qDebug("mouse release buttons %X", (int)ev->buttons());
	if (tool)
	{
		if ( abs(ev->buttonDownScreenPos(Qt::LeftButton).x() - ev->screenPos().x()) < MOUSE_CLICK_MAX_MOVEMENT &&
			 abs(ev->buttonDownScreenPos(Qt::LeftButton).y() - ev->screenPos().y()) < MOUSE_CLICK_MAX_MOVEMENT )
			tool->mouseLeftClickEvent(ev);
		tool->mouseReleaseEvent(ev);
		update();
	}
	else
		QGraphicsScene::mouseReleaseEvent(ev);
}

void PDFScene::keyPressEvent ( QKeyEvent * ev )
{
	switch (ev->key())
	{
		case Qt::Key_Delete:
			foreach(QGraphicsItem* i, selectedItems())
			{
				removeItem(i);
				delete i;
			}
			break;
		default:
			QGraphicsScene::keyPressEvent(ev);
			break;
	}
}

QImage PDFScene::image()
{
	return pdfView.pixmap().toImage();
}

bool PDFScene::openImage(QString path)
{
	QPixmap pm(path);
	if (pm.isNull())
		return false;
	mainImage = QImage(path);
	pdfView.setPixmap(pm);
//	pdfView.setFlags(0);
	pdfView.setFlag(QGraphicsItem::ItemIsSelectable, false);
	foreach(QGraphicsItem* i, items())
		if (i->type() == Rectangle::Type)
		{
			removeItem(i);
			delete i;
		}
	return true;
}
