#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QStringList>

class FileWatcher : public QObject
{
    Q_OBJECT
public:
    explicit FileWatcher(QObject *parent = nullptr);
    QStringList getOutput();

public slots:
    void watchFile();
    void run();

signals:
    void watchFileChanged();

private:
    QStringList output;
};

#endif // FILEWATCHER_H
