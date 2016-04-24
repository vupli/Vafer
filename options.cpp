#include "options.h"
#include "ui_options.h"
#include "mainwindow.h"
#include <fstream>

Options::Options(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Options)
{
  ui->setupUi(this);

  ui->comboBox->addItem("1280x720");
  ui->comboBox->addItem("800x600");
  ui->checkBox->setChecked(true);
}

Options::~Options()
{
  delete ui;
}

void Options::on_pushButton_clicked()
{
  std::ofstream resolutoinFile("resolutoin.txt", std::ios_base::out);
  if((ui->comboBox->currentIndex())-1){
      resolutoinFile << 0;
    }
  else {
      resolutoinFile << 1;
    }

  std::ofstream soundFile("sound.txt", std::ios_base::out);

  if(ui->checkBox->isChecked()){
      soundFile << 1;
    }
  else{
      soundFile << 0;
    }

  this->close();
  MainWindow *mainwindow = new MainWindow();
  mainwindow->show();
}

