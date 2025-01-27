#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    network_access_manager = new QNetworkAccessManager(this);
}

MainWindow::~MainWindow()
{
    delete ui;

    if (network_access_manager)
        delete network_access_manager;
}

void MainWindow::on_pushButton_clicked()
{
    QUrl url("ftp://192.168.x.x/D:/file");
    url.setUserName("1");
    url.setPassword("1");
    url.setPort(55);

    QString str = "dene.txt";
    file = new QFile(str, this);

    QNetworkRequest request(url);
    file->open(QIODevice::ReadOnly);
    QByteArray byte_file = file->readAll();
    network_reply = network_access_manager->put(request, byte_file);

    connect(network_reply, SIGNAL(uploadProgress(qint64, qint64)),
            this, SLOT(OnUploadProgress(qint64, qint64)));
    connect(network_access_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(OnReplyFinished(QNetworkReply*)));
    connect(network_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(OnLoadError(QNetworkReply::NetworkError)));
}

void MainWindow::OnUploadProgress(qint64 byte_sents, qint64 byte_totals)
{
    qDebug() << "Uploaded : " << byte_sents << " of " << byte_totals;
}

void MainWindow::OnReplyFinished(QNetworkReply *)
{
    if (network_reply->error() == QNetworkReply::NoError)
        qDebug() << "Ftp complete.";
    else
        qDebug() << network_reply->errorString();

    network_reply->deleteLater();
    network_reply->close();
    network_reply->deleteLater();
}

void MainWindow::OnLoadError(QNetworkReply::NetworkError error)
{
    qDebug() << "Load Error : " << error;
}
