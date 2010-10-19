#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "downloader.h"
#include "download.h"

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    void SetProgress(int);
    ~Dialog();

public slots:
    void allDownloadFinished(Download* dw);

private:
    Ui::Dialog *ui;
    Downloader manager;
    Download dld;

private slots:
    void on_pushButton_clicked();
};

#endif // DIALOG_H
