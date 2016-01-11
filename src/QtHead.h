#pragma once

#if defined(WIN32)
#include "QtCore/QtCore"
#include "QtGui/QtGui"
#include "QtWebKit/QtWebKit"

#elif defined(MAC)
#import <QtCore/QtCore>
#import <QtGui/QtGui>
#import <QtWebkit/QtWebkit>
#endif

//#ifdef Q_OS_WIN32
//#elif defined(Q_OS_MAC)