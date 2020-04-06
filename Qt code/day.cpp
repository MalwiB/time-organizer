#include "day.h"
#include <QDebug>
#include <QByteArray>
#include <QThread>

class Week;

Day::Day(QDate actualDate) {

    day = new QWidget;

    int indexOfDay = actualDate.dayOfWeek() - 1;

    date = new QLabel(day);
    date->setGeometry(QRect(5 + indexOfDay*87, 0, 71, 20));

    int aDay = actualDate.day();
    int aMonth = actualDate.month();
    int aYear = actualDate.year();
    QString sDate = QString("%1.%2.%3").arg(aDay).arg(aMonth).arg(aYear);

    date->setText(sDate);

    verticalLayoutWidget = new QWidget(day);
    verticalLayoutWidget->setGeometry(QRect(620, 40, 20, 401));

    verticalLayout = new QVBoxLayout(verticalLayoutWidget);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    for(int i=0; i<10; i++) {
        tasks[i] = new Task(this, i);
    }
}


void Day::clearDayData() {
    date->clear();
    for(int i=0; i<10; i++) {
        tasks[i]->clearTask();
    }
}


int Day::writeState(QSerialPort* serialPort, const char* ledNumber, const char* state) {
    int bytesSum = 0;

    qDebug() << "ledNumber: " << ledNumber;
    bytesSum += serialPort->write(ledNumber);

    qDebug() << "state: " << state;
    bytesSum += serialPort->write(state);

    while(serialPort->waitForBytesWritten(3000));

    return bytesSum;
}



void Day::sendDayInfo(QSerialPort* serialPort) {
    for(int i=0; i<10; i++) {
        std::string ledNumber_str = std::to_string(i);
        std::string state_str;
        if(tasks[i]->isDone->isEnabled() && !tasks[i]->isDone->isChecked())
            state_str = "1";
        else
            state_str = "0";
        const char* ledNumber_c = ledNumber_str.c_str();
        const char* state_c = state_str.c_str();

        writeState(serialPort, ledNumber_c, state_c);

        QThread::sleep(2);
    }
}

