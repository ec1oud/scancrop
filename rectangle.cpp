// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainimagescene.h"
#include "rectangle.h"
#include <QGraphicsSceneContextMenuEvent>
#include <QInputDialog>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QtDebug>
#include <math.h>

QPixmap *Rectangle::resizeHandle(nullptr);

#define HANDLE_MARGIN 10
#define HANDLE_VIRTUAL_SIZE 50

Rectangle::Rectangle(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) : QGraphicsPolygonItem(parent)
{
    QPolygonF poly(QRectF(x, y, w, h));
    setPolygon(poly);
    setFlags(/*QGraphicsItem::ItemIsMovable |*/ QGraphicsItem::ItemIsSelectable);
    setZValue(1.0);
    setPen({Qt::green, 0});
}

Rectangle::Rectangle(const cv::RotatedRect &rr, QGraphicsItem *parent) : QGraphicsPolygonItem(parent)
{
    cv::Point2f pts[4];
    rr.points(pts);
    // rr.points():
    // bottomLeft, topLeft, topRight, bottomRight
    // QPolygonF wants:
    // the four vertices of the rectangle in clockwise order starting and ending with the top-left vertex.
    QPolygonF poly;
    poly << QPointF(pts[1].x, pts[1].y);
    poly << QPointF(pts[2].x, pts[2].y);
    poly << QPointF(pts[3].x, pts[3].y);
    poly << QPointF(pts[0].x, pts[0].y);
    poly << QPointF(pts[1].x, pts[1].y);
    setPolygon(poly);
    setFlags(QGraphicsItem::ItemIsSelectable);
    setZValue(1.0);
    setPen({Qt::green, 0});
}

Rectangle::Rectangle(const std::vector<cv::Point> &pts, QGraphicsItem *parent) : QGraphicsPolygonItem(parent)
{
    // topLeft, bottomLeft, bottomRight, topRight
    // QPolygonF wants:
    // the four vertices of the rectangle in clockwise order starting and ending with the top-left vertex.
    QPolygonF poly;
    poly << QPointF(pts[0].x, pts[0].y);
    poly << QPointF(pts[3].x, pts[3].y);
    poly << QPointF(pts[2].x, pts[2].y);
    poly << QPointF(pts[1].x, pts[1].y);
    poly << QPointF(pts[0].x, pts[0].y);
    setPolygon(poly);
    setFlags(QGraphicsItem::ItemIsSelectable);
    setZValue(1.0);
    setPen({Qt::green, 0});
}

Rectangle::Rectangle(QXmlStreamReader &r)
{
    QPolygonF poly(5);
    bool done = false;
    QPointF p;
    int pidx = 0;
    QString tagName;
    while (!done && !r.atEnd()) {
        QXmlStreamReader::TokenType t = r.tokenType();
//		qDebug() << "rect constructor looking at token type" << r.tokenString();
        switch (t) {
            case QXmlStreamReader::StartElement:
                tagName = r.name().toString();
                break;
            case QXmlStreamReader::Characters: {
                if (r.text().trimmed().isEmpty())
                    break;
                qDebug() << tagName << r.text();
                if (tagName.at(0) == QChar('x')) {
                    pidx = tagName.at(1).digitValue();
                    bool ok = false;
                    qreal val = r.text().toDouble(&ok);
                    if (ok)
                        p.setX(val);
                } else if (tagName.at(0) == QChar('y')) {
                    pidx = tagName.at(1).digitValue();
                    bool ok = false;
                    qreal val = r.text().toDouble(&ok);
                    if (ok)
                        p.setY(val);
                } else if (tagName == "constrained") {
                    m_constrained = r.text().toInt();
                } else if (tagName == "name") {
                    m_name = r.text().toString();
                }
            } break;
            case QXmlStreamReader::EndElement:
//				qDebug() << "endElement" << r.name();
                if (r.name() == u"rectangle")
                    done = true;
                else if (r.name().length() > 1 && r.name().at(0) == 'y') {
                    poly[pidx] = p;
                    if (pidx == 0)
                        poly[4] = p;
//					qDebug("endElement yx, pidx %d, setting point %lf, %lf", pidx, p.x(), p.y());
                }
                break;
            default:
                break;
        }
        t = r.readNext();
    }
    qDebug("poly:");
    for (QPointF p : poly)
        qDebug("   %lf, %lf", p.x(), p.y());
    setPolygon(poly);
    setFlags(QGraphicsItem::ItemIsSelectable);
    setZValue(1.0);
    setPen({Qt::green, 0});
}

