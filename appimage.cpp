#include "appimage.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QProcess>
#include <QThread>

//Testing - Delete later
#include <QThread>
/*Default path should be set to home, however the folder should only be created once the first app image is created...probably*/

AppImage::AppImage() {

    //If directory doesn't exist, create it
    //Home Directory is the main directory that the user chooses to create the folder, home by defualt
    homeDirectory = new QDir(QDir::homePath());
    //The directory in which all AppImages will be put
    workingDirectory = new QDir(QDir::homePath());

    if(workingDirectory->exists("AppImages") == false)
    {
        workingDirectory->mkdir("AppImages");
    }

    workingDirectory->cd("AppImages");

    //Path with be gotten from config file
    toolPath = "/home/hayden/AppImages/appimagetool.AppImage";

}

AppImage::~AppImage()
{
}

void AppImage::create(QString iName, QString binPath, QString iconPath, QString category)
{
    qDebug() << "Create running on " << QThread::currentThread();

    //Creates main folder
    imageName = iName;
    fileName = iName.replace(" ", "_");

    writeCompileScript();

    imageFolder();

    //Creates bin folder
    binFolder(binPath);

    //Copies icon to main folder
    moveIcon(iconPath);

    desktopConfig(category);

    runScript();

    runImageTool();

}

void AppImage::imageFolder()
{
    QString mainFileName = QString("%1.AppDir").arg(fileName);

    if(workingDirectory->exists(mainFileName) == true)
    {
        qDebug() << "File already exists";
    }
    else
    {
        workingDirectory->mkdir(mainFileName);
    }

    workingDirectory->cd(mainFileName);

}

void AppImage::binFolder(QString binPath)
{
    QDir* binDirectory = new QDir(workingDirectory->absolutePath());

    if(binDirectory->exists("usr") == false)
    {
        binDirectory->mkdir("usr");
    }

    binDirectory->cd("usr");

    if(binDirectory->exists("bin") == false)
    {
        binDirectory->mkdir("bin");
    }

    binDirectory->cd("bin");

    //QDir* binCopyFolder = new QDir(binPath);

    //Only binary for now
    QFile::copy(binPath, binDirectory->absolutePath() + QString("/%0").arg(fileName));

    //List of all the files in the folder
    /*QStringList* binList = new QStringList(binCopyFolder->entryList());

    for(int i = 0; i < binList->size(); i++)
    {
        QFile::copy(binPath + binList->at(i), binDirectory->absolutePath() + QString("/%0").arg(binList->at(i)));
    }*/

    delete binDirectory;
    qDebug() << "Bin files transfer completed.";
}

void AppImage::moveIcon(QString iconPath)
{
    QFile::copy(iconPath, QString("%0/%1-256x256").arg(workingDirectory->absolutePath(), fileName));

    qDebug() << "Icon transferred successfully.";
}

void AppImage::desktopConfig(QString category)
{
    QFile configFile(QString("%0/%1.desktop").arg(workingDirectory->absolutePath(), fileName));

    if(!configFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Desktop congig file failed to open";
    }

    QTextStream stream(&configFile);

    stream << "[Desktop Entry]\n" << QString("Name=%0\n").arg(imageName) << QString("Exec=%0\n").arg(fileName) << QString("Icon=%0-256x256\n").arg(fileName);

    stream << "Type=Application\n" << QString("Categories=%0").arg(category);

    configFile.close();

    qDebug() << "Desktop config file created.";

}

void AppImage::runScript()
{
    QFile scriptFile(QString("%0/AppRun").arg(workingDirectory->absolutePath()));
    
    if(!scriptFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Script file failed to open";
    }
    
    QTextStream stream(&scriptFile);
    
    stream << "#!/bin/sh\n" << "HERE=\"$(dirname \"$(readlink -f \"${0}\")\")\"\n" << QString("EXEC=\"${HERE}/usr/bin/%0\"\n").arg(fileName) << "exec \"${EXEC}\"";

    scriptFile.close();

    qDebug() << "Script file created.";
}

void AppImage::runImageTool()
{
    //The bash script runs a process, and the process will still run in the background even if you close the application because it was run through the terminal with a script.
    qDebug() << "runImageTool is running on: " << QThread::currentThread();

    bashScript = new QProcess();

    QStringList arguments;

    arguments << QString("%0/AppImages/AppImageCompile.sh").arg(homeDirectory->absolutePath());

    //bashScript->start(toolPath, arguments);
    //bashScript->execute(toolPath, arguments);
    bashScript->execute("sh", arguments);


    //Is this stuff needed?
    while(bashScript->state() == QProcess::Running)
    {
        terminalOutput = bashScript->readAllStandardOutput();
    }

    connect(bashScript, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));

    qDebug() << "Script is running.\n" << bashScript->state();

}

void AppImage::terminateBash()
{
    qDebug() << "Terminate Bash run successfully " << QThread::currentThread();

    bashScript->kill();
}


void AppImage::writeCompileScript()
{
    QFile file(QString("%0/AppImageCompile.sh").arg(workingDirectory->absolutePath()));

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open compile script";
    }

    QTextStream stream(&file);

    if(!toolPath.isEmpty() && !fileName.isEmpty())
    {
        //Change the architecture to not be hard coded
        stream << "#!/bin/sh\n" << "\n" << "ARCH=x86_64 " << toolPath << QString(" %0/%1.AppDir").arg(workingDirectory->absolutePath(), fileName) << " > /home/hayden/Desktop/output.txt";
    }
    else
    {
        qDebug() << "Failed to create compile script. Tool Path or File Name is missing.";
    }


    file.close();
}

void AppImage::processOutput()
{
    qDebug() << "======================================================================= \n" << terminalOutput << "\n ====================================================";
}
