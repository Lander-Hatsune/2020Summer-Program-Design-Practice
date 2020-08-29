#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
using std::cout;
using std::endl;

inline int getpix(int x) {
    return (x - 1) * (SCREEN_SIZE / GRID_SIZE);
}

inline int getpos(int px) {
    return (int)(px / (SCREEN_SIZE / GRID_SIZE)) + 1;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    game = Snakegame();
    this->setWindowTitle("Snake");
    this->setMouseTracking(true);

    this->roundtimer = new QTimer();
    this->roundtimer->setInterval(60);
    connect(this->roundtimer, SIGNAL(timeout()), this, SLOT(nxt_round()));

    this->rendtimer = new QTimer();
    rendtimer->setInterval(30);
    connect(this->rendtimer, SIGNAL(timeout()), this, SLOT(update()));

    this->screen = QPixmap(SCREEN_SIZE, SCREEN_SIZE);
    connect(this, SIGNAL(gameover()), this, SLOT(dealgameover()));

    ui->b_save->setDisabled(true);
    ui->actionSave->setDisabled(true);
    ui->b_pause->setDisabled(true);
    ui->actionPause->setDisabled(true);

    rend();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_W) {
        game.change_dir(1);
    }
    else if (ev->key() == Qt::Key_A) {
        game.change_dir(2);
    }
    else if (ev->key() == Qt::Key_S) {
        game.change_dir(3);
    }
    else if (ev->key() == Qt::Key_D) {
        game.change_dir(4);
    }
}
#ifdef WIN32
const int bias_x = 20;
const int bias_y = 84;
#else
const int bias_x = 20;
const int bias_y = 106;
#endif
bool vis[45][45];
void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    if (ui->lbl_state->text() == "<html><head/><body><p align=\"center\"><span style=\" font-size:48pt; font-weight:600; font-style:italic; color:#53a485;\">Initializing</span></p></body></html>") {
        int x = getpos(ev->pos().x() - bias_x);
        int y = getpos(ev->pos().y() - bias_y);
        if (x == 1 || x == GRID_SIZE || y == 1 || y == GRID_SIZE) {
            return;
        }
        if (game.arena[x][y] == 0 && !vis[x][y]) {
            game.arena[x][y] = 3;
            vis[x][y] = 1;
        }
        else if (game.arena[x][y] == 3 && !vis[x][y]) {
            game.arena[x][y] = 0;
            vis[x][y] = 1;
        }
        rend();
        update();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if (ui->lbl_state->text() == "<html><head/><body><p align=\"center\"><span style=\" font-size:48pt; font-weight:600; font-style:italic; color:#53a485;\">Initializing</span></p></body></html>") {
        int x = getpos(ev->pos().x() - bias_x);
        int y = getpos(ev->pos().y() - bias_y);
        if (x == 1 || x == GRID_SIZE || y == 1 || y == GRID_SIZE) {
            return;
        }
        if (game.arena[x][y] == 0) {
            game.arena[x][y] = 3;
            vis[x][y] = 1;
        }
        else if (game.arena[x][y] == 3) {
            game.arena[x][y] = 0;
            vis[x][y] = 1;
        }
        rend();
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    memset(vis, 0, sizeof(vis));
}

void MainWindow::nxt_round()
{
    int res = this->game.move();
    if (res == -1) {
        this->roundtimer->stop();
        emit gameover();
    }
    ui->lcd_score->display(game.score);
    rend();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    ui->lbl_main->setPixmap(screen);
}

void MainWindow::rend()
{
    this->screen.fill(Qt::darkGray);
    for (int i = 1; i <= GRID_SIZE; i++) {
        for (int j = 1; j <= GRID_SIZE; j++) {
            if (game.arena[i][j] == 1) {// fruit
                this->Terrain(i, j, 1);
            }
            else if (game.arena[i][j] == 3) {// wall
                this->Terrain(i, j, 3);
            }
        }
    }
    int qsize = game.snake.size();
    for (int i = 1; i <= qsize; i++) {
        cord cur = game.snake.front();
        if (qsize >= 5) {
            if (i == qsize) {// head
                Terrain(cur.x, cur.y, 4);
            }
            else if (i == 1) {// tail
                Terrain(cur.x, cur.y, 5);
            }
            else if (i == qsize - 1) {
                Terrain(cur.x, cur.y, 6);
            }
            else if (i == 2) {
                Terrain(cur.x, cur.y, 7);
            }
            else {
                Terrain(cur.x, cur.y, 2);
            }
        }
        else {
            if (i == qsize) {// head
                Terrain(cur.x, cur.y, 4);
            }
            else if (i == 1) {// tail
                Terrain(cur.x, cur.y, 5);
            }
            else {
                this->Terrain(cur.x, cur.y, 2);
            }
        }
        game.snake.pop();
        game.snake.push(cur);
    }
}

void MainWindow::setbools(bool r, bool p, bool q, bool s, bool l)
{
    ui->b_stt->setDisabled(r);
    ui->actionStart->setDisabled(r);
    ui->b_pause->setDisabled(p);
    ui->actionPause->setDisabled(p);
    ui->b_quit->setDisabled(q);
    ui->actionQuit->setDisabled(q);
    ui->b_save->setDisabled(s);
    ui->actionSave->setDisabled(s);
    ui->b_load->setDisabled(l);
    ui->actionLoad->setDisabled(l);
}

void MainWindow::dealgameover()
{
    ui->lbl_state->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:48pt; font-weight:600; font-style:italic; color:#53a485;\">GAME OVER</span></p></body></html>");
    QMessageBox overmsg;
    overmsg.setWindowTitle("Game over");
    overmsg.setText("Game over, try again?");
    overmsg.setStandardButtons(QMessageBox::Yes | QMessageBox::Close);
    int ret = overmsg.exec();
    if (ret == QMessageBox::Yes) {
        on_b_stt_clicked();
    }
    else if (ret == QMessageBox::Close) {
        this->close();
    }
}

void MainWindow::Terrain(int x, int y, int type)
{
    QPainter ptr(&screen);
    int px = getpix(x);
    int py = getpix(y);
    ptr.setPen(Qt::darkGray);
    if (type == 1) {// fruit
        ptr.setBrush(Qt::magenta);
    }
    else if (type == 2) {// body
        ptr.setBrush(QBrush(QColor::fromRgb(102, 205, 170)));
    }
    else if (type == 3) {// wall
        ptr.setBrush(Qt::lightGray);
    }
    else if (type == 4) {// head
        ptr.setPen(Qt::lightGray);
        ptr.setBrush(QBrush(QColor::fromRgb(122, 225, 190)));
    }
    else if (type == 5) {// tail
        ptr.setPen(Qt::darkGreen);
        ptr.setBrush(QBrush(QColor::fromRgb(82, 185, 150)));
    }
    else if (type == 6) {// neck
        ptr.setPen(Qt::gray);
        ptr.setBrush(QBrush(QColor::fromRgb(112, 215, 180)));
    }
    else if (type == 7) {// leg
        ptr.setPen(Qt::darkGreen);
        ptr.setBrush(QBrush(QColor::fromRgb(92, 195, 160)));
    }
    ptr.drawRect(px, py, SCREEN_SIZE / GRID_SIZE, SCREEN_SIZE / GRID_SIZE);
}



void MainWindow::on_b_stt_clicked()
{
    if (ui->b_stt->text() == "Start") {
        ui->b_stt->setText("Restart");
        ui->actionStart->setText("Restart");
        ui->actionStart->setIcon(QIcon(":/new/prefix1/redo.jpg"));
        ui->lbl_state->setText("");

        setbools(true, false, false, true, true);

        int fx = (rand() % GRID_SIZE) + 1;
        int fy = (rand() % GRID_SIZE) + 1;
        while (game.arena[fx][fy] != 0) {
           fx = (rand() % GRID_SIZE) + 1,
           fy = (rand() % GRID_SIZE) + 1;
        }
        game.arena[fx][fy] = 1;
        rend();
        roundtimer->start();
        rendtimer->start();
    }
    else {
        ui->b_pause->setText("Pause");
        ui->actionPause->setText("Pause");
        ui->actionPause->setIcon(QIcon(":/new/prefix1/pause.jpg"));

        setbools(false, true, false, true, false);

        ui->lbl_state->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:48pt; font-weight:600; font-style:italic; color:#53a485;\">Initializing</span></p></body></html>");
        ui->b_stt->setText("Start");
        ui->actionStart->setText("Start");
        ui->actionStart->setIcon(QIcon(":/new/prefix1/play.jpg"));
        game = Snakegame();
        rend();
        roundtimer->stop();
        rendtimer->stop();
    }
}

void MainWindow::on_b_pause_clicked()
{
    if (ui->b_pause->text() == "Pause") {
        ui->lbl_state->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:48pt; font-weight:600; font-style:italic; color:#53a485;\">Paused</span></p></body></html>");

        setbools(false, false, false, false, true);

        roundtimer->stop();
        rendtimer->stop();
        ui->b_pause->setText("Resume");
        ui->actionPause->setText("Resume");
        ui->actionPause->setIcon(QIcon(":/new/prefix1/play.jpg"));
    }
    else {
        setbools(true, false, false, true, true);

        ui->lbl_state->setText("");
        roundtimer->start();
        rendtimer->start();
        ui->b_pause->setText("Pause");
        ui->actionPause->setText("Pause");
        ui->actionPause->setIcon(QIcon(":/new/prefix1/pause.jpg"));

    }
}

void MainWindow::on_b_quit_clicked()
{
    if (ui->b_stt->text() != "Start" && ui->b_pause->text() == "Pause") {
        on_b_pause_clicked();
    }
    QMessageBox msgb;
    msgb.setText("Are you sure to quit?");
    msgb.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgb.setDefaultButton(QMessageBox::No);
    int ret = msgb.exec();
    if (ret == QMessageBox::Yes) {
        this->close();
    }
    else if (ui->b_stt->text() != "Start") {
        if (ui->b_pause->text() == "Resume") {
            on_b_pause_clicked();
        }
    }
}

void MainWindow::on_actionStart_triggered()
{
    on_b_stt_clicked();
}

void MainWindow::on_actionPause_triggered()
{
    on_b_pause_clicked();
}

void MainWindow::on_actionQuit_triggered()
{
    on_b_quit_clicked();
}

void MainWindow::on_b_save_clicked()
{
    this->hide();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "./",
                                                    tr("Text (*.txt)"));
    this->show();
    if (fileName.isEmpty()) return;
    game.save(fileName.toStdString().c_str());
}

void MainWindow::on_b_load_clicked()
{
    on_b_stt_clicked();
    on_b_pause_clicked();
    this->hide();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "./",
                                                    tr("Text (*.txt)"));
    this->show();
    if (fileName.isEmpty()) return;
    game.load(fileName.toStdString().c_str());
    ui->lcd_score->display(game.score);
    rend();
    update();
}

void MainWindow::on_actionSave_triggered()
{
    on_b_save_clicked();
}

void MainWindow::on_actionLoad_triggered()
{
    on_b_load_clicked();
}
