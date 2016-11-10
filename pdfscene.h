#ifndef PDFSCENE_H
#define PDFSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QImage>
#include "tool.h"
#include "boxtool.h"
#include "selecttool.h"

class PDFScene : public QGraphicsScene
{
	Q_OBJECT
public:
	PDFScene();
	BoxTool* boxTool;
	SelectTool* selectTool;
	QColor colorAt(int x, int y);

signals:
	void cursorPos(QPointF pos);

public slots:
	void image(QImage pm);
	void image(int /* pnum */, QImage pm) { image(pm); }
	QImage image();
	void setTool(Tool* t) { tool = t; }
	bool openImage(QString path);

protected:
	void mouseMoveEvent ( QGraphicsSceneMouseEvent * ev);
	void mousePressEvent ( QGraphicsSceneMouseEvent * ev);
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * ev);
	void keyPressEvent ( QKeyEvent * ev ) ;

private:
	QImage mainImage;
	QGraphicsPixmapItem pdfView;
	Tool* tool;

};

#endif // PDFSCENE_H
