#include <QtCore/QAbstractItemModel>
#include <QtCore/QSettings>
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QDesktopWidget>
#include <QtGui/QDockWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QFileDialog> 
#include <QtGui/QMessageBox> 
#include <QtGui/QStatusBar>

#include <QtGui/QGraphicsPolygonItem>

#include "n57_mainwindow.h"
#include "n57_globals.h"
#include "n57_texts.incl"

#include "cell_s57_base.h"
#include "cell_s57_iterators.h"
#include "boundingbox_degrees.h"
#include "geo_projections.h"
#include <naviWidget.h>
#include <naviNaviWidgets.h>

using namespace Enc;
using namespace Navi57;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(QString("%1 %2").arg(APP_DIALOG_NAME).arg(APP_VERSION));

    //****** Central widget and Layout ******
    QWidget * centralWgt = new QWidget(this); 
    setCentralWidget(centralWgt);
    QVBoxLayout * mainLyt = new QVBoxLayout(centralWgt);
 
    //**** most important Members: scene and view ****
    naviWgt = new NaviWidget(centralWgt);
    mainLyt->addWidget(naviWgt);
    
    createMenues();
    createNaviDocWgts();

    //**** inits ****
    //** set old size - rem: maybe screen has changed **
    QSettings mySettings(ORG_SETTING_NAME, APP_SETTING_NAME);
    bool wasFullScreen = mySettings.value("mainwindow/fullScreen").toBool();
    if (wasFullScreen) showMaximized();
    else
    {
        QSize oldWinSize = mySettings.value("mainwindow/size").toSize();
        const QRect screenRect = QApplication::desktop()->availableGeometry(-1);
        if (oldWinSize.width() <= screenRect.size().width() && oldWinSize.height() <= screenRect.size().height()) {
            resize(oldWinSize);
        }
    }
}

MainWindow::~MainWindow()
{
    //**** save Settings ****
    QSettings mySettings(ORG_SETTING_NAME, APP_SETTING_NAME);
    mySettings.setValue("mainwindow/size", size());
    mySettings.setValue("mainwindow/fullScreen", isMaximized());
}

//*****************************************************************************
/// Create the Menues and coressponding Actions, and connect them
/*!
****************************************************************************** */
void MainWindow::createMenues()
{
    //**** The Menue actions ****
    QAction * cellOpenAct = new QAction(tr("&Open S-57 Cells..."), this);

    QAction * cellZoomInAct = new QAction(tr("Zoom in..."), this);
    QAction * cellZoomOutAct = new QAction(tr("zoom out..."), this);

    //**** The Menues ****
    QMenu * fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(cellOpenAct);

    QMenu * viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(cellZoomInAct);
    viewMenu->addAction(cellZoomOutAct);

    QMenu * optMenu = menuBar()->addMenu(tr("&Options"));

    QMenu * hlpMenu = menuBar()->addMenu(tr("&Help"));
    QAction * aboutAct = hlpMenu->addAction(QString("About ") +APP_DIALOG_NAME, this, SLOT(onAbout()));
    QAction * aboutQtAct = hlpMenu->addAction("About Qt", this, SLOT(onAboutQt()));

     //**** connections ****
    connect(cellOpenAct, SIGNAL(triggered()), this, SLOT(onOpenS57Cell()));
    connect(cellZoomInAct,  SIGNAL(triggered()), naviWgt, SLOT(zoomIn()));
    connect(cellZoomOutAct, SIGNAL(triggered()), naviWgt, SLOT(zoomOut()));
    connect(naviWgt, SIGNAL(progressMessage(const QString &)), statusBar(),SLOT(showMessage(const QString &)));
}