QRectF Rectangle::boundingRect() const
{
    QRectF ret = QGraphicsPolygonItem::boundingRect();
    ret.setRect(ret.x() - HANDLE_MARGIN - 1, ret.y() - HANDLE_MARGIN - 1, ret.width() + HANDLE_MARGIN + HANDLE_MARGIN + 1,
                ret.height() + HANDLE_MARGIN + HANDLE_MARGIN + 1);
    return ret.normalized();
    //	return QGraphicsRectItem::boundingRect().adjusted(HANDLE_MARGIN - 1, HANDLE_MARGIN - 1, -HANDLE_MARGIN, -HANDLE_MARGIN);
}

qreal Rectangle::rotation() const
{
    QPolygonF poly = polygon();
    qreal width = QLineF(poly[2], poly[3]).length();
    qreal ret = asin((poly[3].y() - poly[2].y()) / width);
    if (poly[3].y() < poly[0].y())
        ret = M_PI - ret;
    return ret * 180.0 / M_PI;
}

Rectangle::ResizeHandleIdx Rectangle::handleAt(QPointF pos)
{
    //	int hw = handleWidth();
    QPolygonF poly = polygon();
    //	QRectF bounds = QGraphicsPolygonItem::boundingRect();
    QRectF handleBounds(0, 0, HANDLE_VIRTUAL_SIZE, HANDLE_VIRTUAL_SIZE);
    handleBounds.moveCenter(mapToScene(poly[0]));
    if (handleBounds.contains(pos))
        return RH_TOP_LEFT;
    handleBounds.moveCenter(mapToScene(poly[1]));
    if (handleBounds.contains(pos))
        return RH_TOP_RIGHT;
    handleBounds.moveCenter(mapToScene(poly[2]));
    if (handleBounds.contains(pos))
        return RH_BOTTOM_RIGHT;
    handleBounds.moveCenter(mapToScene(poly[3]));
    qDebug("%lf, %lf to %lf, %lf contains %lf, %lf? %d", handleBounds.x(), handleBounds.y(), handleBounds.bottomRight().x(),
           handleBounds.bottomRight().y(), pos.x(), pos.y(), handleBounds.contains(pos));
    if (handleBounds.contains(pos))
        return RH_BOTTOM_LEFT;
    return RH_NONE;
}

void Rectangle::startResize()
{
    QPolygonF poly = polygon();
    actualWidth();
    actualHeight();
    sineA = (poly[3].y() - poly[2].y()) / width;
    qDebug("sineA is (%lf - %lf) / %lf = %lf", poly[2].y(), poly[3].y(), width, sineA);
    double angleA = asin(sineA);
    cosineA = cos(angleA);
    qDebug("angleA %lf, cosineA %lf", angleA * 180 / M_PI, cosineA);
    //	sineB = sin(M_PI_2 - angleA);
    aspect = width / height;
}

double Rectangle::actualWidth() const
{
    if (qFuzzyIsNull(width)) {
        QPolygonF poly = polygon();
        width = QLineF(poly[2], poly[3]).length();
    }
    return width;
}

double Rectangle::actualHeight() const
{
    if (qFuzzyIsNull(height)) {
        QPolygonF poly = polygon();
        height = QLineF(poly[0], poly[3]).length();
    }
    return height;
}

