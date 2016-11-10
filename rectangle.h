#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QGraphicsRectItem>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class Rectangle : public QGraphicsPolygonItem
{
public:
	Rectangle(qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0);
	Rectangle(QXmlStreamReader& r);

	enum { Type = UserType + 1 };
	int type() const { return Type; }

	enum ResizeHandleIdx {
		RH_NONE,
		RH_TOP_LEFT,
		RH_TOP_RIGHT,
		RH_BOTTOM_RIGHT,
		RH_BOTTOM_LEFT	 };

	ResizeHandleIdx handleAt(QPointF pos);

	void startResize();

	void resize(ResizeHandleIdx hidx, QPointF pos);

	void rotate(bool clockwise);

	int handleWidth();

	double actualWidth();
	double actualHeight();

	QRectF boundingRect () const;

	/**
		Degrees counter-clockwise from the default rotation
		in which the visibly distinct baseline is horizontal
		at the bottom of the rectangle.
	*/
	qreal rotation() const;

	virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

	void writeXML(QXmlStreamWriter& w);

protected:
	double width;
	double height;
	double aspect;
	double sineA;
	double cosineA;
	double sineB;
	double cosineB;
	static QPixmap* resizeHandle;
};

#endif // RECTANGLE_H
