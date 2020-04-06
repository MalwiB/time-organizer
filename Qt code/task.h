#ifndef TASK_H
#define TASK_H

#include "QTextEdit"
#include "QCheckBox"
#include "QVBoxLayout"
#include "day.h"

class Day;

class Task : public QObject {
    Q_OBJECT

public:
    QTextEdit* description;
    QCheckBox* isDone;

    Task(Day* day, int i);

    bool isTaskEmpty();

    bool isStatusToDo();
    bool isStatusDone();
    bool isStatusEmpty();

    void setStatusToDo();
    void setStatusDone();
    void setStatusEmpty();

    QString getDescription() {return description->toPlainText();}

    void clearTask();

public slots:
    void changeBackground();

    void changeTaskEnable();

};

#endif // TASK_H
