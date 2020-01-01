#include "webcrawler.h"
#include <QWebEnginePage>
#include <webcrawlershandler.h>
#include <QTimer>
#include <iostream>

WebCrawler::WebCrawler(const QString &url, QObject *parent) : QObject(parent),
    _webEnginePage( new QWebEnginePage(this) ),
    _isCrawlingStarted(false)
{
    connect(_webEnginePage,
            &QWebEnginePage::loadFinished,
            this,
            &WebCrawler::slot_WebPageLoadFinished);

//    connect(_webEnginePage,
//            &QWebEnginePage::loadStarted,
//            this,
//            &WebCrawler::slot_WebPageLoadStarted);

//    connect(_webEnginePage,
//            &QWebEnginePage::loadProgress,
//            this,
//            &WebCrawler::slot_WebPageLoadProgress);

    QTimer::singleShot(10000,this,
                       [&]
    {
        if( !_isCrawlingStarted )
        {
            deleteLater();
        }
    });


    _webEnginePage->setUrl(QUrl(url));
}

WebCrawler::~WebCrawler()
{
    std::cout << "web crawler deleted" << std::endl;
}

void WebCrawler::startCrawling()
{
    _isCrawlingStarted = true;

    //calling javascript method to get all links
    _webEnginePage->runJavaScript("function myFunction() {"
                              "var array = [];"
                              "var links = document.getElementsByTagName('a');"
                              "for(var i=0, max=links.length; i<max; i++) {"
                             "     array.push(links[i].href);"
                              "}"
                              "return array"
                              "} myFunction();",
                                [&] (const QVariant &result)
    {
        QVariantList urlsList = result.toList();
        for( const QVariant &url : urlsList)
        {
            WebCrawlersHandler::getInstance()->startCrawler(url.toString());
        }
        deleteLater();

    }
    );
}

void WebCrawler::slot_WebPageLoadStarted()
{
    std::cout << __FUNCSIG__ << std::endl;
}

void WebCrawler::slot_WebPageLoadProgress(int progress)
{
    Q_UNUSED(progress)
    std::cout << __FUNCSIG__ << std::endl;
}

void WebCrawler::slot_WebPageLoadFinished(bool isLoaded)
{
    if( isLoaded )
    {
        startCrawling();
    }
    else
    {
        deleteLater();
    }
}
