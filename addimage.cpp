#include "addimage.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QString>
#include "appimage.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <QTimer>
#include <QThread>
#include "filewatcher.h"
#include <QFileSystemWatcher>

AddImage::AddImage(QWidget *parent)
    : QWidget{parent}
{
    //Widget Initialization
    addButton = new QPushButton("Add");
    binPathButton = new QPushButton("bin");
    iconPathButton = new QPushButton("icon");
    terminateButton = new QPushButton("Terminate");

    layout = new QGridLayout;

    nameEdit = new QLineEdit ;
    binPathEdit = new QLineEdit ;
    iconPathEdit = new QLineEdit ;
    categoryEdit = new QLineEdit ;

    nameLabel = new QLabel("Name");
    binPathLabel = new QLabel("Bin Path");
    iconPathLabel = new QLabel("Icon Path");
    categoryLabel = new QLabel("Category");
    outputLabel = new QLabel;

    //Threading
    QThread::currentThread()->setObjectName("Main Thread");

    qDebug() << "1: " << QThread::currentThread();

    QThread* fileThread = new QThread;
    fileThread->setObjectName("File Thread");

    watcher = new FileWatcher;

    watcher->moveToThread(fileThread);

    fileThread->start();

    //Adding widgets to layout
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameEdit, 1, 0);
    layout->addWidget(binPathLabel, 2, 0);
    layout->addWidget(binPathEdit, 3, 0);
    layout->addWidget(iconPathLabel, 4, 0);
    layout->addWidget(iconPathEdit, 5, 0);
    layout->addWidget(categoryLabel, 6, 0);
    layout->addWidget(categoryEdit, 7, 0);
    layout->addWidget(addButton, 8, 0);
    layout->addWidget(terminateButton, 9, 0);
    layout->addWidget(outputLabel, 10, 0);

    layout->addWidget(binPathButton, 3, 1);
    layout->addWidget(iconPathButton, 5, 1);

    //Connecting signals and slots
    connect(binPathButton, SIGNAL(pressed()), this, SLOT(openBinDialog()));
    connect(iconPathButton, SIGNAL(pressed()), this, SLOT(openIconDialog()));
    connect(addButton, SIGNAL(pressed()), this, SLOT(addImage()));
    connect(fileThread, SIGNAL(started()), watcher, SLOT(run()));
    connect(watcher, SIGNAL(watchFileChanged()), this, SLOT(updateOutputText()));

    this->setLayout(layout);


}

void AddImage::openBinDialog()
{
    binPathEdit->setText(QFileDialog::getOpenFileName());
}

void AddImage::openIconDialog()
{
    iconPathEdit->setText(QFileDialog::getOpenFileName());
}

void AddImage::addImage()
{
    //Move image to a new thread
    AppImage* newImage = new AppImage;
    QThread* imageThread = new QThread;

    imageThread->setObjectName("Image Thread");

    newImage->moveToThread(imageThread);

    if(!nameEdit->text().isEmpty() && !binPathEdit->text().isEmpty() && !iconPathEdit->text().isEmpty() && !categoryEdit->text().isEmpty())
    {
        newImage->create(nameEdit->text(), binPathEdit->text(), iconPathEdit->text(), categoryEdit->text());
        imageThread->start();
        updateOutputText();
    }
    else
    {
        QMessageBox::information(this, "App Imager", "All fields not filled!");
    }

    //connect(imageThread, SIGNAL(started()), newImage, SLOT(runImageTool()));
    connect(terminateButton, SIGNAL(pressed()), newImage, SLOT(terminateBash()));

}

void AddImage::updateOutputText()
{
    QString label;

    for(int i = 0; i < watcher->getOutput().size(); i++)
    {
        label += QString("%0 \n").arg(watcher->getOutput().at(i));
    }

    outputLabel->setText(label);
}

void AddImage::runningQuery()
{
    qDebug() << QThread::currentThread() << " is still running";
}
