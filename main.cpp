#include "mainwindow.h"
#include <QApplication>
#include <fstream>


int main(int argc, char *argv[])
{
  std::ofstream resolutoinFile("resolutoin.txt");
  resolutoinFile << "1";
  resolutoinFile.close();

  std::ofstream soundFile("sound.txt");
  soundFile << "1";
  soundFile.close();

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
