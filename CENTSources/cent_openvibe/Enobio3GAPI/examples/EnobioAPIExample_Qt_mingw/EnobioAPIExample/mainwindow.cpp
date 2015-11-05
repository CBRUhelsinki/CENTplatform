#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDate>
#include <QDebug>

#include "channeldata.h"
#include "StatusData.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // The consumers signals every time they receive new data and status so
    // their signals are directly connected to the TextEdit in the GUI
    QObject::connect(&_enobioDataConsumer, SIGNAL(newData(QString)),
                    ui->dataTextEdit, SLOT(append(QString)));
    QObject::connect(&_enobioStatusConsumer, SIGNAL(newStatus(QString)),
                    ui->statusTextEdit, SLOT(append(QString)));
    // The consumers are registereds into the Enobio processor in order to
    // receive both data and status
    _enobio.registerConsumer(Enobio3G::ENOBIO_DATA, _enobioDataConsumer);
    _enobio.registerConsumer(Enobio3G::STATUS, _enobioStatusConsumer);

    //_enobio.setDemoMode(true, "demosignals.txt");


}

MainWindow::~MainWindow()
{
    delete ui;
}


//
// Implementation of the receiveData for both Data and Status consumers
// The execution of these methods happens in a thread created by the Enobio
// instance so accesing GUI resources might lead to a program crash
//
void EnobioDataConsumer::receiveData(const PData &data)
{
    // The EnobioData is destroyed after the execution of receiveData by
    // the caller
    ChannelData * pData = (ChannelData *)data.getData();
    // Provide the data to whatever slot is connected to that signal
    emit newData(QString::number(pData->data()[0]) + "\t" +
                 QString::number(pData->data()[1]) + "\t" +
                 QString::number(pData->data()[2]) + "\t" +
                 QString::number(pData->data()[3]) + "\t" +
                 QString::number(pData->data()[4]) + "\t" +
                 QString::number(pData->data()[5]) + "\t" +
                 QString::number(pData->data()[6]) + "\t" +
                 QString::number(pData->data()[7]) + "\t" +
                 QString::number(pData->timestamp()));
}

void EnobioStatusConsumer::receiveData(const PData &data)
{
    StatusData * pStatus = (StatusData *)data.getData();
    // Provide the data to whatever slot is connected to that signal
    emit newStatus(QDate::currentDate().toString("dd/MM/yyyy") + " " +
                   QTime::currentTime().toString("hh:mm:ss") + " " +
                   QString::fromStdString(StatusData::getTypeFromCode(pStatus->getCode())) +
                   ": " + QString::fromStdString(pStatus->getString()));
}

void MainWindow::on_openCloseEnobioButton_toggled(bool checked)
{
    if (checked)
    {
        //unsigned char mac[6] = {0xea, 0xfb, 0x4b, 0x80, 0x07, 0x00};
        unsigned char mac[6] = {0xc6, 0x2b, 0x4b, 0x80, 0x07, 0x00};
        if(_enobio.openDevice(mac))
        {
            ui->openCloseEnobioButton->setText("Close Enobio");
        }
        else
        {
            QMessageBox::information(this, "EnobioAPI demo",
                                     "Error opening Enobio. Check it is "
                                     "plugged to the computer");
        }
    }
    else
    {
        ui->openCloseEnobioButton->setText("Open Enobio");
        _enobio.closeDevice();
    }
}

void MainWindow::on_startStopEEGButton_toggled(bool checked)
{
    if (checked)
    {
        ui->startStopEEGButton->setText("Stop EEG");
        _enobio.startStreaming();
    }
    else
    {
        ui->startStopEEGButton->setText("Start EEG");
        _enobio.stopStreaming();
    }
}
