#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H

#include <QObject>

class QWebEnginePage;

class WebCrawler : public QObject
{
    Q_OBJECT

    friend class WebCrawlersHandler;

private:
    explicit WebCrawler(const QString &url,
                        QObject *parent = nullptr);

    ~WebCrawler();

    QWebEnginePage *_webEnginePage;

    bool _isCrawlingStarted;

    void startCrawling();


signals:
    void signal_CreateCrawler(const QString &url);

private slots:
    void slot_WebPageLoadStarted();
    void slot_WebPageLoadProgress(int progress);
    void slot_WebPageLoadFinished(bool isLoaded);
};

#endif // WEBCRAWLER_H
