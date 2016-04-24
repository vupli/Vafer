#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "credits.h"
#include "screensaver.h"

#include "source/Game.hpp"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_startButton_clicked()
{
  this->close();
  RunGame();
}

void MainWindow::on_exitButton_clicked()
{
    exit(0);
}

void MainWindow::on_optionsButton_clicked()
{
  this->close();
  Options *options = new Options();
  options->show();
}

void MainWindow::on_creditsButton_clicked()
{
  this->close();
  Credits *credits = new Credits();
  credits->show();
}

void MainWindow::on_screenButton_clicked()
{
    Screensaver *screen = new Screensaver();
    this->close();
    screen->show();
}
