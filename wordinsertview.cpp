#include "wordinsertview.h"
#include "ui_wordinsertview.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <readconfigutils.h>

wordInsertView::wordInsertView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wordInsertView)
{
    ui->setupUi(this);
}

wordInsertView::~wordInsertView()
{
    delete ui;
}

void wordInsertView::on_insertWordButton_clicked()
{
    QString englishWord = ui->EnglishlineEdit->text();
    QString translateWord = ui->TranslatelineEdit_2->text();
    ReadConfigUtils readConfig;


    QString url = readConfig.getUrl() + "/english/insertWord";

    QJsonObject jsonObject;
    jsonObject.insert("wordName", englishWord);
    jsonObject.insert("translate", translateWord);
    jsonObject.insert("memoryMethod", ui->TranslatelineEdit_3->text());

    if (ui->wordTypeRadio->isChecked()) {
        jsonObject.insert("wordType", 0);
    } else if (ui->preffixRadioType->isChecked()) {
        jsonObject.insert("wordType", 1);
    } else if (ui->suffixRadioType->isChecked()) {
        jsonObject.insert("wordType", 2);
    } else if (ui->bodyRadioType->isChecked()) {
        jsonObject.insert("wordType", 3);
    }

    QByteArray dataArray;
    QJsonDocument document;
    document.setObject(jsonObject);
    dataArray = document.toJson(QJsonDocument::Compact);

    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *pReply = manager->post(request, dataArray);
    QEventLoop eventLoop;
        QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();
    //    // 获取网页Body中的内容
        QByteArray bytes = pReply->readAll();

        QJsonDocument jsonDocument(QJsonDocument::fromJson(bytes));
        QJsonObject messageJson = jsonDocument.object();


        QMessageBox *msgBox = new QMessageBox("提示", messageJson.value("message").toString(), QMessageBox::Information, QMessageBox::Ok, 0, 0);

        msgBox->show();
}

