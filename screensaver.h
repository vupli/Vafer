#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

namespace Ui {
class Screensaver;
}

class Screensaver : public QWidget
{
    Q_OBJECT

public:
    explicit Screensaver(QWidget *parent = 0);
    ~Screensaver();

private slots:
    void on_closeButton_clicked();
    void update();

private:
    Ui::Screensaver *ui;
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;
    int x0, y0, speedX, speedY;
};

#endif // SCREENSAVER_H
