#ifndef JOINDLG_H
#define JOINDLG_H

#include <QDialog>
#include <QThread>
#include <QtNetwork>
#include "definitions.h"
#include "gamewindow.h"

namespace Ui {
class JoinDlg;
}

class JoinDlg : public QDialog
{
    Q_OBJECT

public:
    explicit JoinDlg(QWidget *parent = nullptr);
    ~JoinDlg();

private slots:
    void on_b_ok_clicked();
    void on_b_cancel_clicked();

private:
    QWidget* parent;
    Ui::JoinDlg *ui;
};

#endif // JOINDLG_H
