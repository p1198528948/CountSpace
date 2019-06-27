#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    quint64 num;

    qint64 du(const QString &path);
    quint64 dirFileSize(const QString &path);

    long long int GetDirectorySize(const char *dir);
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