void Rectangle::resize(ResizeHandleIdx hidx, QPointF pos, bool constrained)
{
    m_constrained = constrained;
    QPolygonF poly = polygon();
//	QRectF bounds = QGraphicsPolygonItem::boundingRect();
//	double aspect = poly[2].x() - poly[3].x() / poly[0].y() - poly[3].y();
//	double width = QLineF(poly[2], poly[3]).length();
//	double height = QLineF(poly[0], poly[3]).length();
//	double aspect = width / height;
//	qDebug("aspect %lf", aspect);
    switch (hidx) {
        case RH_TOP_LEFT: {
            // Constrain to a constant angle.
            // sine and cosine were set in startResize() and now we hold them
            // constant while allowing height to vary.
            // Implementation: use y coordinate from mouse, and set the X coord
//			qDebug("x = %lf - %lf * (
//			pos.setX(poly[3].x() + sine * (poly[2].x() - poly[1].x()));
            if (constrained)
                pos.setX(poly[3].x() - sineA * (poly[3].y() - pos.y()));
            poly[0] = pos;
            poly[4] = pos;
            if (constrained)
                poly[1] = QPointF(pos.x() + poly[2].x() - poly[3].x(), pos.y() + poly[2].y() - poly[3].y());
        } break;
        case RH_TOP_RIGHT: {
            // Constrain to a constant angle.
            // sine and cosine were set in startResize() and now we hold them
            // constant while allowing height to vary.
            // Implementation: use y coordinate from mouse, and set the X coord
            if (constrained)
                pos.setX(poly[2].x() - sineA * (poly[2].y() - pos.y()));
            poly[1] = pos;
            if (constrained)
                poly[0] = poly[4] = QPointF(pos.x() - poly[2].x() + poly[3].x(), pos.y() - poly[2].y() + poly[3].y());
        } break;
        case RH_BOTTOM_RIGHT: {
            // Constrain to prevent rotation beyond +/- 90 degrees
            if (constrained && pos.x() < poly[3].x())
                pos.setX(poly[3].x());
            poly[2] = pos;
            if (!constrained)
                break;
            // Treat the bottom left as the fulcrum.
            actualWidth();
            height = width / aspect;
            double dy = poly[2].y() - poly[3].y();
//			qDebug("dy %lf, aspect %lf, -> dx %lf", dy, aspect, dy / aspect);
            double sine = dy / width;
            double cosine = cos(asin(sine));
            poly[0].rx() = poly[4].rx() = poly[3].x() + sine * height;
//			qDebug("sine %lf, cosine %lf, other dy: %lf * %lf", sine, cosine, cosine * height);
            // cosine = poly[0].y as if fulcrum is the origin / height
            poly[0].ry() = poly[4].ry() = poly[3].y() - cosine * height;
            poly[1].rx() = poly[2].x() + sine * height;
            poly[1].ry() = poly[2].y() - cosine * height;
        } break;
        case RH_BOTTOM_LEFT: {
            // Constrain to prevent rotation beyond +/- 90 degrees
            if (constrained && pos.x() > poly[2].x())
                pos.setX(poly[2].x());
            poly[3] = pos;
            if (!constrained)
                break;
            // Treat the bottom right as the fulcrum.
            actualWidth();
            height = width / aspect;
            double dy = poly[3].y() - poly[2].y();
//			qDebug("dy %lf, aspect %lf, -> dx %lf", dy, aspect, dy / aspect);
//			qDebug("other dy: %lf - sqrt(%lf ^2 - %lf ^2)", poly[3].y(), height, dy / aspect);
            double sine = dy / width;
            double cosine = cos(asin(sine));
//			poly[0].rx() = poly[4].rx() = poly[3].x() + dy / aspect;
            poly[0].rx() = poly[4].rx() = poly[3].x() - sine * height;
//			qDebug("sine %lf, cosine %lf, other dy: %lf * %lf", sine, cosine, cosine * height);
//			poly[0].ry() = poly[4].ry() = poly[3].y() - sqrt(height * height - (dy / aspect) * (dy / aspect));
            // cosine = poly[0].y as if fulcrum is the origin / height
            poly[0].ry() = poly[4].ry() = poly[3].y() - cosine * height;
            poly[1].rx() = poly[2].x() - sine * height;
            poly[1].ry() = poly[2].y() - cosine * height;
            break;
        }
        default:
            break;
    }
    prepareGeometryChange();
//	setRect(bounds);
    setPolygon(poly);
}

void Rectangle::rotate(bool clockwise)
{
    /* actual rotation:
    // rotate points in the polygon about the centroid
    QPolygonF poly = polygon();
    // So umm find the centroid first
    qreal cx = 0, cy = 0;
    for(int i = 0; i < 4; ++i)
    {
        cx += poly[i].x();
        cy += poly[i].y();
    }
    cx /= 4;
    cy /= 4;
    QTransform xf;
    // set transform to translate polygon to the origin
    xf.translate(cx, cy);
    // then rotate
    xf.rotate(clockwise ? -90 : 90);
    // then translate back
    xf.translate(-cx, -cy);
    setPolygon(xf.map(polygon()));
    */

    // Instead, it's more useful to just change which of the line segments
    // is the baseline.  That is, if clockwise then the segment which was
    // from points 1 to 2 will now be from 2 to 3.
    // Implementation: First, remove point 4.  Then if clockwise,
    // pop point 0 and append at the end; if counter-clockwise,
    // remove point 3 and prepend as point 0.  Finally, copy point 0
    // back to point 4.
    QPolygonF poly = polygon();
    poly.remove(4);
    if (clockwise) {
        QPointF p = poly[0];
        poly.remove(0);
        poly.append(p);
    } else {
        QPointF p = poly[3];
        poly.remove(3);
        poly.prepend(p);
    }
    poly.append(poly[0]);
    setPolygon(poly);
}

