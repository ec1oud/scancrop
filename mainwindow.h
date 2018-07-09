#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainimagescene.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QFileInfo>

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

private slots:
	void on_actionFind_images_triggered();
 void on_actionNext_triggered();
 void on_actionPrevious_triggered();
 void on_actionZoom_Width_triggered();
 void on_actionZoom_to_Fit_triggered();
 void cursorMoved(QPointF pos);
 void updateNextPrevious();
	void selectionChanged();
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
	void on_actionBox_toggled(bool );
	void on_actionSelect_toggled(bool );
	void on_actionPan_toggled(bool );
	void on_actionOpen_triggered();

private:
	Ui::MainWindow *ui;
    MainImageScene mainScene;
	QFileInfo openedImage;
	QFileInfo openedTemplate;
	QStringList args;
	int argsIdx;
};

#endif // MAINWINDOW_H
