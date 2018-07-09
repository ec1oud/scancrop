#include "selecttool.h"
#include "mainimagescene.h"

SelectTool::SelectTool(MainImageScene* scene) :
	Tool(scene),
	rubberBand(nullptr),
	resizing(nullptr),
	dragging(nullptr)
{
	QPen dashPen(Qt::black);
	QVector<qreal> dashes;
	dashes << 2 << 2;
	dashPen.setDashPattern(dashes);
	rubberBand = scene->addRect(0, 0, 10, 10, dashPen);
	rubberBand->setVisible(false);
}

void SelectTool::mousePressEvent ( QGraphicsSceneMouseEvent * ev)
{
	QPointF pos = ev->scenePos();
	QList<QGraphicsItem *> itemsHere = scene->items(pos);
//	qDebug("on mouse press, found %d items", itemsHere.count());
//	QGraphicsItem* sel = scene->itemAt(pos);
	dragging = nullptr;
	resizing = nullptr;
	if (itemsHere.count() > 0 && itemsHere[0]->type() > QGraphicsItem::UserType)
	{
		// Mouse was pressed on a UserType item.
		// Assume that all UserType items can be resized.
		/// @todo We need an intermediate class for such items, but for now
		/// there is only Rectangle.
		Rectangle* item = (Rectangle*)itemsHere[0];
		if (item->isSelected())
		{
			// See which resize handle, if any, is under the cursor.
			resizingHandle = item->handleAt(ev->scenePos());
			qDebug("pressed handle %d, polygon corner %d", resizingHandle, ((int)resizingHandle) - 1);
			resizing = (resizingHandle == Rectangle::RH_NONE ? nullptr : item);
			if (resizing)
				resizing->startResize();
		}
		else
			item->setSelected(true);
		if (!resizing)
			dragging = item;
	}
	else
	{
		rubberBand->setVisible(true);
		rubberBand->setZValue(1.0);
		rubberBand->setRect(QRectF(ev->scenePos(), QSize(0, 0)));
	}
}

void SelectTool::mouseMoveEvent ( QGraphicsSceneMouseEvent * ev)
{
	if (ev->buttons())
	{
		if (resizing)
			resizing->resize(resizingHandle, ev->scenePos());
		else if (dragging)
		{
			QPointF polyOrigin = dragging->mapToScene(dragging->polygon()[3]);
			dragging->moveBy(ev->scenePos().x() - polyOrigin.x(),
							ev->scenePos().y() - polyOrigin.y());
//			dragging->setPos(ev->scenePos());
//			dragging->setPos(dragging->mapToScene(ev->pos())); // no dragging occurs
//			dragging->setPos(ev->pos());
		}
		else
		{
			QRectF rect = rubberBand->rect();
			rect.setBottomRight(ev->scenePos());
			rubberBand->setRect(rect);
		}
	}
}

void SelectTool::mouseReleaseEvent ( QGraphicsSceneMouseEvent * ev )
{
	if (!ev->isAccepted())
	{
		// If it was not a "click"
		rubberBand->setVisible(false);
		if (resizing)
		{
//			resizing->setRect(resizing->rect().normalized());
			resizing = nullptr;
		}
		else
		{
			QPainterPath rect;
			rect.addRect(rubberBand->rect());
			scene->setSelectionArea(rect);
			foreach(QGraphicsItem* i, scene->selectedItems())
				if (i->type() < QGraphicsItem::UserType)
					i->setSelected(false);
			qDebug("selected %d items", scene->selectedItems().count());
		}
		dragging = nullptr;
	}
	emit selectionChanged();
}

void SelectTool::mouseLeftClickEvent ( QGraphicsSceneMouseEvent * ev)
{
	QPointF pos = ev->scenePos();
//	qDebug("mouseLeftClickEvent at %f, %f", pos.x(), pos.y());
	QList<QGraphicsItem *> itemsHere = scene->items(pos);
//	qDebug("on mouse left click, found %d items", itemsHere.count());
//	QGraphicsItem* sel = scene->itemAt(pos);
	foreach(QGraphicsItem* i, scene->selectedItems())
		i->setSelected(false);
	if (itemsHere.count() > 0)
	{
		QGraphicsItem* item = itemsHere[0];
		if (item->type() > QGraphicsItem::UserType)
		{
			item->setSelected(true);
			QRectF bounds = item->boundingRect();
			qDebug("first item found at location is of type %d; selected? %d; bounds %d, %d %d x %d",
				   item->type(), item->isSelected(), (int)bounds.x(),
				   (int)bounds.y(), (int)bounds.width(), (int)bounds.height());
		}
//		if (item->type() > QGraphicsItem::UserType)
//		{
//			bounds = ((Rectangle*)item)->rect();
//			qDebug("    rect is %d, %d %d x %d", (int)bounds.x(), (int)bounds.y(), (int)bounds.width(), (int)bounds.height());
//		}
//		item->update();
	}
	ev->accept();
}
