#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
   ui->pushButton->setEnabled(false);
   connect(&dld, SIGNAL(downloadFinished(Download*)), SLOT(allDownloadFinished(Download*)));
   dld.setFileName(tr(""));
   dld.setMaxRate(100000);
   dld.setMaxThreads(2);
   dld.setUrl("http://files-vt.unn.ru/39/39442631945/152841739/Simply_Irresistible_(1999).avi");
   dld.start();
//    manager.append(ui->Url->text());

//    QObject::connect(&manager, SIGNAL(finished()), SLOT(DownloadFinished()));
////    app.exec();

}

void Dialog::SetProgress(int c)
{
    ui->progressBar->setValue(c);
}

void Dialog::allDownloadFinished(Download* dw)
{
    ui->pushButton->setEnabled(true);
}
