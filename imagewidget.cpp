#include "imagewidget.h"
#include <QPainter>
#include <QResizeEvent>

ImageWidget::ImageWidget(QWidget* parent) :
    QWidget(parent)
{
}

void ImageWidget::image(QImage pm)
{
	m_image = pm;
	update();
}

void ImageWidget::paintEvent ( QPaintEvent * event )
{
	QPainter painter(this);
	painter.drawImage(0, 0, m_image);
}

void ImageWidget::resizeEvent ( QResizeEvent * event )
{
	emit dimensions(event->size());
}

void ImageWidget::lookBusy(bool b)
{
	setCursor(b ? Qt::WaitCursor : Qt::ArrowCursor);
}
