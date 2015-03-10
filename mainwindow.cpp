#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i=0;i<serial_info.availablePorts().length();i++){
        ui->comboBox->addItem(serial_info.availablePorts()[i].portName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    serial = new QSerialPort;
    serial->setPort(serial_info.availablePorts()[ui->comboBox->currentIndex()]);
    if(serial->open(QIODevice::ReadWrite)){
        qDebug() << ui->comboBox->currentIndex();
        qDebug() << serial_info.availablePorts()[ui->comboBox->currentIndex()].portName();
        serial->setBaudRate(250000);
        serial->setDataBits(QSerialPort::Data8);
        serial->setStopBits(QSerialPort::TwoStop);
        serial->setParity(QSerialPort::NoParity);
        qDebug() << serial->baudRate();
        qDebug() << serial->stopBits();
        QByteArray config;
        config.append(0x0A);
        config.append(0xA0);
        config.append(0xFF);
        config.append(0x01);
        config.append((quint8)(ui->spinBox->value()>>8));
        config.append((quint8)ui->spinBox->value()&0xFF);
        config.append(0x01);
        config.append(0xFF);
        config.append(0xA0);
        config.append(0x0A);
        qDebug() << "length: " << config.length();
        qDebug() << config.toHex();
        serial->write(config.data(),config.length());
//        serial->waitForBytesWritten(500);
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();
        serial->close();
    }
    delete serial;
}

void MainWindow::on_pushButton_clicked()
{
    ui->comboBox->clear();
    for(int i=0;i<serial_info.availablePorts().length();i++){
        ui->comboBox->addItem(serial_info.availablePorts()[i].portName());
    }
}
