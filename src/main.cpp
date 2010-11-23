#include "n57_app.h"
#include "n57_mainwindow.h"

int main(int argc, char *argv[])
{
    NaviS57 app(argc, argv);
    MainWindow * mw = new MainWindow(0);
    mw->show();
    int retval = app.exec();
    delete mw;
    return retval;
}