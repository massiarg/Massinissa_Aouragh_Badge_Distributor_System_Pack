#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager();
    plotOpen = new Plotwindow();
    plotHigh = new Plotwindow();
    plotLow = new Plotwindow();
    plotClose = new Plotwindow();
    plotVolume = new Plotwindow();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(myReplyFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Btn1_clicked()
{
    ui->textEdit->append("Loading data wait a moment...");
    loadWebPage();
}

void MainWindow::on_Btn2_clicked()
{
    plotOpen->makeplot(plotOpen->getValues());
    plotOpen->show();
}

void MainWindow::on_Btn3_clicked()
{
    plotHigh->makeplot(plotHigh->getValues());
    plotHigh->show();
}

void MainWindow::on_Btn4_clicked()
{
    plotLow->makeplot(plotLow->getValues());
    plotLow->show();
}

void MainWindow::on_Btn5_clicked()
{
    plotClose->makeplot(plotClose->getValues());
    plotClose->show();
}

void MainWindow::on_Btn6_clicked()
{
    plotVolume->makeplotVolume(plotVolume->getValues());
    plotVolume->show();
}

void MainWindow::setstrd(QString in){
    MainWindow::strd = in;
}

QString MainWindow::getstrd(){
    return MainWindow::strd;
}

void MainWindow::saveToFile(QList<QList<QPair<QString, QString>>>& in){
    QString fileName = "D:\\GitQT\\HomeworkQT\\HomeworkQT\\Datafromwebsite.txt";
    QFile file(fileName);
    QTextStream stream(&file);

    if(file.open(QIODevice::ReadWrite)){
        for(int i = 0; i<in.size(); i++){
            switch(i){
            case 0:
                stream<<"Open Values"<<"\r\n";
                break;
            case 1:
                stream<<"High Values"<<"\r\n";
                break;
            case 2:
                stream<<"Low Values"<<"\r\n";
                break;
            case 3:
                stream<<"Close Values"<<"\r\n";
                break;
            case 4:
                stream<<"Volume Values"<<"\r\n";
                break;
            }
            QList<QPair<QString, QString>> tempo = in[i];
            for(int j = 0; j<tempo.size(); j++){
                QPair<QString, QString> data = tempo[j];
                stream<< data.first << " - " << data.second<<"\r\n";
            }
        }

        plotOpen->setValues(alldata[0]);
        plotHigh->setValues(alldata[1]);
        plotLow->setValues(alldata[2]);
        plotClose->setValues(alldata[3]);
        plotVolume->setValues(alldata[4]);
        ui->textEdit->append(getstrd());
    }

}

void MainWindow::saveInMemory(QList<QList<QPair<QString, QString> > >& in){
    QDataStream stream(&buffer, QIODevice::ReadWrite);


    for (int i = 0; i<in.size(); i++){
        QList<QPair<QString, QString>> tempo = in[i];
        stream<<tempo;
        for(int j = 0; j<tempo.size(); j++){
            QPair<QString, QString> temp = tempo[j];
            stream<<temp.first;
            stream<<temp.second;
        }
    }


    readFromMemory(buffer);

}

void MainWindow::readFromMemory(QByteArray& in){
    QDataStream readstream(&buffer, QIODevice::ReadOnly);
    for(int i = 0; i<in.size(); i++){
        QString text1;
        QString text2;
        readstream >> text1;
        readstream >> text2;
        if(text1.isEmpty()!=true && text2.isEmpty()!=true){
          qDebug()<<text1<<" - "<<text2<<"\r\n";
        }
    }
}

void MainWindow::loadWebPage(){
    QNetworkRequest request;

    QString urlString = QString("https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=MSFT&apikey=%0").arg("demo");

    QUrl url(urlString);

    request.setUrl(url);

    setstrd("Loading data is finished");
}

void MainWindow::myReplyFinished(QNetworkReply *reply){
    QByteArray webdata = reply->readAll();

    QJsonDocument document = QJsonDocument::fromJson(webdata);

    if(document.isArray() == true){
        QJsonArray rootArray = document.array();

    }else if (document.isObject() == true){
        QJsonObject rootObject = document.object();

        QJsonObject timeSeries = rootObject["Time Series (Daily)"].toObject();
        QStringList keys = timeSeries.keys();

        for(int i = 0; i<5; i++){
            switch (i){
            case 0:{
                for (QString k : keys){
                            QJsonObject dayValues = timeSeries[k].toObject();
                            QString openValue =  dayValues["1. open"].toString();

                            QPair<QString,QString> dataItem;
                            dataItem.first = k;
                            dataItem.second = openValue;

                            graphValues.append(dataItem);
                        }

                        alldata.append(graphValues);
                        graphValues.clear();

                break;}
            case 1:{
                for (QString k : keys){
                            QJsonObject dayValues = timeSeries[k].toObject();
                            QString openValue =  dayValues["2. high"].toString();

                            QPair<QString,QString> dataItem;
                            dataItem.first = k;
                            dataItem.second = openValue;

                            graphValues.append(dataItem);
                        }

                        alldata.append(graphValues);
                        graphValues.clear();

                break;}
            case 2:{
                for (QString k : keys){
                            QJsonObject dayValues = timeSeries[k].toObject();
                            QString openValue =  dayValues["3. low"].toString();

                            QPair<QString,QString> dataItem;
                            dataItem.first = k;
                            dataItem.second = openValue;

                            graphValues.append(dataItem);
                        }

                        alldata.append(graphValues);
                        graphValues.clear();

                break;}
            case 3:{
                for (QString k : keys){
                            QJsonObject dayValues = timeSeries[k].toObject();
                            QString openValue =  dayValues["4. close"].toString();

                            QPair<QString,QString> dataItem;
                            dataItem.first = k;
                            dataItem.second = openValue;

                            graphValues.append(dataItem);
                        }

                        alldata.append(graphValues);
                        graphValues.clear();

                break;}
            case 4:{
                for (QString k : keys){
                            QJsonObject dayValues = timeSeries[k].toObject();
                            QString openValue =  dayValues["5. volume"].toString();

                            QPair<QString,QString> dataItem;
                            dataItem.first = k;
                            dataItem.second = openValue;

                            graphValues.append(dataItem);
                        }

                        alldata.append(graphValues);
                        graphValues.clear();

                break;}
            }
        }

        saveToFile(alldata);
        saveInMemory(alldata);
    }

}
