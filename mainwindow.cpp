#include "mainwindow.h"
#include "addimage.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    addUI = new AddImage;

    addUIButton = new QPushButton;

    this->setCentralWidget(addUI);
}

MainWindow::~MainWindow()
{
}

