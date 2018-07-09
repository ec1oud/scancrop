#include "mainimagescene.h"
#include "boxtool.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>

#define MOUSE_CLICK_MAX_MOVEMENT 3
#define MIN_CONTOUR_AREA 250

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

QVector<QRectF> MainImageScene::detectPhotoBoundaries()
{
    QVector<QRectF> ret;
    QImage rgb = mainImage.convertToFormat(QImage::Format_RGB32).rgbSwapped();
    cv::Mat mat(rgb.height(), rgb.width(), CV_8UC4, rgb.bits(), size_t(rgb.bytesPerLine()));

    using namespace cv;
    using namespace std;
    Mat src, cannyEdges;
    //~ imshow("source", mat);
    cvtColor(mat, src, CV_BGR2GRAY);
    Canny(src, cannyEdges, 50, 100, 3);

//mainImage = QImage(cannyEdges.data, cannyEdges.cols, cannyEdges.rows, int(cannyEdges.step), QImage::Format_Grayscale8);
//mainImageView.setPixmap(QPixmap::fromImage(mainImage));
//    imshow("canny output", cannyEdges);
//    std::vector<Vec4i> lines;
//    HoughLinesP(dst, lines, 1, CV_PI/180, 32, 32, 10 );
    vector<vector<Point> > contours;
    vector<vector<Point> > approxContours;
    findContours(cannyEdges, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    qDebug() << "found contours" << contours.size();

    vector<Point2f> outerPts(4);
    for (auto contour : contours) {
QVector<QPointF> qpts;
for(Point p : contour)
    qpts << QPointF(p.x, p.y);
QGraphicsPolygonItem *pgn = new QGraphicsPolygonItem(QPolygonF(qpts));
pgn->setPen(QPen(Qt::cyan));
addItem(pgn);
        vector<Point> approx;
        approxPolyDP(contour, approx, arcLength(contour, true) * 0.05, true);
        if (contourArea(approx, false) < MIN_CONTOUR_AREA)
            continue;
        auto rect = minAreaRect(contour);
        Point2f pts[4];
        rect.points(pts);

QVector<QPointF> bpts;
bpts << QPointF(pts[0].x, pts[0].y) << QPointF(pts[1].x, pts[1].y) << QPointF(pts[2].x, pts[2].y) << QPointF(pts[3].x, pts[3].y);
pgn = new QGraphicsPolygonItem(QPolygonF(bpts));
pgn->setPen(QPen(Qt::green));
addItem(pgn);

        outerPts[0].x = outerPts[0].x < pts[0].x ? outerPts[0].x : pts[0].x;
        outerPts[0].y = outerPts[0].y > pts[0].y ? outerPts[0].y : pts[0].y;
        outerPts[1].x = outerPts[1].x < pts[1].x ? outerPts[1].x : pts[1].x;
        outerPts[1].y = outerPts[1].y < pts[1].y ? outerPts[1].y : pts[1].y;
        outerPts[2].x = outerPts[2].x > pts[2].x ? outerPts[2].x : pts[2].x;
        outerPts[2].y = outerPts[2].y < pts[2].y ? outerPts[2].y : pts[2].y;
        outerPts[3].x = outerPts[3].x > pts[3].x ? outerPts[3].x : pts[3].x;
        outerPts[3].y = outerPts[3].y > pts[3].y ? outerPts[3].y : pts[3].y;
    }
    RotatedRect rect = minAreaRect(outerPts);
    QRectF r(0, 0, rect.size.width, rect.size.height);
    r.moveCenter(QPointF(rect.center.x, rect.center.y));
    ret << r;
    qDebug() << "image size" << rgb.width() << rgb.height() << "outer rect center" << rect.center.x << rect.center.y << "size" << rect.size.width << rect.size.height;

    Rectangle *box = new Rectangle(r.x(), r.y(), r.width(), r.height());
    addItem(box);
    box->setZValue(1.0);

    return ret;
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
