#ifndef STUDYVIEW_H
#define STUDYVIEW_H

#include <QWidget>

namespace Ui {
class StudyView;
}

class StudyView : public QWidget
{
    Q_OBJECT

public:
    explicit StudyView(QWidget *parent = nullptr);
    ~StudyView();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::StudyView *ui;
};

#endif // STUDYVIEW_H
