#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QtSql/QtSql>
#include "QDate"
#include "week.h"
#include "databasefunctions.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    QWidget *centralWidget;
    QPushButton *previousWeek;
    QPushButton *nextWeek;
    QPushButton *synchroButton;
    Week* week;

    QSqlDatabase tasksDb;

public:
    MainWindow();
    ~MainWindow();


public slots:
    void toPreviousWeek();
    void toNextWeek();
    void synchro();

};

#endif // MAINWINDOW_H
