#include "QFileEx.h"
#include "qfileinfo.h"
#include "QDir"

bool QCopyFile(const QString &src, const QString &dst)
{
    if (src == dst) {
        return true;
    }
    if (!QFile::exists(src)) {
        return false;
    }
    if (QFile::exists(dst)) {
        QFile::remove(dst);
    }
    QFileInfo file(dst);
    QDir dir;

    dir.mkpath(file.absolutePath());

    return QFile::copy(src, dst);
}

// 将D:\fold 文件夹复制到 e:\trash文件夹下
// 将生成e:\trash\fold文件夹
bool QCopyDir(const QString &src_path, const QString &dst_path)
{
    QFileInfo file(src_path);
    QDir src_dir(src_path);
    QDir dst_dir(dst_path);
    QFileInfoList fs = src_dir.entryInfoList();

    QString r = dst_path + QDir::separator() + file.fileName();
    dst_dir.mkdir(r);

    for (int i = 0; i < fs.size(); ++i) {
        QFileInfo &info = fs[i];
        if (info.fileName() == "." || info.fileName() == "..") {
            continue;
        }

        if (info.isDir()) {
            QCopyDir(info.absoluteFilePath(), r);
        } else if (info.isFile()) {
            QCopyFile(info.absoluteFilePath(), r + QDir::separator() + info.fileName());
        }
    }
    return true;
}

bool QDeleteDir(const QString &path)
{
    QDir dir(path);
    QFileInfoList fs = dir.entryInfoList();

    for (int i = 0; i < fs.size(); ++i) {
        QFileInfo &info = fs[i];
        if (info.fileName() == "." || info.fileName() == "..") {
            continue;
        }

        if (info.isDir()) {
            QDeleteDir(info.absoluteFilePath());
        } else if (info.isFile()) {
            QFile::remove(info.absoluteFilePath());
        }
    }
    return dir.rmdir(path);
}
