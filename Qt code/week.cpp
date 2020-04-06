#include "week.h"
#include <QDebug>
#include <QThread>

int FLAG = 0;

Week::Week(QWidget* centralWidget)
{
    week = new QTabWidget(centralWidget);
    week->setObjectName(QStringLiteral("week"));
    week->setGeometry(QRect(20, 10, 671, 471));
    week->setAutoFillBackground(false);

    weeksMonday = getWeeksMonday(QDate::currentDate());

    configureSerialPort();

}

void Week::configureSerialPort() {
    serialPort = new QSerialPort();

    const QString serialPortName("COM6");
    serialPort->setPortName(serialPortName);
    const int serialPortBaudRate = 9600;
    serialPort->setBaudRate(serialPortBaudRate);

    if (!serialPort->open(QIODevice::ReadWrite)) {
        QString error_str = QString("Failed to open port %1, error: %2")
                .arg(serialPortName).arg(serialPort->errorString());
        qDebug() << error_str << endl;
        exit(EXIT_FAILURE);
    }
}


// TO DO: clean code!  (code is repeated)
QDate getWeekBefore(QDate currentMonday) {
    int days = currentMonday.day() - 7;
    if (days <= 0) {
        int months = currentMonday.month() - 1;
        if(months <= 0) {
            int year = currentMonday.year() - 1;
            months = 12;
            QDate newMonthDate(currentMonday.year(), months, 1);
            int daysInMonth = newMonthDate.daysInMonth();
            int newDayOfMonth = daysInMonth + days;  // days are negative
            return QDate(year, months, newDayOfMonth);
        }
        QDate newMonthDate(currentMonday.year(), months, 1);
        int daysInMonth = newMonthDate.daysInMonth();
        int newDayOfMonth = daysInMonth + days;  // days are negative
        QDate newDate(currentMonday.year(), months, newDayOfMonth);
        return newDate;
    }
    return QDate(currentMonday.year(), currentMonday.month(), days);
}

QDate getWeeksMonday(QDate currentDate) {
    int currentDayOfWeek = currentDate.dayOfWeek();
    int currentMonth = currentDate.month();
    int currentYear = currentDate.year();

    // week begins with monday: 1 (! not 0)
    if(currentDayOfWeek == 1) {
        return currentDate;
    }
    else {
        int dayOfCurrentMonth = currentDate.day();
        int newDayOfMonth = dayOfCurrentMonth - currentDayOfWeek + 1;
        if(newDayOfMonth <= 0) {
            int newMonth = currentMonth - 1;
            if(newMonth <= 0) {
                int year = currentDate.year() - 1;
                newMonth = 12;
                QDate newMonthDate(currentYear, newMonth, 1);
                int newMonthDays = newMonthDate.daysInMonth();
                newDayOfMonth = newMonthDays + newDayOfMonth; // newDayOfMonth is negative
                return QDate(year, newMonth, newDayOfMonth);
            }
            QDate newMonthDate(currentYear, newMonth, 1);
            int newMonthDays = newMonthDate.daysInMonth();
            newDayOfMonth = newMonthDays + newDayOfMonth; // newDayOfMonth is negative
            return QDate(currentYear, newMonth, newDayOfMonth);

        }
        else {
            return QDate(currentYear, currentMonth, newDayOfMonth);
        }
    }
}

void Week::clearWeekData() {
    for(int i=0; i<7; i++) {
        days[i]->clearDayData();
    }
}


void Week::loadWeek(QDate date) {
    QDate mondayDate = getWeeksMonday(date);

    for(int i=0; i<7; i++) {
        days[i] = new Day(mondayDate);
        week->addTab(days[i]->day, QString());
        week->setTabText(i, weekNames[i]);
        mondayDate = mondayDate.addDays(1);
    }

    //  open tab on current day of week
    int dayOfWeek = date.dayOfWeek();
    week->setCurrentIndex(dayOfWeek-1);

}


void Week::changeWeek(QDate date) {
    QDate mondayDate = getWeeksMonday(date);
    weeksMonday = mondayDate;

    for(int i=0; i<7; i++) {
        int aDay = mondayDate.day();
        int aMonth = mondayDate.month();
        int aYear = mondayDate.year();
        QString sDate = QString("%1.%2.%3").arg(aDay).arg(aMonth).arg(aYear);
        days[i]->date->setText(sDate);
        mondayDate = mondayDate.addDays(1);
    }

    week->setCurrentIndex(0);
}


void Week::synchro() {
    int currentIndex = week->currentIndex();
    qDebug() << "Synchro for: " << currentIndex << " index, " << this->days[currentIndex]->date->text();
    days[currentIndex]->sendDayInfo(serialPort);
}


