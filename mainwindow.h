// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainimagescene.h"
#include "preferencesdialog.h"
#include "scandialog.h"

#include <QFileInfo>
#include <QLineEdit>
#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QStringList mainArgs, QWidget *parent = 0);
    ~MainWindow();
    void open(QString path);
    bool openImage(QString path);
    void openTemplate(QString path);

protected:
    bool event(QEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void on_actionFind_images_triggered();
    void on_actionNext_triggered();
    void on_actionPrevious_triggered();
    void on_actionZoom_Width_triggered();
    void on_actionZoom_to_Fit_triggered();
    void cursorMoved(QPointF pos);
    void updateNextPrevious();
    void selectionChanged();
    void imageOpened(const QString &path);
    void on_actionZoom_25_triggered();
    void on_actionOpen_template_triggered();
    void on_actionSave_template_triggered();
    void on_actionQuit_triggered();
    void on_actionSelect_All_triggered();
    void on_actionZoom_100_triggered();
    void on_actionSave_triggered();
    void on_actionRotate_CounterClockwise_triggered();
    void on_actionRotate_Clockwise_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionZoom_In_triggered();
    void on_actionBox_toggled(bool);
    void on_actionSelect_toggled(bool);
    void on_actionPan_toggled(bool);
    void on_actionOpen_triggered();
    void on_actionToggle_image_visible_triggered();
    void on_actionSettings_triggered();
    void on_actionScan_triggered();
    void on_actionKeep_rectangular_toggled(bool k);

private:
    Ui::MainWindow *ui;
    MainImageScene mainScene;
    QFileInfo openedImage;
    QFileInfo openedTemplate;
    QStringList args;
    int argsIdx;
    bool m_zoomFitPending = true;
    PreferencesDialog m_prefsDialog;
    ScanDialog m_scanDialog;
    QMetaObject::Connection m_scannerConnection;
};

#endif // MAINWINDOW_H
