/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Wed Dec 9 21:53:42 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionZoom_to_Fit;
    QAction *actionZoom_Width;
    QAction *actionPan;
    QAction *actionSelect;
    QAction *actionBox;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionRotate_Clockwise;
    QAction *actionRotate_CounterClockwise;
    QAction *actionZoom_100;
    QAction *actionSave_template;
    QAction *actionOpen_template;
    QAction *actionSelect_All;
    QAction *actionQuit;
    QAction *actionZoom_25;
    QAction *actionPrevious;
    QAction *actionNext;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuTools;
    QMenu *menuEdit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QGraphicsView *topLeftView;
    QGraphicsView *topRightView;
    QGraphicsView *bottomLeftView;
    QGraphicsView *bottomRightView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(924, 860);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon1);
        actionZoom_to_Fit = new QAction(MainWindow);
        actionZoom_to_Fit->setObjectName(QString::fromUtf8("actionZoom_to_Fit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/zoom_fit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_to_Fit->setIcon(icon2);
        actionZoom_Width = new QAction(MainWindow);
        actionZoom_Width->setObjectName(QString::fromUtf8("actionZoom_Width"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/zoom_width.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_Width->setIcon(icon3);
        actionPan = new QAction(MainWindow);
        actionPan->setObjectName(QString::fromUtf8("actionPan"));
        actionPan->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/tool-pan.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPan->setIcon(icon4);
        actionSelect = new QAction(MainWindow);
        actionSelect->setObjectName(QString::fromUtf8("actionSelect"));
        actionSelect->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/cursor.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelect->setIcon(icon5);
        actionBox = new QAction(MainWindow);
        actionBox->setObjectName(QString::fromUtf8("actionBox"));
        actionBox->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/draw_box.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBox->setIcon(icon6);
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/zoom_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_In->setIcon(icon7);
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/zoom_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_Out->setIcon(icon8);
        actionRotate_Clockwise = new QAction(MainWindow);
        actionRotate_Clockwise->setObjectName(QString::fromUtf8("actionRotate_Clockwise"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/shape_rotate_clockwise.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRotate_Clockwise->setIcon(icon9);
        actionRotate_CounterClockwise = new QAction(MainWindow);
        actionRotate_CounterClockwise->setObjectName(QString::fromUtf8("actionRotate_CounterClockwise"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/shape_rotate_anticlockwise.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRotate_CounterClockwise->setIcon(icon10);
        actionZoom_100 = new QAction(MainWindow);
        actionZoom_100->setObjectName(QString::fromUtf8("actionZoom_100"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/images/zoom100.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_100->setIcon(icon11);
        actionSave_template = new QAction(MainWindow);
        actionSave_template->setObjectName(QString::fromUtf8("actionSave_template"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/images/save_template.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_template->setIcon(icon12);
        actionOpen_template = new QAction(MainWindow);
        actionOpen_template->setObjectName(QString::fromUtf8("actionOpen_template"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/images/open_template.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_template->setIcon(icon13);
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QString::fromUtf8("actionSelect_All"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionZoom_25 = new QAction(MainWindow);
        actionZoom_25->setObjectName(QString::fromUtf8("actionZoom_25"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/images/zoom25.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_25->setIcon(icon14);
        actionPrevious = new QAction(MainWindow);
        actionPrevious->setObjectName(QString::fromUtf8("actionPrevious"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/images/book_previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrevious->setIcon(icon15);
        actionNext = new QAction(MainWindow);
        actionNext->setObjectName(QString::fromUtf8("actionNext"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/images/book_next.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNext->setIcon(icon16);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setMargin(11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        horizontalLayout->addWidget(graphicsView);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 924, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setMaximumSize(QSize(16777215, 28));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        topLeftView = new QGraphicsView(dockWidgetContents);
        topLeftView->setObjectName(QString::fromUtf8("topLeftView"));
        topLeftView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        topLeftView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout->addWidget(topLeftView);

        topRightView = new QGraphicsView(dockWidgetContents);
        topRightView->setObjectName(QString::fromUtf8("topRightView"));
        topRightView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        topRightView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout->addWidget(topRightView);

        bottomLeftView = new QGraphicsView(dockWidgetContents);
        bottomLeftView->setObjectName(QString::fromUtf8("bottomLeftView"));
        bottomLeftView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        bottomLeftView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout->addWidget(bottomLeftView);

        bottomRightView = new QGraphicsView(dockWidgetContents);
        bottomRightView->setObjectName(QString::fromUtf8("bottomRightView"));
        bottomRightView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        bottomRightView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout->addWidget(bottomRightView);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionOpen_template);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuView->addAction(actionZoom_In);
        menuView->addAction(actionZoom_Out);
        menuView->addAction(actionZoom_100);
        menuView->addAction(actionZoom_25);
        menuView->addAction(actionZoom_to_Fit);
        menuView->addAction(actionZoom_Width);
        menuTools->addAction(actionSelect);
        menuTools->addAction(actionPan);
        menuTools->addAction(actionBox);
        menuEdit->addAction(actionSelect_All);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addAction(actionOpen_template);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionPrevious);
        mainToolBar->addAction(actionNext);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionZoom_In);
        mainToolBar->addAction(actionZoom_Out);
        mainToolBar->addAction(actionZoom_100);
        mainToolBar->addAction(actionZoom_25);
        mainToolBar->addAction(actionZoom_to_Fit);
        mainToolBar->addAction(actionZoom_Width);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionRotate_CounterClockwise);
        mainToolBar->addAction(actionRotate_Clockwise);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSelect);
        mainToolBar->addAction(actionPan);
        mainToolBar->addAction(actionBox);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "scancrop", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open image...", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "Save cropped image(s)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("MainWindow", "Save cropped image(s)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionZoom_to_Fit->setText(QApplication::translate("MainWindow", "Zoom to Fit", 0, QApplication::UnicodeUTF8));
        actionZoom_Width->setText(QApplication::translate("MainWindow", "Zoom Width", 0, QApplication::UnicodeUTF8));
        actionPan->setText(QApplication::translate("MainWindow", "Pan", 0, QApplication::UnicodeUTF8));
        actionPan->setShortcut(QApplication::translate("MainWindow", "F2", 0, QApplication::UnicodeUTF8));
        actionSelect->setText(QApplication::translate("MainWindow", "Select", 0, QApplication::UnicodeUTF8));
        actionSelect->setShortcut(QApplication::translate("MainWindow", "F1", 0, QApplication::UnicodeUTF8));
        actionBox->setText(QApplication::translate("MainWindow", "Box", 0, QApplication::UnicodeUTF8));
        actionBox->setShortcut(QApplication::translate("MainWindow", "F4", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setText(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setShortcut(QApplication::translate("MainWindow", "Ctrl+=", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0, QApplication::UnicodeUTF8));
        actionRotate_Clockwise->setText(QApplication::translate("MainWindow", "Rotate Clockwise", 0, QApplication::UnicodeUTF8));
        actionRotate_Clockwise->setShortcut(QApplication::translate("MainWindow", "Ctrl+.", 0, QApplication::UnicodeUTF8));
        actionRotate_CounterClockwise->setText(QApplication::translate("MainWindow", "Rotate CounterClockwise", 0, QApplication::UnicodeUTF8));
        actionRotate_CounterClockwise->setShortcut(QApplication::translate("MainWindow", "Ctrl+,", 0, QApplication::UnicodeUTF8));
        actionZoom_100->setText(QApplication::translate("MainWindow", "Zoom to 100%", 0, QApplication::UnicodeUTF8));
        actionZoom_100->setShortcut(QApplication::translate("MainWindow", "Ctrl+1", 0, QApplication::UnicodeUTF8));
        actionSave_template->setText(QApplication::translate("MainWindow", "Save template", 0, QApplication::UnicodeUTF8));
        actionSave_template->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        actionOpen_template->setText(QApplication::translate("MainWindow", "Open template...", 0, QApplication::UnicodeUTF8));
        actionOpen_template->setShortcut(QApplication::translate("MainWindow", "Ctrl+T", 0, QApplication::UnicodeUTF8));
        actionSelect_All->setText(QApplication::translate("MainWindow", "Select All", 0, QApplication::UnicodeUTF8));
        actionSelect_All->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionZoom_25->setText(QApplication::translate("MainWindow", "Zoom 25%", 0, QApplication::UnicodeUTF8));
        actionZoom_25->setShortcut(QApplication::translate("MainWindow", "Ctrl+2", 0, QApplication::UnicodeUTF8));
        actionPrevious->setText(QApplication::translate("MainWindow", "Previous Image", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPrevious->setToolTip(QApplication::translate("MainWindow", "Previous Image", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPrevious->setShortcut(QApplication::translate("MainWindow", "PgUp", 0, QApplication::UnicodeUTF8));
        actionNext->setText(QApplication::translate("MainWindow", "Next Image", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionNext->setToolTip(QApplication::translate("MainWindow", "Next Image", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionNext->setShortcut(QApplication::translate("MainWindow", "PgDown", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
