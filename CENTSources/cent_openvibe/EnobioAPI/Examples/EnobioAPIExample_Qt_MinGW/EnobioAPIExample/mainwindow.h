#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

#include "Enobio.h"


namespace Ui {
    class MainWindow;
}


//
// Definition of the consumers to receive both data and status from Enobio
//
class EnobioDataConsumer : public QObject, public IDataConsumer
{
    Q_OBJECT
public:
    void receiveData (const PData& data);
signals:
    void newData (QString stringData);
};

class EnobioStatusConsumer : public QObject, public IDataConsumer
{
    Q_OBJECT
public:
    void receiveData (const PData& data);
signals:
    void newStatus (QString stringStatus);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //
    // Declaration of Enobio and consumers
    //
    Enobio _enobio;
    EnobioDataConsumer _enobioDataConsumer;
    EnobioStatusConsumer _enobioStatusConsumer;

private slots:
    void on_startStopEEGButton_toggled(bool checked);
    void on_openCloseEnobioButton_toggled(bool checked);
};

#endif // MAINWINDOW_H
