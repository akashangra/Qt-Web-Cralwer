#include "webcrawlersrunnable.h"
#include <webcrawlerworkerobject.h>

WebCrawlersRunnable::WebCrawlersRunnable(const QString &url):
    _url( url )
{

}

void WebCrawlersRunnable::run()
{
    WebCrawlerWorkerObject workerObj(_url);
}
