#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_startButton_clicked();

  void on_exitButton_clicked();

  void on_optionsButton_clicked();

  void on_creditsButton_clicked();

  void on_screenButton_clicked();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
