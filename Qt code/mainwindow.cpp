#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

MainWindow::MainWindow()
{
    resize(735, 600);
    setWindowTitle("Time management system");

    centralWidget = new QWidget(this);

    previousWeek = new QPushButton("previousWeek", centralWidget);
    previousWeek->setGeometry(QRect(20, 500, 101, 28));

    nextWeek = new QPushButton(centralWidget);
    nextWeek->setGeometry(QRect(130, 500, 101, 28));

    synchroButton = new QPushButton("SYNCHRO", centralWidget);
    synchroButton->setGeometry(QRect(240, 500, 101, 28));

    previousWeek->setText("Previous Week");
    nextWeek->setText("Next Week");

    setCentralWidget(centralWidget);

    week = new Week(centralWidget);

    QDate currentDate = QDate::currentDate();

    // create actual week
    week->loadWeek(currentDate);

    // connect database
    connectDatabase();


    // read week from database
    readWeek(week);

    week->synchro();

    // SIGNAL, SLOTS
    connect(previousWeek, SIGNAL(clicked()), this, SLOT(toPreviousWeek()));
    connect(nextWeek, SIGNAL(clicked()), this, SLOT(toNextWeek()));
    connect(synchroButton, SIGNAL(clicked()), this, SLOT(synchro()));
}

MainWindow::~MainWindow() {
    saveWeek(week);
}


void MainWindow::synchro() {
        week->synchro();
}

void MainWindow::toPreviousWeek() {
    saveWeek(week);

    week->clearWeekData();

    QDate previousDate = getWeekBefore(week->weeksMonday);
    week->weeksMonday = previousDate;

    week->changeWeek(previousDate);

    readWeek(week);

    week->synchro();
}


void MainWindow::toNextWeek() {
    saveWeek(week);

    week->clearWeekData();

    QDate currentDate = week->weeksMonday;
    QDate nextDate = currentDate.addDays(7);
    week->weeksMonday = nextDate;

    week->changeWeek(nextDate);

    readWeek(week);

    week->synchro();
}



