#include "filewatcher.h"
#include <QFileSystemWatcher>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QThread>

FileWatcher::FileWatcher(QObject *parent)
{

}

QStringList FileWatcher::getOutput()
{
    return output;
}

void FileWatcher::watchFile()
{
    qDebug() << "File being watched on " << QThread::currentThread();

    //QFile file("/home/hayden/Desktop/Test File.txt");
    QFile file("/home/hayden/Desktop/output.txt");

    QTextStream stream(&file);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file";
    }

    QStringList tempOutput;

    while(stream.atEnd() == false)
    {
        tempOutput << stream.readLine();
    }

    output = tempOutput;

    //qDebug() << output;

    //deleteLater();

    emit watchFileChanged();


}

void FileWatcher::run()
{
    qDebug() << "Running " << QThread::currentThread();

    QFileSystemWatcher* watcher = new QFileSystemWatcher;

    watcher->addPath("/home/hayden/Desktop/output.txt");
    //watcher->addPath("/home/hayden/Desktop/Test File.txt");

    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(watchFile()));

}
