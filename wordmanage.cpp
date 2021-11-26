#include "wordmanage.h"
#include "ui_wordmanage.h"
#include "wordinsertview.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardItem>
#include <QJsonArray>
#include <QMessageBox>
#include <readconfigutils.h>

wordManage::wordManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wordManage)
{
    ui->setupUi(this);
}

wordManage::~wordManage()
{
    delete ui;
    delete insertView;
}


void wordManage::on_FlushButton_clicked()
{
    QTableWidget* table = ui->tableWidget;

    ReadConfigUtils readConfig;


    QString url = readConfig.getUrl() + "/english/getAllWords";

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


    QJsonArray dataArray = rootObject.value("data").toArray();

    for (int i = 0; i < dataArray.size(); i++)
    {
        table->setRowCount(i + 1);

        table->setColumnWidth(5, 150);
        table->setColumnWidth(6, 150);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(dataArray.at(i)["id"].toInteger()));
        idItem->setFlags(Qt::ItemIsEnabled);
        table->setItem(i, 0, idItem);


        table->setItem(i, 1, new QTableWidgetItem(dataArray.at(i)["wordName"].toString()));
        table->setItem(i, 2, new QTableWidgetItem(dataArray.at(i)["translate"].toString()));

        table->setItem(i, 3, new QTableWidgetItem(dataArray.at(i)["memoryMethod"].toString()));

        QTableWidgetItem* correctNumberItem = new QTableWidgetItem(QString::number(dataArray.at(i)["correctNumber"].toInt()));
        correctNumberItem->setFlags(Qt::ItemIsEnabled);
        table->setItem(i, 4, correctNumberItem);


        QTableWidgetItem* incorrectNumberItem = new QTableWidgetItem(QString::number(dataArray.at(i)["incorrectNumber"].toInt()));
        // 设置单元格为无法编辑
        incorrectNumberItem->setFlags(Qt::ItemIsEnabled);
        table->setItem(i, 5, incorrectNumberItem);

        QTableWidgetItem* occurrenceNumberItem = new QTableWidgetItem(QString::number(dataArray.at(i)["occurrenceNumber"].toInt()));
        occurrenceNumberItem->setFlags(Qt::ItemIsEnabled);
        table->setItem(i, 6, occurrenceNumberItem);

        qDebug() << dataArray.at(i)["wordType"].toInt();

        // todo 可优化为下拉列表框 懒
        if (dataArray.at(i)["wordType"].toInt() == 0) {
            table->setItem(i, 7, new QTableWidgetItem(QString("单词")));
        } else if (dataArray.at(i)["wordType"].toInt() == 1) {

            table->setItem(i, 7, new QTableWidgetItem(QString("前缀")));
        } else if (dataArray.at(i)["wordType"].toInt() == 2) {

            table->setItem(i, 7, new QTableWidgetItem(QString("后缀")));
        } else {
            table->setItem(i, 7, new QTableWidgetItem(QString("词根")));

        }


        QTableWidgetItem* createDateItem = new QTableWidgetItem(dataArray.at(i)["createDate"].toString());
        createDateItem->setFlags(Qt::ItemIsEnabled);
        table->setItem(i, 8, createDateItem);


//        QTableWidgetItem* updateDateItem = new QTableWidgetItem(QString::number(dataArray.at(i)["updateDate"].toInt()));
        QTableWidgetItem* updateDateItem = new QTableWidgetItem(dataArray.at(i)["updateDate"].toString());
        updateDateItem->setFlags(Qt::ItemIsEnabled);
        table->setItem(i, 9, updateDateItem);


    }

    QMessageBox *msgBox = new QMessageBox("提示", rootObject.value("message").toString(), QMessageBox::Information, QMessageBox::Ok, 0, 0);

    msgBox->show();

}


void wordManage::on_CearPushButton_clicked()
{
    QLineEdit* seachInputLine = ui->SearchLineEdit;
    seachInputLine->setText("");
}


void wordManage::on_SeachPushButton_clicked()
{

    QTableWidget* table = ui->tableWidget;


    QString searchCondition = ui->SearchLineEdit->text();


    if (searchCondition == "") {
        for (int i = 0; i < table->rowCount(); i++) {
             table->setRowHidden(i, false);
        }
    } else
    {
        for (int i = 0; i < table->rowCount(); i++) {
               table->setRowHidden(i, true);//隐藏所有行
        }

        QList<QTableWidgetItem*> itemList = table->findItems(searchCondition, Qt::MatchContains);
        if (!itemList.isEmpty()) { //不为空
             for (int i = 0; i < itemList.count(); i++) {
                 table->setRowHidden(itemList.at(i)->row(), false); //item.at(i).row()输出行号
             }
        }
    }
}


void wordManage::on_insertWordButtion_clicked()
{
    wordInsertView* insertView = new wordInsertView();
    insertView->show();
}



void wordManage::on_pushButton_clicked()
{
    QList<QTableWidgetItem*> itemList = ui->tableWidget->selectedItems();


    ReadConfigUtils readConfig;


    QString url = readConfig.getUrl() + "/english/delete?wordName=" +  itemList.first()->text();

    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QNetworkRequest request;
    request.setUrl(QUrl(url));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *pReply = manager->post(request, "");


    QEventLoop eventLoop;
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    QByteArray bytes = pReply->readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(bytes));

    QJsonObject rootObject = jsonDocument.object();


    QMessageBox *msgBox = new QMessageBox("提示", rootObject.value("message").toString(), QMessageBox::Information, QMessageBox::Ok, 0, 0);

    msgBox->show();

    on_FlushButton_clicked();
}


void wordManage::on_pushButton_2_clicked()
{
    ReadConfigUtils readConfig;


    QString url = readConfig.getUrl() + "/english/updateWord";

    QList<QTableWidgetItem*> slectedItems = ui->tableWidget->selectedItems();

    if (slectedItems.size() > 0) {

        QString wordName = slectedItems.at(0)->text();
        QString transalte = slectedItems.at(1)->text();
        QString id = ui->tableWidget->item(slectedItems.at(0)->row(), 0)->text();



        QJsonObject jsonObject;
        jsonObject.insert("id", id);
        jsonObject.insert("wordName", wordName);
        jsonObject.insert("translate", transalte);
        jsonObject.insert("memoryMethod", slectedItems.at(2)->text());

        qDebug() << slectedItems.at(3)->text();

        bool flag = true;

        QString wordTypeString =  slectedItems.at(3)->text();

        if (wordTypeString ==  "单词") {
            jsonObject.insert("wordType", 0);
        } else if (wordTypeString  ==  "前缀") {
            jsonObject.insert("wordType", 1);
        } else if (wordTypeString ==  "后缀") {
            jsonObject.insert("wordType", 2);
        } else if (wordTypeString ==  "词根") {
            jsonObject.insert("wordType", 3);
        } else {
            QMessageBox *msgBox = new QMessageBox("提示", "请输入类型为: 单词 | 前缀 | 后缀 | 词根", QMessageBox::Information, QMessageBox::Ok, 0, 0);

            msgBox->show();

            flag = false;

            return ;
        }


        if (flag) {
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
        } else {

            QMessageBox *msgBox = new QMessageBox("提示", "更新失败", QMessageBox::Information, QMessageBox::Ok, 0, 0);

            msgBox->show();
        }

    } else {

        QMessageBox *msgBox = new QMessageBox("提示", "请选中要修改列", QMessageBox::Information, QMessageBox::Ok, 0, 0);

        msgBox->show();
    }


}

