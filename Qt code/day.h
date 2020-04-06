#ifndef DAY_H
#define DAY_H

#include "QWidget"
#include "QLabel"
#include "QVBoxLayout"
#include "week.h"
#include "task.h"
#include <QSerialPort>

class Task;

class Week;

class Day {
public:
    QWidget* day;
    QLabel* date;
    Task* tasks[10];

    QWidget *layoutWidget;
    QWidget* verticalLayoutWidget;
    QVBoxLayout *verticalLayout;


public:
    Day(QDate date);

    void clearDayData();

    void sendDayInfo(QSerialPort* port);

    int writeState(QSerialPort* serialPort, const char* ledNumber, const char* state);
};

#endif // DAY_H
