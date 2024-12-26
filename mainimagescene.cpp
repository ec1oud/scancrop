// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainimagescene.h"
#include "boxtool.h"
#include <QDebug>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define MOUSE_CLICK_MAX_MOVEMENT 3
#define MIN_CONTOUR_AREA 250

static const int CANNY_THRESHOLD = 50;
static const int SQUARES_THRESHOLD_COUNT = 11;

MainImageScene::MainImageScene() : QGraphicsScene(), boxTool(new BoxTool(this)), selectTool(new SelectTool(this)), tool(selectTool)
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

QColor MainImageScene::colorAt(int x, int y) { return mainImage.pixel(x, y); }

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double cos3pts(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

QVector<QRectF> MainImageScene::detectPhotoBoundaries()
{
    using namespace cv;
    QVector<QRectF> ret;

    QImage rgb = mainImage.convertToFormat(QImage::Format_RGB32).rgbSwapped();
    Mat src(rgb.height(), rgb.width(), CV_8UC4, rgb.bits(), size_t(rgb.bytesPerLine()));

    Mat pyr, timg, gray0(src.size(), CV_8U), gray;

    // down-scale and upscale the src to filter out the noise
    pyrDown(src, pyr, Size(src.cols / 2, src.rows / 2));
    pyrUp(pyr, timg, src.size());
    std::vector<std::vector<Point> > contours;

    // find squares in every color plane of the src
    for (int c = 0; c < 3; c++) {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for (int l = 0; l < SQUARES_THRESHOLD_COUNT; l++) {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if (l == 0) {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 0, CANNY_THRESHOLD, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1, -1));
            } else {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l + 1) * 255 / SQUARES_THRESHOLD_COUNT;
            }

            // find contours and store them all as a list
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

            std::vector<Point> approx;
            // test each contour
            for (size_t i = 0; i < contours.size(); i++) {
                // approximate contour with accuracy proportional to the contour perimeter
                approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);

                // square contours should have 4 vertices after approximation,
                // relatively large area (to filter out noisy contours), and be convex.
                // Note: absolute value of an area is used because area may be
                // positive or negative - in accordance with the contour orientation
                if (approx.size() == 4 && fabs(contourArea(approx)) > 1000 && isContourConvex(approx)) {
                    double maxCosine = 0;

                    for (int j = 2; j < 5; j++) {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(cos3pts(approx[j % 4], approx[j - 2], approx[j - 1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small (all angles are ~90°)
                    // then write quandrange vertices to resultant sequence
                    if (maxCosine < 0.3) {
                        QVector<QPointF> bpts;
                        for (Point p : approx)
                            bpts << QPointF(p.x, p.y);

                        QGraphicsPolygonItem *pgn = new QGraphicsPolygonItem(QPolygonF(bpts));
                        pgn->setPen(QPen(Qt::green));
                        addItem(pgn);

                        Rectangle *box = new Rectangle(approx);
                        qDebug() << approx[0].x << approx[0].y << approx[1].x << approx[1].y << approx[2].x << approx[2].y
                                 << approx[3].x << approx[3].y << "->" << *box;
                        box->setPen(QPen(Qt::magenta));
                        addItem(box);
                        box->setZValue(1.0);

                        ret << box->boundingRect();
                    }
                }
            }
        }
    }

    return ret;
}

void MainImageScene::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    if (tool)
        tool->mouseMoveEvent(ev);
    else
        QGraphicsScene::mouseMoveEvent(ev);
    emit cursorPos(ev->scenePos());
}

void MainImageScene::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    if (tool)
        tool->mousePressEvent(ev);
    else
        QGraphicsScene::mousePressEvent(ev);
}

void MainImageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if (tool) {
        if (abs(ev->buttonDownScreenPos(Qt::LeftButton).x() - ev->screenPos().x()) < MOUSE_CLICK_MAX_MOVEMENT &&
            abs(ev->buttonDownScreenPos(Qt::LeftButton).y() - ev->screenPos().y()) < MOUSE_CLICK_MAX_MOVEMENT)
            tool->mouseLeftClickEvent(ev);
        tool->mouseReleaseEvent(ev);
        update();
    } else
        QGraphicsScene::mouseReleaseEvent(ev);
}

void MainImageScene::keyPressEvent(QKeyEvent *ev)
{
    switch (ev->key()) {
        case Qt::Key_Delete:
            for (QGraphicsItem *i : selectedItems()) {
                removeItem(i);
                delete i;
            }
            break;
        default:
            QGraphicsScene::keyPressEvent(ev);
            break;
    }
}

QImage MainImageScene::image() { return mainImageView.pixmap().toImage(); }

bool MainImageScene::openImage(QString path)
{
    QPixmap pm(path);
    if (pm.isNull())
        return false;
    mainImage = pm.toImage();
    mainImageView.setPixmap(pm);
    mainImageView.setFlag(QGraphicsItem::ItemIsSelectable, false);
    for (QGraphicsItem *i : items())
        if (i->type() == Rectangle::Type) {
            removeItem(i);
            delete i;
        }
    emit imageOpened(path);
    return true;
}
