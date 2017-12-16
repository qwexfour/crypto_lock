#-------------------------------------------------
#
# Project created by QtCreator 2017-12-08T18:31:36
#
#-------------------------------------------------

QT       += core gui \
         network \
         bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cryptolock
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        lib/RSA.c \
        lib/msglib.c \
        lib/longdiv.c

HEADERS += \
        mainwindow.h \
        lib/RSA.h \
        lib/msglib.h \
        lib/consts.h \
        lib/longdiv.h

FORMS += \
        mainwindow.ui

CONFIG += mobility
# c11
MOBILITY = connectivity


#unix:!macx: LIBS += -L$$PWD/../lib/ -ltest_parser

#INCLUDEPATH += $$PWD/../
#DEPENDPATH += $$PWD/../

#unix:!macx: PRE_TARGETDEPS += $$PWD/../lib/libtest_parser.a


#INCLUDEPATH += $$PWD/../
#DEPENDPATH += $$PWD/../

# msglib
#unix:!macx:
#LIBS += -L$$PWD/../lib/ -lmsglib
#unix:!macx:
#PRE_TARGETDEPS += $$PWD/../lib/libmsglib.a

#longdiv
#unix:!macx:
#LIBS += -L$$PWD/../lib/ -llongdiv
#unix:!macx:
#PRE_TARGETDEPS += $$PWD/../lib/liblongdiv.a

# RSA
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/release/ -lRSA
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/debug/ -lRSA
#else:unix:
#LIBS += -L$$PWD/../lib/ -lRSA
#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/release/libRSA.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/debug/libRSA.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/release/RSA.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/debug/RSA.lib
#else:unix:
#PRE_TARGETDEPS += $$PWD/../lib/libRSA.a
