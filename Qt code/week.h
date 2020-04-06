#ifndef WEEK_H
#define WEEK_H

#include "iostream"
#include "QString"
#include "QWidget"
#include "QTabWidget"
#include "QDate"
#include <QSerialPort>
#include "day.h" //<---- it's not working

// only this is working:
class Day;

QDate getWeeksMonday(QDate currentDate);

QDate getWeekBefore(QDate currentMonday);

class Week {

public:
    QTabWidget* week;
    QDate weeksMonday;
    Day* days[7];
    QString weekNames[7] = {"Monday   ",
                           "Tuesday  ",
                           "Wednesday",
                           "Thursday ",
                           "Friday   ",
                           "Saturday ",
                           "Sunday   "};

    QSerialPort* serialPort;

    Week(QWidget* centralWidget);

    void clearWeekData();

    void loadWeek(QDate date);

    void changeWeek(QDate date);

    void configureSerialPort();

    void synchro();

};

#endif // WEEK_H
