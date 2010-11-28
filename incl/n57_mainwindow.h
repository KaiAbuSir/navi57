#ifndef N57_MAINWINDOW_H
#define N57_MAINWINDOW_H

#include <QtCore/QString>
#include <QtCore/QAbstractItemModel>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QMainWindow>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>

#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QPen>
#include <QtGui/QBrush>

#include <vector>

namespace Enc
{
    class NaviWidget;
    class ChartProjectionComboBox;
    class ChartScaleWidget;
    class ChartPositionWidget;
    class ChartEastNorthWidget;
    class ChartRotationWidget;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void createMenues();
    void createNaviDocWgts();

    void onAboutQt();
    void onAbout();
   
    void onOpenS57Cell();
   
private:

    Enc::NaviWidget * naviWgt;

    Enc::ChartProjectionComboBox * projectWgt;
    Enc::ChartScaleWidget * scaleWgt;
    Enc::ChartPositionWidget * posWgt;
    Enc::ChartEastNorthWidget * xyWgt;
    Enc::ChartRotationWidget * rotWgt;

};


#endif // MAINWINDOW_H