int Rectangle::handleWidth()
{
    if (!resizeHandle)
        resizeHandle = new QPixmap(":/images/resize-handle.png");
    return resizeHandle->width();
}

void Rectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//  qDebug("Rectangle::paint; selected? %d", isSelected());
    // painter->setCompositionMode(QPainter::CompositionMode_Xor);
    QGraphicsPolygonItem::paint(painter, option, widget);
    //	QRectF bounds = boundingRect();
    //	painter->drawLine(bounds.x(), bounds.y(), 0, 0);
    if (isSelected()) {
//		qDebug("Rectangle::paint, selected too");
        int halfHandleW = handleWidth() / 2;
        for (QPointF p : polygon()) {
            p.rx() -= halfHandleW;
            p.ry() -= halfHandleW;
            painter->drawPixmap(p, *resizeHandle);
        }
//		QRectF outerBounds = QGraphicsPolygonItem::boundingRect()
//				.adjusted(halfHandleW, halfHandleW, halfHandleW, halfHandleW);
////		QRectF outerBounds = bounds;
//		painter->drawPixmap(outerBounds.topLeft(), *resizeHandle);
//		painter->drawPixmap(outerBounds.topRight(), *resizeHandle);
//		painter->drawPixmap(outerBounds.bottomLeft(), *resizeHandle);
//		painter->drawPixmap(outerBounds.bottomRight(), *resizeHandle);
    }
    const QPolygonF poly = polygon();
    if (!m_name.isNull()) {
        // auto center = poly.boundingRect().center();
        // painter->drawText(center, m_name);
        auto availableWidth = poly.boundingRect().width();
        auto fm = painter->fontMetrics();
        auto w = fm.horizontalAdvance(m_name);
        if (w > availableWidth || w < availableWidth / 2) {
            QFont font = painter->font();
            qDebug() << "availableWidth" << availableWidth << "default font text width" << w << "so changing font size"
                     << font.pointSizeF() << "->" << font.pointSizeF() * availableWidth / w;
            font.setPointSizeF(font.pointSizeF() * availableWidth / w);
            painter->setFont(font);
        }
        painter->setPen(Qt::green);
        painter->drawText(poly.boundingRect(), m_name, {Qt::AlignHCenter | Qt::AlignVCenter});
    }
    // painter->setCompositionMode(QPainter::CompositionMode_Xor);
    painter->setPen({Qt::cyan, 0});
    painter->drawLine(poly[3], poly[2]);
}

void Rectangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(&m_setNameAction);
    menu.addAction(&m_rotateCWAction);
    menu.addAction(&m_rotateCCWAction);
    QAction *a = menu.exec(event->screenPos());
    if (a == &m_setNameAction) {
        QString s = QInputDialog::getText(nullptr, MainImageScene::tr("Name for cropped area"),
                                          MainImageScene::tr("Name for cropped area:"),
                                          QLineEdit::Normal, m_name);
        if (!s.isNull())
            setName(s);
    } else if (a == &m_rotateCWAction) {
        rotate(true);
    } else if (a == &m_rotateCCWAction) {
        rotate(false);
    }
}

void Rectangle::setName(const QString &name)
{
    if (m_name == name)
        return;
    m_name = name;
}

void Rectangle::writeXML(QXmlStreamWriter &w)
{
    QPolygonF poly = polygon();
    w.writeStartElement("rectangle");
    w.writeTextElement("x0", QString::number(poly[0].x()));
    w.writeTextElement("y0", QString::number(poly[0].y()));
    w.writeTextElement("x1", QString::number(poly[1].x()));
    w.writeTextElement("y1", QString::number(poly[1].y()));
    w.writeTextElement("x2", QString::number(poly[2].x()));
    w.writeTextElement("y2", QString::number(poly[2].y()));
    w.writeTextElement("x3", QString::number(poly[3].x()));
    w.writeTextElement("y3", QString::number(poly[3].y()));
    w.writeTextElement("angle", QString::number(rotation()));
    w.writeTextElement("constrained", QString::number(m_constrained));
    if (!m_name.isNull())
        w.writeTextElement("name", m_name);
    w.writeEndElement();
}

QDebug operator<<(QDebug debug, const Rectangle &r)
{
    const QPolygonF poly = r.polygon();
    debug << "Rectangle" << poly << ":" << r.actualWidth() << "x" << r.actualHeight() << "rot" << r.rotation();
    return debug;
}
