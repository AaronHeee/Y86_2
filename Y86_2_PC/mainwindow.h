#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "parameter.h"
#include "client.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void displayAll(int n);
    void openText();
    void displayMem(int n);
private slots:
    void cycleDisplay();

    void on_actionOpen_o_triggered();

    void on_Bu_RESET_clicked();

    void on_Bu_STEP_clicked();

    void on_Bu_START_clicked();

    void on_Sl_speed_rangeChanged(int min, int max);

    void on_Sl_speed_valueChanged(int value);

    void on_Bu_BACK_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
