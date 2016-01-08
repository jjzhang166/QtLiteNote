#pragma once

#include "QtHead.h"


// src, dst都是完整的文件路径
// 比如将 src=d:\1.txt 移到dst=e:\temp\2.txt
// 如果已经存在文件则覆盖已有文件
// dst所在的文件夹不必存在
bool QCopyFile(const QString &src, const QString &dst);

//src_path, dst_path都是完整的文件夹路径
// 比如将 src=d:\dir 移到 dst=e:\path
//如果已经存在文件夹则合并起来
bool QCopyDir(const QString &src_path, const QString &dst_path);

//递归删除文件夹
bool QDeleteDir(const QString &path);
