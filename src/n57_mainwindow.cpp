#include <QtCore/QAbstractItemModel>
#include <QtCore/QSettings>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QFileDialog> 
#include <QtGui/QMessageBox> 

#include <QtGui/QGraphicsPolygonItem>

#include "n57_mainwindow.h"
#include "n57_globals.h"
#include "n57_texts.incl"

#include "cell_s57_base.h"
#include "cell_s57_iterators.h"
#include "boundingbox_degrees.h"
#include "geo_projections.h"
#include <naviWidget.h>

using namespace Enc;
using namespace Navi57;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(QString("%1 %2").arg(APP_DIALOG_NAME).arg(APP_VERSION));

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

    //****** Central widget and Layout ******
    QWidget * centralWgt = new QWidget(this); 
    setCentralWidget(centralWgt);
    QVBoxLayout * mainLyt = new QVBoxLayout(centralWgt);
 
//##########################################################################
    naviWgt = new NaviWidget(centralWgt);
    mainLyt->addWidget(naviWgt);
    
//##########################################################################



    //**** connections ****
    connect(cellOpenAct, SIGNAL(triggered()), this, SLOT(onOpenS57Cell()));
    connect(cellZoomInAct,  SIGNAL(triggered()), naviWgt, SLOT(zoomIn()));
    connect(cellZoomOutAct, SIGNAL(triggered()), naviWgt, SLOT(zoomOut()));

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
void MainWindow::onAboutQt()
{
    QMessageBox::aboutQt(this, APP_DIALOG_NAME);
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, APP_DIALOG_NAME, aboutText);
}

//*****************************************************************************
/// 
/*!
****************************************************************************** */
void MainWindow::onOpenS57Cell()
{
    QStringList cellList = QFileDialog::getOpenFileNames(this, tr("Selecte one or more S-57 Cells"), QString(), tr("S-57 Cells (*.000);; All Files (*.*)"));
    if (cellList.isEmpty()) return;
    
    naviWgt->loadCharts(cellList);

}






