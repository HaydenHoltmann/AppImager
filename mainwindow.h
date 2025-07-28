#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class AddImage;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    AddImage* addUI;

    QPushButton* addUIButton;

};
#endif // MAINWINDOW_H
