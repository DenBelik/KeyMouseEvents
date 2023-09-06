#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QImage logo;
    int imageHeight = 200;
    int imageWidth = 200;
    double imageBrightness = 0;
    double scaleCoef = 1;
    int xCoords = 0;
    int yCoords = 0;
    int rotateCoef = 0;
    QPoint mousePressed;

    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *tagret, QEvent *event);

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *event);
};
#endif // MAINWINDOW_H
