#include "dialog.h"
#include "ui_dialog.h"

#include <QDir>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QObject::tr("计算备份文件所需的空间"));

    num = 0;

    // /opt/qt-arm/bin
    // /root/am335x/shell
    // /root/am335x/exploringBB/chp02
    // /opt/temp

    const QString path = "/usr/sbin";

    //qDebug() << QString("%1 目录大小 %2").arg(path).arg(dirFileSize(path));
    //qDebug() << QString("files : %1").arg(num);

    //qDebug() << QString("%1 目录大小 %2").arg(path).arg(du(path));

    // /opt/temp
    // /usr/sbin
    qDebug() << QString("%1 目录大小 %2").arg(path).arg(GetDirectorySize("/usr/sbin"));
}

Dialog::~Dialog()
{
    delete ui;
}
#if 1
qint64 Dialog::du(const QString &path)
{
    QDir dir(path);
    qint64 size = 0;
    //遍历文件
    foreach(QFileInfo fileInfo,dir.entryInfoList(QDir::Files))
    {
        size += fileInfo.size();
    }
    //遍历文件夹
    foreach(QString subDir,dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot))
    {
        size += du(path+QDir::separator()+subDir);
    }
    char unit ='B';
    qint64 curSize=size;
    if(curSize>1024){
        curSize/=1024;
        unit ='K';
        if(curSize>1024){
            curSize/=1024;
            unit ='M';
            if(curSize>1024){
                curSize/=1024;
                unit ='G';
            }
        }
    }
    qDebug()<<curSize<<unit<<"\t"<<qPrintable(path)<<endl;
    return size;
}
#endif
quint64 Dialog::dirFileSize(const QString &path)
{
    QDir dir(path);
    quint64 size = 0;
    //dir.entryInfoList(QDir::Files)返回文件信息
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
    //计算文件大小
    size += fileInfo.size();
    qDebug() << QString("%1 %2").arg(size).arg(fileInfo.fileName());
    num++;
    }
    //dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)返回所有子目录，并进行过滤
    foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
    //若存在子目录，则递归调用dirFileSize()函数
    size += dirFileSize(path + QDir::separator() + subDir);
    }
    return size;
}

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

//计算某目录所占空间大小（包含本身的4096Byte）
long long Dialog::GetDirectorySize(const char *dir)
{
    DIR *dp = NULL;
    struct dirent *entry;
    struct stat statbuf;
    long long int totalSize=0;

    if ((dp = opendir(dir)) == NULL)
    {
        //fprintf(stderr, "Cannot open dir: %s\n", dir);
        return -1; //可能是个文件，或者目录不存在
    }

    //先加上自身目录的大小
    lstat(dir, &statbuf);
    totalSize+=statbuf.st_size;

    while ((entry = readdir(dp)) != NULL)
    {
        char subdir[256];
        //sprintf(subdir, "%s/%s", dir, entry->d_name);
        // snprintf 所需头文件 stdio.h
        snprintf(subdir, 256, "%s/%s", dir, entry->d_name);
        lstat(subdir, &statbuf);

        if (S_ISDIR(statbuf.st_mode))
        {
            // strcmp string.h
            if (strcmp(".", entry->d_name) == 0 ||
                strcmp("..", entry->d_name) == 0)
            {
                continue;
            }

            long long int subDirSize = GetDirectorySize(subdir);
            totalSize+=subDirSize;
        }
        else
        {
            totalSize+=statbuf.st_size;
        }
    }

    closedir(dp);
    dp = NULL;
    return totalSize;
}
