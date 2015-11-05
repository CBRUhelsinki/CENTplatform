#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDate>
#include <QDebug>
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
    _enobio.registerConsumer(Enobio::ENOBIO_DATA, _enobioDataConsumer);
    _enobio.registerConsumer(Enobio::STATUS, _enobioStatusConsumer);
    // Enobio configuration
    Property property1(Enobio::STR_PROPERTY_ENABLE_CHANNEL_1, true);
    Property property2(Enobio::STR_PROPERTY_ENABLE_CHANNEL_2, true);
    Property property3(Enobio::STR_PROPERTY_ENABLE_CHANNEL_3, true);
    Property property4(Enobio::STR_PROPERTY_ENABLE_CHANNEL_4, true);
    _enobio.setProperty(property1);
    _enobio.setProperty(property2);
    _enobio.setProperty(property3);
    _enobio.setProperty(property4);
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
    EnobioData * pData = (EnobioData *)data.getData();
    // Provide the data to whatever slot is connected to that signal
    emit newData(QString::number(pData->getChannel1()) + "\t" +
                 QString::number(pData->getChannel2()) + "\t" +
                 QString::number(pData->getChannel3()) + "\t" +
                 QString::number(pData->getChannel4()) + "\t" +
                 QString::number(pData->getTimestamp()));
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
        if(_enobio.openDevice())
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
