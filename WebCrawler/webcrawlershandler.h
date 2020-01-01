#ifndef WEBCRAWLERSHANDLER_H
#define WEBCRAWLERSHANDLER_H

#include <QObject>
#include <QSet>
#include <QQueue>

class QTimer;

/**
 * @brief The WebCrawlersHandler class : this class initiates web crawling
 */
class WebCrawlersHandler : public QObject
{
    Q_OBJECT

private:
    /**
     * @brief WebCrawlersHandler
     * @param parent
     */
    explicit WebCrawlersHandler(QObject *parent = nullptr);

    /**
     * @brief validateUrl : To validate requested url
     * @param urlStr
     * @return
     */
    bool validateUrl(const QString &urlStr);

    /**
     * @brief _webCrawlerHandler
     */
    static WebCrawlersHandler *_webCrawlerHandler;

    /**
     * @brief _linksVisited : set to maintain links visited to prevent duplicacy
     */
    QSet<QString> _linksVisited;

    /**
     * @brief _maxCurrentlyCreatedCrawlersAllowed : this holds maximum crawler objects allowed at a particular time
     */
    const unsigned short _maxCurrentlyCreatedCrawlersAllowed;
    /**
     * @brief _currentyCreatedWebCrawlers : this maintains currently running crawlers
     */
    unsigned short _currentyCreatedWebCrawlers;

    /**
     * @brief _maxCrawls : to limit no. of crawls
     */
    int _maxCrawls;

    /**
     * @brief _crawlersUrlQueue : requested url is added to this queue to dequeu on timer
     */
    QQueue<QString> _crawlersUrlQueue;

    /**
     * @brief _timer : crawl websites from queue on timer
     */
    QTimer *_timer;

    /**
     * @brief _hostName : host name of initially requested site
     */
    QString _hostName;

public:
    WebCrawlersHandler(const WebCrawlersHandler &other) = delete;
    WebCrawlersHandler& operator=(const WebCrawlersHandler &other) = delete;

    /**
     * @brief getInstance
     * @param parent
     * @return
     */
    static WebCrawlersHandler *getInstance(QObject *parent = nullptr);

    /**
     * @brief startCrawler
     * @param url
     */
    void startCrawler(const QString &url);

    /**
     * @brief initializeCrawler : initiates crawler in the start
     * @param url
     * @param maxCrawls
     */
    void initializeCrawler(const QString &url,int maxCrawls);

public slots:
    /**
     * @brief slot_WebCrawlerDeleted
     * @param obj
     */
    void slot_WebCrawlerDeleted(QObject *obj = nullptr);
};

#endif // WEBCRAWLERSHANDLER_H
