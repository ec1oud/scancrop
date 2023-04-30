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
    using namespace cv;
    QVector<QRectF> ret;

    QImage rgb = mainImage.convertToFormat(QImage::Format_RGB32).rgbSwapped();
    Mat src(rgb.height(), rgb.width(), CV_8UC4, rgb.bits(), size_t(rgb.bytesPerLine()));
    Mat pre(rgb.height(), rgb.width(), CV_8UC4, rgb.bits(), size_t(rgb.bytesPerLine()));
    Mat element = getStructuringElement(MORPH_RECT, Size(150, 150));
    morphologyEx(src, pre, MORPH_CLOSE, element, Point(-1,-1), 3);

    qDebug() << pre.cols << pre.rows << pre.step;
    image(QImage((const uchar*)(pre.data), pre.cols, pre.rows, pre.step, QImage::Format_RGB32).rgbSwapped());

    using namespace std;
    Mat cannyEdges;
    cvtColor(pre, src, COLOR_BGR2GRAY);
    Canny(src, cannyEdges, 50, 100, 3);

    std::vector<Vec4i> lines;
    HoughLinesP(cannyEdges, lines, 1, CV_PI/180, 32, 32, 10 );
    vector<Point> pointset;
    for( size_t i = 0; i < lines.size(); i++ ) {
        QGraphicsLineItem *l = new QGraphicsLineItem(lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
        l->setPen(QPen(Qt::red));
        addItem(l);
        pointset.push_back(Point(lines[i][0], lines[i][1]));
        pointset.push_back(Point(lines[i][2], lines[i][3]));
    }

    vector<Point> convex_hull;
    convexHull(pointset, convex_hull);
    qDebug() << "convex hull has" << convex_hull.size();

    QVector<QPointF> bpts;
    for (Point p : convex_hull)
        bpts << QPointF(p.x, p.y);

    QGraphicsPolygonItem *pgn = new QGraphicsPolygonItem(QPolygonF(bpts));
    pgn->setPen(QPen(Qt::green));
    addItem(pgn);

    RotatedRect rect = minAreaRect(convex_hull);
    QRectF r(0, 0, rect.size.width, rect.size.height);
    r.moveCenter(QPointF(rect.center.x, rect.center.y));
    ret << r;

    Rectangle *box = new Rectangle(r.x(), r.y(), r.width(), r.height());
    box->setPen(QPen(Qt::magenta));
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
