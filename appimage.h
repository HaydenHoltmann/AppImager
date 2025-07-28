#ifndef APPIMAGE_H
#define APPIMAGE_H

#include <QString>
#include <QObject>

class QDir;
class QProcess;

class AppImage : public QObject
{
    Q_OBJECT
public:
    AppImage();
    ~AppImage();
    void create(QString imageName, QString binPath, QString iconPath, QString category);
    void imageFolder();
    void binFolder(QString binPath);
    void moveIcon(QString iconPath);
    void desktopConfig(QString category);
    void runScript();
    void writeCompileScript();

public slots:
    void processOutput();
    void runImageTool();
    void terminateBash();

private:
    QDir* homeDirectory;
    QDir* workingDirectory;
    QString imageName;
    QString fileName;
    QString toolPath;
    QString terminalOutput;
    QProcess* bashScript;
};

#endif // APPIMAGE_H
