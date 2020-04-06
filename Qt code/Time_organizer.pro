#-------------------------------------------------
#
# Project created by QtCreator 2017-11-13T13:58:35
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Time_management_system
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    task.cpp \
    day.cpp \
    week.cpp \
    databasefunctions.cpp

HEADERS  += mainwindow.h \
    day.h \
    task.h \
    week.h \
    databasefunctions.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../Qt/5.7/msvc2015_64/lib/ -lQt5Sql
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../Qt/5.7/msvc2015_64/lib/ -lQt5Sqld
else:unix: LIBS += -L$$PWD/../../../../../../Qt/5.7/msvc2015_64/lib/ -lQt5Sql

INCLUDEPATH += $$PWD/../../../../../../Qt/5.7/msvc2015_64/include/QtSql
DEPENDPATH += $$PWD/../../../../../../Qt/5.7/msvc2015_64/include/QtSql
