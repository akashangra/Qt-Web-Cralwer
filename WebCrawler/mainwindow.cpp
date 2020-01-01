#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <QRegularExpression>
#include <iostream>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QDomDocument>
#include <webcrawlershandler.h>
#include <mutex>
#include <QDateTime>

MainWindow *MainWindow::_mInstance = nullptr;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEditWebSites->setReadOnly( true );
}

MainWindow::~MainWindow()
{
    std::cout << __FUNCSIG__ << std::endl;
    delete ui;
}

MainWindow *MainWindow::getInstance()
{
    static MainWindow mainWindow;
    return &mainWindow;
}

void MainWindow::appendLinks(const QString &link)
{
    ui->plainTextEditWebSites->appendPlainText(QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + " " + link + "\n");
}

void MainWindow::onCrawlingCompleted()
{
    ui->pushButtonCrawl->setEnabled(true);
    MainWindow::getInstance()->appendLinks(" Crawling Completed ");
}


void MainWindow::on_pushButtonCrawl_clicked()
{
    ui->plainTextEditWebSites->clear();
    WebCrawlersHandler::getInstance(this)->initializeCrawler( ui->lineEditCrawlLink->text(),
                                                              ui->spinBox->value());
    ui->pushButtonCrawl->setEnabled( false );
}
