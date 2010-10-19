#include "download.h"
//#include "dthread.h"
//#include <QNetworkRequest>
//#include <QNetworkReply>
#include <QFileInfo>
#include <QUrl>
//#include <QWaitCondition>
//#include <QMutex>
#include <QDebug>

Download::Download(QObject *parent) :
    QObject(parent)
{
    maxRate = 0;
    maxThreads = 1;
    fileName = "";
    url = "";
    sections.clear();
    manager = new QNetworkAccessManager(this);
}

Download::~Download()
{
    manager->deleteLater();
}

void Download::setFileName(QString fn)
{
    fileName = fn;
}

void Download::setMaxRate(int mr)
{
    maxRate = mr;
}

void Download::setMaxThreads(int mt)
{
    maxThreads = mt;
}

void Download::setUrl(QString u)
{
    url = u;
}

void Download::start()
{
    QNetworkRequest request;
    QUrl ur(url);
    request.setUrl(ur);
    request.setRawHeader("User-Agent", "Downloader 1.0");
    request.setRawHeader("Host", ur.host().toLocal8Bit().data());
//    request.setRawHeader("Referer",url.host().toLocal8Bit().data());
//    request.setRawHeader("Referrer", http://www.my-university.com/)
//    request.setRawHeader("Accept", "image/png,image/*;q=0.8,*/*;q=0.5");
//    request.setRawHeader("Accept-Language", "en-us,en;q=0.5");
//    request.setRawHeader("Accept-Encoding", "gzip,deflate");
//    request.setRawHeader("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
//    request.setRawHeader("Keep-Alive", "300");
//    request.setRawHeader("Connection", "keep-alive");
//    request.setRawHeader("Range",byte.toLocal8Bit().data()); // задаём размер фрагмента
    reply = manager->head(request);
//    reply->setReadBufferSize(10);
    QObject::connect(reply,  SIGNAL(finished()), this, SLOT(fileReplyFinished()));
    QObject::connect(reply, SIGNAL(readyRead()), SLOT(fileReplyFinished()));
    QObject::connect(reply, SIGNAL(metaDataChanged()), SLOT(fileReplyFinished()));
    QObject::connect(manager, SIGNAL(authenticationRequired(QNetworkReply *,  QAuthenticator*)), this, SLOT(httpAuthenticationRequired(QNetworkReply *,  QAuthenticator*)));
    reply->setParent(this);
//    QWaitCondition wt;
//    QMutex req;
//    req.lock();
//    wt.wait(&req, 3000);
//    req.unlock();
//    fileSaving = new QFutureWatcher<qint64>(this);
////    connect(fileSaving, SIGNAL(progressValueChanged(int)), SLOT(showProgress(int)));
//    connect(fileSaving, SIGNAL(resultReadyAt(int)), SLOT(savingResult(int)));
//    connect(odfSaving, SIGNAL(finished()), SLOT(finished()));

//    openButton = new QPushButton(tr("Select HTML files"));
//    connect(openButton, SIGNAL(clicked()), SLOT(open()));

//    cancelButton = new QPushButton(tr("Cancel"));
//    cancelButton->setEnabled(false);
//    connect(cancelButton, SIGNAL(clicked()), odfSaving, SLOT(cancel()));

//    pauseButton = new QPushButton(tr("Pause/Resume"));
//    pauseButton->setEnabled(false);
//    connect(pauseButton, SIGNAL(clicked()), odfSaving, SLOT(togglePaused()));
}

void Download::fileReplyFinished()
{
    if(reply->error())
    {
        qDebug() << "error " << reply->errorString();
        file.close();
        emit downloadFinished(this);
        return;
    }
//    if(reply->operation() == QNetworkAccessManager::HeadOperation)
    if(reply->operation() == QNetworkAccessManager::HeadOperation)
    {
         length = reply->header(QNetworkRequest::ContentLengthHeader).toUInt(); // Получаем длину файла
         QList<QByteArray> hdrs;
         hdrs = reply->rawHeaderList();
//         if(reply->hasRawHeader("Content-Disposition")) str = reply->rawHeader("Content-Disposition");
         if(reply->hasRawHeader("Allow")) str = reply->rawHeader("Allow");
//         if(reply->hasRawHeader("Content-Location")) str = reply->rawHeader("Content-Location");
         if(fileName.isEmpty())
         {
             QFileInfo fileInfo = QUrl(url).path();
             fileName = fileInfo.fileName(); // Получаем имя файла, если не задано
             if(fileName.isEmpty() && reply->hasRawHeader("Content-Location"))
             {
                 fileInfo = QUrl(reply->rawHeader("Content-Location")).path();
                 fileName = fileInfo.fileName();
                 if(fileName.isEmpty() && reply->hasRawHeader("Location"))
                 {
                     fileInfo = QUrl(reply->rawHeader("Location")).path();
                     fileName = fileInfo.fileName();
                     if(fileName.isEmpty()) fileName = "test.txt";
                 }
             }
         }
         file.setFileName(fileName);
         file.open(QIODevice::WriteOnly);
         file.resize(length); // Задаём размер файла
         for(int i = 0; i < maxThreads; i++) // Создаём потоки
         {
             qint64 start = (length / maxThreads) * i;
             qint64 end = (length / maxThreads) * (i + 1);

             DThread *th = new DThread(this, start, end, 1024, url, &file, false);
             connect(th, SIGNAL(signalThreadFinished(DThread*)), SLOT(slotThreadFinished(DThread*)));
//                 th->run(&manager);
             sections.append(th);
             th->start(manager);
         }
    }
//    reply->abort();
//    reply->deleteLater();
//    reply = 0;
}

void Download::slotThreadFinished(DThread *dt)
{
    sections.removeOne(dt);
    if(sections.isEmpty())
    {
        file.close();
        emit downloadFinished(this);
    }
}

void Download::httpAuthenticationRequired(QNetworkReply * rpl, QAuthenticator* auth)
{
    if(reply->error())
    {
        qDebug() << "error " << reply->errorString();
    }
    auth->setUser("valeriy");
    auth->setPassword(";'dfkj");
}
