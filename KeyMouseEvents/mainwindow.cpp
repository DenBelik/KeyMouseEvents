#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->installEventFilter(this);
    ui->centralwidget->setMouseTracking(true);

    logo.load("img/C_Logo.png");
    logo = logo.scaled(imageWidth, imageHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {

    if (xCoords == 0) xCoords = ui->centralwidget->width()/2;
    if (yCoords == 0) yCoords = ui->centralwidget->height()/2;

    QPainter painter(this);
    painter.setViewport(ui->centralwidget->geometry().x(), ui->centralwidget->geometry().y(), ui->centralwidget->geometry().width(), ui->centralwidget->geometry().height());
    painter.translate(xCoords, yCoords);
    painter.rotate(rotateCoef);
    painter.scale(scaleCoef, scaleCoef);
    painter.drawImage(-imageWidth/2, -imageHeight/2, logo);

    QPainter painter2(this);
    QPen pen;
    QBrush brush;
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter2.setPen(pen);
    painter2.setBrush(brush);
    painter2.setViewport(ui->centralwidget->geometry().x(), ui->centralwidget->geometry().y(), ui->centralwidget->geometry().width(), ui->centralwidget->geometry().height());
    painter2.translate(xCoords, yCoords);
    painter2.setOpacity(imageBrightness/100);
    painter.drawRect(-imageWidth/2, -imageHeight/2, imageWidth, imageHeight);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        rotateCoef -= 90;
        break;
    case Qt::Key_Right:
        rotateCoef += 90;
        break;
    case Qt::Key_Up:
        imageBrightness -= 10;
        break;
    case Qt::Key_Down:
        imageBrightness += 10;;
        break;
    }
    qDebug() << imageBrightness;
    if (rotateCoef < 0) rotateCoef = 270;
    if (rotateCoef > 360) rotateCoef = 90;
    if (imageBrightness < 0) imageBrightness = 0;
    if (imageBrightness > 100) imageBrightness = 100;


    update();
}

bool MainWindow::eventFilter(QObject *tagret, QEvent *event)
{
    this->update();

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = (QMouseEvent *) event;
        if (mouseEvent->button() == Qt::LeftButton) {
            if (mouseEvent->x() > xCoords - imageWidth/2 && mouseEvent->x() < xCoords + imageWidth/2 && mouseEvent->y() > yCoords - imageHeight/2 && mouseEvent->y() < yCoords + imageHeight/2) {
                mousePressed = mouseEvent->pos();
            }
        }
    }

    if(event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = (QMouseEvent *) event;
        QKeyEvent *keyEvent = (QKeyEvent *) event;
        if (keyEvent->modifiers() == Qt::ALT && !mousePressed.isNull()) {
            xCoords = mouseEvent->x();
            yCoords = mouseEvent->y();
        }
    }

    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = (QMouseEvent *) event;
        if (mouseEvent->button() == Qt::LeftButton) {
            mousePressed = QPoint();
        }
    }

    if(event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = (QWheelEvent *) event;
        QKeyEvent *keyEvent = (QKeyEvent *) event;
        int encoder = wheelEvent->delta();
        if (encoder > 0 && keyEvent->modifiers()==Qt::CTRL) scaleCoef += 0.1;
        else if (encoder < 0 && keyEvent->modifiers()==Qt::CTRL) scaleCoef -= 0.1;
        if (encoder > 0 && keyEvent->modifiers()==Qt::NoModifier) yCoords -= 5;
        if (encoder < 0 && keyEvent->modifiers()==Qt::NoModifier) yCoords += 5;
        if (scaleCoef > 2.0) scaleCoef = 2.0;
        if (scaleCoef < 0.2) scaleCoef = 0.2;
    }
    return true;
}
