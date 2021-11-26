#ifndef WORDINSERTVIEW_H
#define WORDINSERTVIEW_H

#include <QWidget>

namespace Ui {
class wordInsertView;
}

class wordInsertView : public QWidget
{
    Q_OBJECT

public:
    explicit wordInsertView(QWidget *parent = nullptr);
    ~wordInsertView();

private slots:
    void on_insertWordButton_clicked();

private:
    Ui::wordInsertView *ui;
};

#endif // WORDINSERTVIEW_H
