// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QGraphicsRectItem>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include <opencv2/core/types.hpp>

class Rectangle : public QGraphicsPolygonItem
{
public:
    Rectangle(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = 0);
    Rectangle(const cv::RotatedRect &rr, QGraphicsItem *parent = 0);
    Rectangle(const std::vector<cv::Point> &pts, QGraphicsItem *parent = 0);
    Rectangle(QXmlStreamReader &r);

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    enum ResizeHandleIdx { RH_NONE, RH_TOP_LEFT, RH_TOP_RIGHT, RH_BOTTOM_RIGHT, RH_BOTTOM_LEFT };

    ResizeHandleIdx handleAt(QPointF pos);

    void startResize();

    void resize(ResizeHandleIdx hidx, QPointF pos, bool constrained = true);

    void rotate(bool clockwise);

    int handleWidth();

    double actualWidth() const;
    double actualHeight() const;

    QRectF boundingRect() const;

    /**
        Degrees counter-clockwise from the default rotation
        in which the visibly distinct baseline is horizontal
        at the bottom of the rectangle.
    */
    qreal rotation() const;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void writeXML(QXmlStreamWriter &w);

protected:
    mutable double width = 0;
    mutable double height = 0;
    double aspect = 0;
    double sineA = 0;
    double cosineA = 1;
    double sineB = 0;
    double cosineB = 1;
    static QPixmap *resizeHandle;
    // bool m_constrained = true; // if false, it can be a trapezoid

    friend QDebug operator<<(QDebug debug, const Rectangle &option);
};

QDebug operator<<(QDebug debug, const Rectangle &option);

#endif // RECTANGLE_H
