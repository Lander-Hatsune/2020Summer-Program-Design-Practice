#ifndef INITWINDOW_H
#define INITWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class InitWindow; }
QT_END_NAMESPACE

class InitWindow : public QMainWindow
{
    Q_OBJECT

public:
    InitWindow(QWidget *parent = nullptr);
    ~InitWindow();

private:
    Ui::InitWindow *ui;
};
#endif // INITWINDOW_H
