#include "screensaver.h"
#include "ui_screensaver.h"
#include "mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <iostream>
#include <QMessageBox>

Screensaver::Screensaver(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Screensaver)
{
  ui->setupUi(this);
  QTimer *timer = new QTimer();
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(10);
  view = ui->graphicsView;
  scene = new QGraphicsScene();
  view->setScene(scene);
  QPixmap pixmapItem(":/image/11.png");
  item = new QGraphicsPixmapItem();
  item->setPixmap(pixmapItem);
  scene->addItem(item);
  x0 = 0;
  y0 = 0;
  speedX = 2.5;
  speedY = 1.0;

}

Screensaver::~Screensaver()
{
  delete ui;
}

void Screensaver::on_closeButton_clicked()
{
  this->close();
  MainWindow *mainwindow = new MainWindow();
  mainwindow->show();
}


void Screensaver::update(){
  //  int posX = item->pos().x();
  item->setX(x0+speedX);
  item->setY(y0+speedY);
  y0+=speedY;
  x0+=speedX;
  item->setRotation(x0+y0);
  //  int posY = item->pos().y();
  //  item->setX(posY0);

  if (x0>470||x0<0){
      speedX*=-1;
    }
  if (y0>270||y0<0){
      speedY*=-1;
    }
  // std::cout << posX0 << std::endl;
}

//void Screensaver::on_startAnimation_clicked()
//{
//    view = ui->graphicsView;
//    scene = new QGraphicsScene();
//    view->setScene(scene);
//    QPixmap pixmapItem(":/image/11.png");
//    item = new QGraphicsPixmapItem();
//    item->setPixmap(pixmapItem);
//    scene->addItem(item);
//}


