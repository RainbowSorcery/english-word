#ifndef WORDMANAGE_H
#define WORDMANAGE_H

#include <QWidget>
#include "wordinsertview.h"

namespace Ui {
class wordManage;
}

class wordManage : public QWidget
{
    Q_OBJECT

public:
    explicit wordManage(QWidget *parent = nullptr);
    ~wordManage();

private slots:

    void on_FlushButton_clicked();

    void on_CearPushButton_clicked();

    void on_SeachPushButton_clicked();



    void on_insertWordButtion_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    wordInsertView* insertView;

private:
    Ui::wordManage *ui;


};

#endif // WORDMANAGE_H
