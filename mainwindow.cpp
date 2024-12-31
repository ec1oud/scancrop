// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwindow.h"
#include "common.h"
#include "imagescanner.h"
#include "settings.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QHash>
#include <QImageReader>
#include <QImageWriter>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QMimeData>
#include <QProcess>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

// 4th root of 2, so if you zoom 4 times you double the scale
#define ZOOM_SCALE 1.189207115002721027

MainWindow::MainWindow(QStringList mainArgs, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), args(mainArgs), argsIdx(0)
//	thumbnailCache(&doc)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&mainScene);
    ui->topLeftView->setScene(&mainScene);
    ui->topRightView->setScene(&mainScene);
    ui->bottomLeftView->setScene(&mainScene);
    ui->bottomRightView->setScene(&mainScene);
    ui->topLeftView->scale(2.0, 2.0);
    ui->topRightView->scale(2.0, 2.0);
    ui->bottomLeftView->scale(2.0, 2.0);
    ui->bottomRightView->scale(2.0, 2.0);
    connect(&mainScene, SIGNAL(cursorPos(QPointF)), this, SLOT(cursorMoved(QPointF)));
    connect(mainScene.selectTool, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    connect(&mainScene, &MainImageScene::imageOpened, this, &MainWindow::imageOpened);
    ui->actionSelect->setChecked(true);
    ui->actionKeep_rectangular->setChecked(true);
    if (args.count() > 0)
        open(args.first());
    updateNextPrevious();
    setAcceptDrops(true);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::open(QString fpath)
{
    qDebug() << QString("MainWindow::open(%1)").arg(fpath);
    QFileInfo fi(fpath);
    if (!fi.isReadable()) {
        /// @todo display warnings, critical etc. on error dialogs
        qWarning() << "unable to read" << fpath;
        return;
    }
    /// @todo use OS file type somehow rather than extension
    if (fi.suffix().compare("xml", Qt::CaseInsensitive) == 0)
        openTemplate(fi.absoluteFilePath());
    else
        openImage(fi.absoluteFilePath());
    setWindowTitle(QString("scancrop (%1)").arg(fi.absoluteFilePath()));
}

bool MainWindow::openImage(QString fpath)
{
    qDebug() << QString("MainWindow::openImage(%1)").arg(fpath);
    openedImage = QFileInfo(fpath);
    if (!openedImage.exists())
        return false;
    return mainScene.openImage(fpath);
}

void MainWindow::openTemplate(QString fpath)
{
    qDebug() << QString("MainWindow::openTemplate(%1)").arg(fpath);
    QFile in(fpath);
    if (!in.open(QIODevice::ReadOnly)) {
        qWarning() << "failed to open" << fpath;
        return;
    }
    openedTemplate = QFileInfo(fpath);
    QXmlStreamReader r(&in);
    bool originalTag = false;
    while (!r.atEnd()) {
        QXmlStreamReader::TokenType t = r.readNext();
        switch (t) {
            case QXmlStreamReader::StartElement:
                if (r.name() == u"rectangle") {
                    Rectangle *rect = new Rectangle(r);
                    mainScene.addItem(rect);
                }
                originalTag = (r.name() == u"original");
                break;
            case QXmlStreamReader::Characters:
                if (originalTag) {
                    originalTag = false;
                    QString imageName = r.text().toString();
                    if (!openImage(imageName)) // try CWD
                    {
                        // Try to find the image in the given path
                        qDebug() << "failed to open" << imageName << "in cwd, looking in" << openedTemplate.dir();
                        QFileInfo fi(openedTemplate.dir(), imageName);
                        if (!openImage(fi.absoluteFilePath())) {
                            bool success = false;
                            // Try to find the image in a subdir of the given path
                            QFileInfoList subs = openedTemplate.dir().entryInfoList(QDir::Dirs);
                            for (const QFileInfo &si : subs) {
                                qDebug() << "still didn't open" << imageName << ", looking in" << si.absoluteFilePath();
                                if (openImage(QFileInfo(QDir(si.absoluteFilePath()), imageName).absoluteFilePath())) {
                                    success = true;
                                    break;
                                }
                            }
                            if (!success)
                                qWarning() << "failed to open" << imageName << "; supported formats:" <<
                                    QImageReader::supportedImageFormats();
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
}

bool MainWindow::event(QEvent *ev)
{
    if (ev->type() == QEvent::Show && m_zoomFitPending) {
        on_actionZoom_to_Fit_triggered();
        m_zoomFitPending = false;
    }
    return QMainWindow::event(ev);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << event->mimeData()->formats() << event->mimeData()->urls();
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (urls.size() > 1)
        qWarning() << "this doesn't yet support more than one file at a time";
    if (urls.size() > 0 && urls.first().isLocalFile())
        openImage(urls.first().toLocalFile());
}

void MainWindow::cursorMoved(QPointF pos)
{
    QColor color = mainScene.colorAt((int)pos.x(), (int)pos.y());
    ui->statusBar->showMessage(QString("%1, %2 scale %3% color %4, %5, %6 brightness %7%")
                                   .arg(pos.x())
                                   .arg(pos.y())
                                   .arg(ui->graphicsView->transform().m11() * 100.0, 0, 'f', 0)
                                   .arg(color.red())
                                   .arg(color.blue())
                                   .arg(color.green())
                                   .arg(((float)color.toHsv().value()) / 255.0 * 100.0));
}

void MainWindow::selectionChanged()
{
    if (mainScene.selectedItems().count() > 0) {
        QGraphicsItem *i = mainScene.selectedItems()[0];
        if (i->type() == Rectangle::Type) {
            Rectangle *rect = (Rectangle *)i;
            QPolygonF poly = rect->mapToScene(rect->polygon());
            ui->topLeftView->centerOn(poly[0]);
            ui->topRightView->centerOn(poly[1]);
            ui->bottomRightView->centerOn(poly[2]);
            ui->bottomLeftView->centerOn(poly[3]);
        }
    }
}

void MainWindow::imageOpened(const QString &)
{
    on_actionZoom_to_Fit_triggered();
}

void MainWindow::on_actionOpen_triggered()
{
    const QString exts = tr("Images (*.") + QImageReader::supportedImageFormats().join(" *.") + ")";
    QString fpath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", exts);
    if (fpath.isEmpty())
        return;
    open(fpath);
//	thumbnailScene.layout();
}

void MainWindow::on_actionPan_toggled(bool checked)
{
    if (checked) {
        ui->actionBox->setChecked(false);
        ui->actionSelect->setChecked(false);
//		ui->graphicsView->viewport()->setCursor(Qt::OpenHandCursor);
        ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
        mainScene.setTool(nullptr);
        for (QGraphicsItem *i : mainScene.items())
            i->setFlags(static_cast<QGraphicsItem::GraphicsItemFlags>(0));
    }
}

void MainWindow::on_actionSelect_toggled(bool checked)
{
    if (checked) {
        ui->actionBox->setChecked(false);
        ui->actionPan->setChecked(false);
        ui->graphicsView->viewport()->setCursor(Qt::ArrowCursor);
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        mainScene.setTool(mainScene.selectTool);
        for (QGraphicsItem *i : mainScene.items())
            i->setFlags(QGraphicsItem::ItemIsSelectable);
    }
}

void MainWindow::on_actionBox_toggled(bool checked)
{
    if (checked) {
        ui->actionPan->setChecked(false);
        ui->actionSelect->setChecked(false);
        ui->graphicsView->viewport()->setCursor(Qt::CrossCursor);
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        mainScene.setTool(mainScene.boxTool);
        for (QGraphicsItem *i : mainScene.items())
            i->setFlags(static_cast<QGraphicsItem::GraphicsItemFlags>(0));
    }
}

void MainWindow::on_actionZoom_In_triggered() { ui->graphicsView->scale(ZOOM_SCALE, ZOOM_SCALE); }

void MainWindow::on_actionZoom_Out_triggered() { ui->graphicsView->scale(1 / ZOOM_SCALE, 1 / ZOOM_SCALE); }

void MainWindow::on_actionRotate_Clockwise_triggered()
{
    for (QGraphicsItem *i : mainScene.selectedItems()) {
        if (i->type() == Rectangle::Type) {
            Rectangle *rect = (Rectangle *)i;
            rect->rotate(true);
        }
    }
}

void MainWindow::on_actionRotate_CounterClockwise_triggered()
{
    for (QGraphicsItem *i : mainScene.selectedItems()) {
        if (i->type() == Rectangle::Type) {
            Rectangle *rect = (Rectangle *)i;
            rect->rotate(false);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    QImage whole = mainScene.image();
    int subpart = 0;
    for (QGraphicsItem *i : mainScene.items())
        if (i->type() == Rectangle::Type) {
            Rectangle *rect = (Rectangle *)i;
            QRect br = rect->mapRectToScene(rect->boundingRect()).toRect();
            qreal rot = rect->rotation();
            qDebug("bounding rect %d, %d, %d x %d, rotating %lf degrees",  br.x(), br.y(), br.width(), br.height(), rot);
            QImage bounded = whole.copy(br);
            QImage cropped;
            QPolygonF poly = rect->polygon();
            if (rect->isConstrained()) {
                // definitely just a rectangle
                QTransform rotation;
                rotation.rotate(rot);
                QImage rotated = bounded.transformed(rotation, Qt::SmoothTransformation);
//              QString filename = QString("/tmp/rotated_%1.jpg").arg(subpart);
//              rotated.save(filename);
                qDebug("rotating %lf degrees, cropping to %lf x %lf", rot, rect->actualWidth(), rect->actualHeight());
                cropped = rotated.copy((rotated.width() - rect->actualWidth()) / 2,
                                       (rotated.height() - rect->actualHeight()) / 2, rect->actualWidth(), rect->actualHeight());
                cropped.setText("description", QString("cropped area %1, %2; %3, %4; %5, %6; %7, %8 (rotation %9 degrees) from %10")
                                                   .arg(poly[0].x())
                                                   .arg(poly[0].y())
                                                   .arg(poly[1].x())
                                                   .arg(poly[1].y())
                                                   .arg(poly[2].x())
                                                   .arg(poly[2].y())
                                                   .arg(poly[3].x())
                                                   .arg(poly[3].y())
                                                   .arg(rot)
                                                   .arg(openedImage.fileName()));
                qDebug() << cropped.text();
            } else {
                // a general polygon that we need to map to a rectangle (inverse perspective transformation)
                auto brect = poly.boundingRect().toRect();
                auto topLen = QLineF(poly.first(), poly.at(1)).length();
                auto bottomLen = QLineF(poly.at(2), poly.at(3)).length();
                // assume that the bottom/top ratio is the same as the
                // foreshortening that projected the rectangle (or square) to a trapezoid
                brect.setHeight(brect.height() * bottomLen / topLen);
                // But if rotation was requested, swap width and height.
                if (qAbs(rot) > 45 && qAbs(rot) < 135) {
                    const auto h = brect.height();
                    brect.setHeight(brect.width());
                    brect.setWidth(h);
                }
                // make dimensions even numbers: HEIF is more compatible that way
                if (brect.width() % 2)
                    brect.setWidth(brect.width() + 1);
                if (brect.height() % 2)
                    brect.setHeight(brect.height() + 1);
                // depends on the fix for https://bugreports.qt.io/browse/QTBUG-21329
                QTransform transform;
                bool ok = QTransform::quadToQuad(poly, QRectF(brect), transform);
                if (ok) {
                    QTransform trueMatrix = QImage::trueMatrix(transform, whole.width(), whole.height());
                    brect.moveTopLeft(trueMatrix.map(poly.first()).toPoint());
                    qDebug() << "mapping" << poly << "to" << brect << "rot" << rot << "ok?" << ok << ":" << transform;
                    cropped = whole.transformed(transform, Qt::SmoothTransformation).copy(brect);
                } else {
                    qWarning() << "undefined transform from" << poly << "to" << brect;
                }
            }
            cropped.convertTo(QImage::Format_RGB888);
            QString format = Settings::instance()->stringOrDefault(SETTING_GROUP_MAIN, "format", "jpg");
            QString fname = QString("%1_%2.%3").arg(openedImage.completeBaseName()).arg(subpart++).arg(format);
            QImageWriter writer;
            writer.setFormat(format.toLocal8Bit());
            if (writer.supportsOption(QImageIOHandler::Description))
                qDebug() << format << "supports embedded text";
            else
                qDebug() << format << "does not support embedded text";
            if (!cropped.save(fname))
                qCritical() << "failed to save" << fname << "with size" << cropped.size();
            else
                qDebug() << "saved" << fname << "from" << cropped.format();
        } // Rectangle item in scene

    on_actionSave_template_triggered();
}

void MainWindow::on_actionZoom_100_triggered()
{
    ui->graphicsView->scale(1.0 / ui->graphicsView->transform().m11(), 1.0 / ui->graphicsView->transform().m11());
}

void MainWindow::on_actionSelect_All_triggered()
{
    for (QGraphicsItem *i : mainScene.items())
        if (i->type() == Rectangle::Type)
            i->setSelected(true);
}

void MainWindow::on_actionQuit_triggered()
{
    if (mainScene.isChanged() && mainScene.items().size() > 0) {
        if (QMessageBox::question(this, tr("Save changes?"), tr("Save XML template (crop boxes)?")) == QMessageBox::Yes)
            on_actionSave_template_triggered();
    }
    QApplication::instance()->quit();
}

void MainWindow::on_actionSave_template_triggered()
{
    QFile out(openedImage.absoluteFilePath() + ".xml");
    if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qCritical() << "failed to write" << out.fileName();
        return;
    }
    QXmlStreamWriter w(&out);
    w.setAutoFormatting(true);
    w.writeStartDocument();
    w.writeStartElement("croppings");
    w.writeTextElement("original", openedImage.fileName());
    for (QGraphicsItem *i : mainScene.items())
        if (i->type() == Rectangle::Type)
            ((Rectangle *)i)->writeXML(w);
    w.writeEndElement();
    w.writeEndDocument();
    qDebug() << "wrote" << out.fileName();
}

void MainWindow::on_actionOpen_template_triggered()
{
    QString fpath = QFileDialog::getOpenFileName(this, tr("Open Template"), "", tr("scancrop templates (*.xml)"));
    if (fpath.isEmpty())
        return;
    openTemplate(fpath);
}

void MainWindow::on_actionZoom_25_triggered()
{
    ui->graphicsView->scale(0.25 / ui->graphicsView->transform().m11(), 0.25 / ui->graphicsView->transform().m11());
}

void MainWindow::on_actionZoom_to_Fit_triggered()
{
    qreal scale = (ui->graphicsView->height() - 10) / mainScene.height();
    qreal scalew = (ui->graphicsView->width() - 10) / mainScene.width();
    if (scalew < scale)
        scale = scalew;
    ui->graphicsView->scale(scale / ui->graphicsView->transform().m11(), scale / ui->graphicsView->transform().m11());
}

void MainWindow::on_actionZoom_Width_triggered()
{
    qreal scale = (ui->graphicsView->width() - 10) / mainScene.width();
    ui->graphicsView->scale(scale / ui->graphicsView->transform().m11(), scale / ui->graphicsView->transform().m11());
}

void MainWindow::on_actionPrevious_triggered()
{
    if (argsIdx > 0)
        open(args[--argsIdx]);
    updateNextPrevious();
}

void MainWindow::on_actionNext_triggered()
{
    if (argsIdx < args.count() - 1)
        open(args[++argsIdx]);
    updateNextPrevious();
}

void MainWindow::updateNextPrevious()
{
    ui->actionNext->setEnabled(argsIdx < args.count() - 1);
    ui->actionPrevious->setEnabled(argsIdx > 0);
}

void MainWindow::on_actionFind_images_triggered()
{
    qDebug() << mainScene.detectPhotoBoundaries();

    // Another idea:
    // Circumnavigate the outer edges and find the color ranges for the background (we hope)
    // Generate a histogram of colors
    // (map of color to number of pixels in which it's found).

    /* TODO
    struct ranges
    QHash<QRgb, int> histo;
    int x = 0, y = 0, w = orig.width(), h = orig.height();
    for (; y < h; ++y)
        for (x = 0; x < w; ++x)
        {
            QRgb color = orig.pixel(x, y);
            histo[color] = histo[color] + 1;
        }
        */
}

void MainWindow::on_actionToggle_image_visible_triggered()
{
    auto &image = mainScene.imageView();
    image.setVisible(!image.isVisible());
}

void MainWindow::on_actionSettings_triggered() { m_prefsDialog.show(); }

void MainWindow::on_actionScan_triggered()
{
    if (!m_scannerConnection)
        m_scannerConnection = connect(ImageScanner::instance(), SIGNAL(done(QImage)), &mainScene, SLOT(image(QImage)));
    m_scanDialog.open();
}

void MainWindow::on_actionKeep_rectangular_toggled(bool k)
{
    mainScene.selectTool->setConstrainedResize(k);
}
