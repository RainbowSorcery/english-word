#include "studyview.h"
#include "ui_studyview.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <readconfigutils.h>

StudyView::StudyView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudyView)
{
    ui->setupUi(this);
}

StudyView::~StudyView()
{
    delete ui;
}

void StudyView::on_pushButton_2_clicked()
{
    QString randomStatus = "";

    if (ui->radioButton->isChecked()) {
        randomStatus = "0";
    } else if (ui->radioButton_2->isChecked()) {
        randomStatus = "1";
    } else {
        randomStatus = "2";
    }
    ReadConfigUtils readConfig;



    QString url = readConfig.getUrl() + "/english/getRandomWord?randomStatus=" + randomStatus;

    QNetworkAccessManager* manager = new QNetworkAccessManager();

    QNetworkRequest requset;
    requset.setUrl(QUrl(url));

    QNetworkReply* reply = manager->get(requset);

    QEventLoop eventLoop;
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    QByteArray byteArray = reply->readAll();

    QJsonDocument jsonDocument(QJsonDocument::fromJson(byteArray));

    QJsonObject rootObject = jsonDocument.object();

    QJsonObject dataObject = rootObject.value("data").toObject();



    ui->label->setText(dataObject.value("wordName").toString());

    qDebug() << dataObject.value("wordName").toInt();

    if (dataObject.value("wordType").toInt() == 0) {
        ui->label_3->setText("单词");
    } else if (dataObject.value("wordType").toInt()  == 1) {
            ui->label_3->setText("前缀");

    } else if (dataObject.value("wordType").toInt()  == 2) {
            ui->label_3->setText("后缀");
    } else {
           ui->label_3->setText("词根");

    }


}


void StudyView::on_pushButton_clicked()
{
    QString wordName = ui->label->text();
    QString translate = ui->lineEdit->text();
    ReadConfigUtils readConfig;


    QString url = readConfig.getUrl() + "/english/checkWord?wordName=" + wordName +"&translate=" + translate;

    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QNetworkRequest request;
    request.setUrl(QUrl(url));

    QNetworkReply *pReply = manager->get(request);


    QEventLoop eventLoop;
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    QByteArray bytes = pReply->readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(bytes));

    QJsonObject rootObject = jsonDocument.object();


    QMessageBox *msgBox = new QMessageBox("提示", rootObject.value("message").toString(), QMessageBox::Information, QMessageBox::Ok, 0, 0);

    msgBox->show();

    on_pushButton_2_clicked();
}

