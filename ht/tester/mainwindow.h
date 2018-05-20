#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent* event = 0);
    void getResults(char mode, int* ptrResults);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
