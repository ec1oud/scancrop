#include "mainimagescene.h"
#include "boxtool.h"

#define MOUSE_CLICK_MAX_MOVEMENT 3

MainImageScene::MainImageScene() :
	QGraphicsScene(),
	boxTool(new BoxTool(this)),
	selectTool(new SelectTool(this)),
	tool(selectTool)
{
    mainImageView.setZValue(0);
    mainImageView.setFlag(QGraphicsItem::ItemIsSelectable, false);
    mainImageView.setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    addItem(&mainImageView);
}

void MainImageScene::image(QImage pm)
{
	mainImage = pm;
    mainImageView.setPixmap(QPixmap::fromImage(pm));
//	setSceneRect(0, 0, pm.width(), pm.height());
    mainImageView.setFlag(QGraphicsItem::ItemIsSelectable, false);
}

QColor MainImageScene::colorAt(int x, int y)
{
	return mainImage.pixel(x, y);
}

void MainImageScene::mouseMoveEvent ( QGraphicsSceneMouseEvent * ev)
{
	if (tool)
		tool->mouseMoveEvent(ev);
	else
		QGraphicsScene::mouseMoveEvent(ev);
	emit cursorPos(ev->scenePos());
}

void MainImageScene::mousePressEvent ( QGraphicsSceneMouseEvent * ev)
{
	if (tool)
		tool->mousePressEvent(ev);
	else
		QGraphicsScene::mousePressEvent(ev);
}

void MainImageScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * ev)
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

void MainImageScene::keyPressEvent ( QKeyEvent * ev )
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

QImage MainImageScene::image()
{
    return mainImageView.pixmap().toImage();
}

bool MainImageScene::openImage(QString path)
{
	QPixmap pm(path);
	if (pm.isNull())
		return false;
    mainImage = pm.toImage();
    mainImageView.setPixmap(pm);
    mainImageView.setFlag(QGraphicsItem::ItemIsSelectable, false);
	foreach(QGraphicsItem* i, items())
		if (i->type() == Rectangle::Type)
		{
			removeItem(i);
			delete i;
		}
	return true;
}
