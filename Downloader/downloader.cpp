#include "downloader.h"

#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <stdio.h>
#include <QMessageBox>

Downloader::Downloader(QObject *parent) :
    QObject(parent), downloadedCount(0), totalCount(0)
{
}

void Downloader::append(const QStringList &urlList)
{
    foreach (QString url, urlList)
        append(QUrl::fromEncoded(url.toLocal8Bit()));

    if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SIGNAL(finished()));
}

void Downloader::append(const QUrl &url)
{
    downloadQueue.enqueue(url);
    if (downloadedCount == totalCount)
        QTimer::singleShot(10, this, SLOT(startNextDownload()));
    ++totalCount;
}

QString Downloader::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}

void Downloader::startNextDownload()
{
    if (downloadQueue.isEmpty()) {
        printf("%d/%d files downloaded successfully\n", downloadedCount, totalCount);
        emit finished();
        return;
    }

    QUrl url = downloadQueue.dequeue();

    QString filename = saveFileName(url);
    output.setFileName(filename);
    if (!output.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Problem opening save file '%s' for download '%s': %s\n",
                qPrintable(filename), url.toEncoded().constData(),
                qPrintable(output.errorString()));

        startNextDownload();
        return;                 // skip this download
    }

    QNetworkRequest request(url);

//    QNetworkAccessManager downloader;

//    downloader.setUrl(QUrl("http://www.my-university.com/computer-science/generic-datafile.gen"));

//    request.setRawHeader("Host", "http://www.my-university.com/");
//    request.setRawHeader("Referrer", http://www.my-university.com/)
//    request.setRawHeader("User-Agent", "Image Client Linux")
//    request.setRawHeader("Accept", "image/png,image/*;q=0.8,*/*;q=0.5");
//    request.setRawHeader("Accept-Language", "en-us,en;q=0.5");
//    request.setRawHeader("Accept-Encoding", "gzip,deflate");
//    request.setRawHeader("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
//    request.setRawHeader("Keep-Alive", "300");
//    request.setRawHeader("Connection", "keep-alive");
//    request.setRawHeader("Range", "bytes 21010-47021/47022");

//    QNetworkReply *reply = downloader.get(qheader);

    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg("valeriy").arg(";'dfkj").toAscii()).toBase64());

    QObject::connect(&manager, SIGNAL(authenticationRequired(QNetworkReply *, QAuthorization*)), SLOT(httpAuthenticationRequired(int, bool)));
    currentDownload = manager.get(request);
    currentDownload->setReadBufferSize(1024);
    connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));
    connect(currentDownload, SIGNAL(finished()),
            SLOT(downloadFinished()));
    connect(currentDownload, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));

    // prepare the output
    printf("Downloading %s...\n", url.toEncoded().constData());
    downloadTime.start();
}

void Downloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
//    progressBar.setValue(bytesReceived*100/bytesTotal);
//    progressBar.setStatus(bytesReceived, bytesTotal);

//    // calculate the download speed
//    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
//    QString unit;
//    if (speed < 1024) {
//        unit = "bytes/sec";
//    } else if (speed < 1024*1024) {
//        speed /= 1024;
//        unit = "kB/s";
//    } else {
//        speed /= 1024*1024;
//        unit = "MB/s";
//    }

//    progressBar.setMessage(QString::fromLatin1("%1 %2")
//                           .arg(speed, 3, 'f', 1).arg(unit));
//    progressBar.update();
}

void Downloader::downloadFinished()
{
//    progressBar.clear();
    output.close();

    if (currentDownload->error()) {
        // download failed
//        fprintf(stderr, "Failed: %s\n", qPrintable(currentDownload->errorString()));
        QMessageBox::warning(0, tr("HTTP Get"), tr("Failed: %1-%2").arg(currentDownload->error()).arg(qPrintable(currentDownload->errorString())));

    } else {
        printf("Succeeded.\n");
        ++downloadedCount;
    }

    currentDownload->deleteLater();
    startNextDownload();
}

void Downloader::downloadReadyRead()
{
    output.write(currentDownload->readAll());
}

//void Downloader::download(QUrl &url, QString fileName)
//{
//    if (fileName.isEmpty())
//    {
//        if (QFileInfo(url.path()).fileName().isEmpty())
//        {
//            fileName="download.file";
//        } else
//        {
//            fileName=QFileInfo(url.path()).fileName();
//        }
//    }
////если имя не передано (передано пустое), то пытаемся достать имя файла из url, в случае неудачи - обзываем "download.file"
//    down.setFileName(fileName);
//    if (!down.open(QIODevice::WriteOnly))
//    {
//        emit saveFailed();
//        return;
//    }
////проверяем на доступность файла для записи, открываем, иначе - генерируем сигнал ошибки, прерываем выполнение
//    QNetworkRequest request(url);
//    request.setUrl(url);
////формируем запрос
//    reply=manager.get(request);
////отправляем запрос
//    connect(&manager,SIGNAL(finished(QNetworkReply*)), this,SLOT(replyFinished(QNetworkReply*)));
//    connect(reply,SIGNAL(readyRead()),this,SLOT(saveFile()));
////соединяем сигналы и слоты, при поступлении новых данных - вызывается слот saveFile(), при окончании передачи - replyFinished()
//}

//void Downloader::saveFile()
//{
//    down.write(reply->readAll());
////записываем полученные данные в файл
//}
//void Downloader::replyFinished(QNetworkReply *reply)
//{
//    if (reply->error())
//        emit downloadFailed();
//    else
//        emit downloadDone();
////если передача завершилась ошибкой - генерируем сигнал ошибки, иначе - генерируем сигнал успешного окончания передачи
//    down.close();
////закрываем файл
//}


