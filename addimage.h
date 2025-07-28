#ifndef ADDIMAGE_H
#define ADDIMAGE_H

#include <QWidget>

class QPushButton;
class QGridLayout;
class QLineEdit;
class QLabel;
class AppImage;
class FileWatcher;

class AddImage : public QWidget
{
    Q_OBJECT
public:
    explicit AddImage(QWidget *parent = nullptr);

public slots:
    void openBinDialog();
    void openIconDialog();
    void addImage();
    void updateOutputText();
    void runningQuery();

signals:

private:
    QPushButton* addButton;
    QPushButton* binPathButton;
    QPushButton* iconPathButton;
    QPushButton* terminateButton;

    QGridLayout* layout;

    QLineEdit* nameEdit;
    QLineEdit* binPathEdit;
    QLineEdit* iconPathEdit;
    QLineEdit* categoryEdit;

    QLabel* nameLabel;
    QLabel* binPathLabel;
    QLabel* iconPathLabel;
    QLabel* categoryLabel;
    QLabel* outputLabel;

    QString fileName;
    FileWatcher* watcher;
};

#endif // ADDIMAGE_H
