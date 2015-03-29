#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <QKeyEvent>
#include <QDebug>

const int MainWindow::historyDiff = 1;
const double MainWindow::volDiff = 0.1;

MainWindow::MainWindow(std::shared_ptr<trikSound::TrikSoundController> controller, QWidget* parent):
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , mController(controller)
{
    ui->setupUi(this);

    ui->historyIndicator->display(controller->angleDetectionHistoryDepth());
    ui->volumeIndicator->display(controller->volume());
    ui->angleIndicator->display(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recieve(const trikSound::AudioEvent& event)
{
    qDebug() << "angle: " << event.angle();
//    ui->angleIndicator->display(event.angle());
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    auto controller = mController.lock();
    int historyDepth = controller->angleDetectionHistoryDepth();
    double vol = controller->volume();

    switch (event->key())
    {
    case Qt::Key_Left:
        historyDepth -= historyDiff;
        if (historyDepth > 0) {
            controller->setAngleDetectionHistoryDepth(historyDepth);
        }
        break;
    case Qt::Key_Right:
        historyDepth += historyDiff;
        if (historyDepth < 10) {
            controller->setAngleDetectionHistoryDepth(historyDepth);
        }
        break;
    case Qt::Key_Down:
        vol -= volDiff;
        if (vol >= 0) {
            controller->setVolume(vol);
        }
        break;
    case Qt::Key_Up:
        vol += volDiff;
        if (vol <= 1.0) {
            controller->setVolume(vol);
        }
        break;
    }

    if (historyDepth > 0 && historyDepth < 10) {
        ui->historyIndicator->display(historyDepth);
    }
    if (vol > 0 && vol < 1.0) {
        ui->volumeIndicator->display(vol);
    }
}