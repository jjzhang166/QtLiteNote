#pragma once
#include "qfile.h"

// src, dst�����������ļ�·��
// ���罫 src=d:\1.txt �Ƶ�dst=e:\temp\2.txt
// ����Ѿ������ļ��򸲸������ļ�
// dst���ڵ��ļ��в��ش���
bool QCopyFile(const QString &src, const QString &dst);

//src_path, dst_path�����������ļ���·��
// ���罫 src=d:\dir �Ƶ� dst=e:\path
//����Ѿ������ļ�����ϲ�����
bool QCopyDir(const QString &src_path, const QString &dst_path);

//�ݹ�ɾ���ļ���
bool QDeleteDir(const QString &path);