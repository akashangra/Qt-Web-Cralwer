#include "webcrawlershandler.h"
#include <webcrawler.h>
#include <QTimer>
#include <mutex>
#include <QUrl>
#include <iostream>
#include <mainwindow.h>

WebCrawlersHandler *WebCrawlersHandler::_webCrawlerHandler = nullptr;
WebCrawlersHandler::WebCrawlersHandler(QObject *parent) : QObject(parent),
    _maxCurrentlyCreatedCrawlersAllowed(100),
    _currentyCreatedWebCrawlers(0),
    _timer(new QTimer(this))
{

    connect(_timer,
            &QTimer::timeout,
            this,
            [&]
    {
        if( _currentyCreatedWebCrawlers < _maxCurrentlyCreatedCrawlersAllowed &&
                !_crawlersUrlQueue.empty())
        {
            QString url = _crawlersUrlQueue.dequeue();
            MainWindow::getInstance()->appendLinks( url );
            WebCrawler *webCrawler = new WebCrawler(url, this);
            connect(webCrawler,
                    &WebCrawler::destroyed,
                    this,
                    &WebCrawlersHandler::slot_WebCrawlerDeleted);

            ++_currentyCreatedWebCrawlers;
        }
    });
}

WebCrawlersHandler *WebCrawlersHandler::getInstance(QObject *parent)
{
    static std::once_flag createInstanceOnceFlag;
    std::call_once(createInstanceOnceFlag,[&]
    {
        _webCrawlerHandler = new WebCrawlersHandler( parent );
    });

    return _webCrawlerHandler;
}

bool WebCrawlersHandler::validateUrl(const QString &urlStr)
{
    QUrl url( urlStr );
    if( url.isValid() )
    {
        if( _linksVisited.size() < _maxCrawls)
        {
            if( !_linksVisited.contains(urlStr) )
            {
                if( _hostName == url.host() )
                {
                    return true;
                }
                else
                {
                    std::cout << " hostname: "
                              << _hostName.toStdString()
                              << " doesnot match for url : "
                              << urlStr.toStdString()
                              << std::endl;
                }
            }
        }
        else
        {
            std::cout << urlStr.toStdString()
                      << " can't be crawled as max limit exceeded"
                      << std::endl;
        }

    }
    else
    {
        std::cout << " Invalid url: "
                  << urlStr.toStdString()
                  << std::endl;
    }

    return false;
}

void WebCrawlersHandler::startCrawler(const QString &url)
{
    if( validateUrl( url ) )
    {
        _linksVisited.insert(url);//set to maintain no. of links visited
        _crawlersUrlQueue.enqueue(url);//queue to maintain no. of objects to be created
        if( !_timer->isActive() )//starting timer if not activated
        {
            _timer->start(10);
        }
    }
}

void WebCrawlersHandler::initializeCrawler(const QString &url, int maxCrawls)
{
    //initialzing members
    _maxCrawls = maxCrawls;
    _linksVisited.clear();
    _hostName = QUrl(url).host();
    startCrawler(url);
}

void WebCrawlersHandler::slot_WebCrawlerDeleted(QObject *obj)
{
    Q_UNUSED(obj)
    --_currentyCreatedWebCrawlers;
//    static int counter = 0;
//    std::cout << " Deletion counter : " << ++counter << std::endl;
    if( _crawlersUrlQueue.empty() )
    {
        _timer->stop(); //stopping timer if queue is empty
        if( !_currentyCreatedWebCrawlers )
        {
            MainWindow::getInstance()->onCrawlingCompleted();
        }
    }
}