//*****************************************************************************
/// Create the Dock-widgets and coressponding Actions, and connect them
/*!
****************************************************************************** */
void MainWindow::createNaviDocWgts()
{
    //**** first, create the widgets itself ****
    projectWgt = new ChartProjectionComboBox();
    scaleWgt = new ChartScaleWidget();
    rotWgt = new ChartRotationWidget();

    posWgt = new ChartPositionWidget();
    xyWgt = new ChartEastNorthWidget();

    //** inits **
    for (int prI=0; prI < ProjectionCount; ++prI) projectWgt->addItem(Projections[prI]); // initProjections

    //** Widget Signals are just forwarded with same-name Scene Signals **
    connect(projectWgt, SIGNAL(currentIndexChanged(int)), naviWgt, SLOT(onProjectionChanged(int)));
    connect(scaleWgt,  SIGNAL(zoomIn()), naviWgt, SLOT(zoomIn()));
    connect(scaleWgt, SIGNAL(zoomOut()), naviWgt, SLOT(zoomOut()));
    connect(rotWgt, SIGNAL(chartHeading(double)), naviWgt, SLOT(setChartHeading(double)));

    connect(naviWgt, SIGNAL(scaleChanged(double)), scaleWgt, SLOT(setScale(double)));
    connect(naviWgt, SIGNAL(headingChanged(double)), rotWgt, SLOT(setHeading(double)));

    //**** turn the navi-widgets into dok-widgets ****
    setDockOptions(QMainWindow::AnimatedDocks);
    QDockWidget::DockWidgetFeatures features = QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable;

    QDockWidget *projectDockWidget = new QDockWidget(tr("Projection"), this);
    projectDockWidget->setFeatures(features);
    projectDockWidget->setWidget(projectWgt);
    //projectDockWidget->setFixedHeight(projectDockWidget->sizeHint().height()); //bad idea, child widget should have fixed hight
    addDockWidget(Qt::RightDockWidgetArea, projectDockWidget);

    QDockWidget * scaleDockWidget = new QDockWidget(tr("Scale"), this);
    scaleDockWidget->setFeatures(features);
    scaleDockWidget->setWidget(scaleWgt);
    addDockWidget(Qt::RightDockWidgetArea, scaleDockWidget);

    QDockWidget *rotateDockWidget = new QDockWidget(tr("Heading"), this);
    rotateDockWidget->setFeatures(features);
    rotateDockWidget->setWidget(rotWgt);
    addDockWidget(Qt::RightDockWidgetArea, rotateDockWidget);

    QDockWidget *posDockWidget = new QDockWidget(tr("Naut. Position"), this);
    posDockWidget->setFeatures(features);
    posDockWidget->setWidget(posWgt);
    addDockWidget(Qt::RightDockWidgetArea, posDockWidget);

    QDockWidget *xyDockWidget = new QDockWidget(tr("x- y- Position"), this);
    xyDockWidget->setFeatures(features);
    xyDockWidget->setWidget(xyWgt);
    addDockWidget(Qt::RightDockWidgetArea, xyDockWidget);
}

//*****************************************************************************
/// As usual: About ... slots
/*!
****************************************************************************** */
void MainWindow::onAboutQt()
{
    QMessageBox::aboutQt(this, APP_DIALOG_NAME);
}

void MainWindow::onAbout()
{
    QString aboutText = QString(Navi57::APP_DIALOG_NAME) + "\nVersion: " +QString(Navi57::APP_VERSION) +"\n\n";

    QFile aboutFile(":/resources/about.txt");
    if (aboutFile.open(QIODevice::ReadOnly))
    {
        aboutText += aboutFile.readAll(); //it returns a QByteArray, but QString has a constructor for it :-)
        aboutFile.close();
    }
    QMessageBox::about(this, APP_DIALOG_NAME, aboutText);
}

//*****************************************************************************
/// Open and display S-57 cells
/*!
****************************************************************************** */
void MainWindow::onOpenS57Cell()
{

    QStringList cellList = QFileDialog::getOpenFileNames(this, tr("Selecte one or more S-57 Cells"), QString(), tr("S-57 Cells (*.000);; All Files (*.*)"));
    if (cellList.isEmpty()) return;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    try
    {
        naviWgt->loadCharts(cellList);
    }
    catch(const QString & msg)
    {
        QMessageBox::warning(this, "Error", msg);
    }
    catch(...)
    {
        QMessageBox::warning(this, "Error", "Unknown exception while loading charts");
    }
    QApplication::restoreOverrideCursor();
}






