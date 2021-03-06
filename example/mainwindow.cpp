#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QDesktopWidget>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    w = new HandlerManager() ;

    w->getUpdate()->setFeedURL("http://kiwi.po/update.xml");
//    //    w->getUpdate()->setFeedURL("http://kineticwing.com/update.xml");

//    // Connect the "check for updates manually" button with the autoupdater
//    connect(ui->updateButton, SIGNAL(clicked()),
//            w->updateChecker(), SLOT(CheckForUpdatesNotSilent() ));


//    //Autocheck
    w->updateChecker()->CheckForUpdatesSilent();



    ui->label->setText( tr("Version %1").arg(QApplication::applicationVersion()) );


    //Move the dialog away from the center
    setGeometry(0,0,this->width(),this->height());
    int i = 0;
    if ( qApp->desktop()->screenCount() > 1 )
    {
        i = qApp->desktop()->screenNumber(this) ;
    }

    //Put the dialog in the screen center
    const QRect screen = qApp->desktop()->screenGeometry(i);
    move( screen.center() - rect().center() );

    setWindowFlags(Qt::WindowStaysOnTopHint);

    hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}
