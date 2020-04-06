#include "task.h"
#include <QDebug>

Task::Task(Day* day, int i) {
    description = new QTextEdit(day->day);

    int height = 40 + i*40;
    description->setGeometry(QRect(0, height, 621, 41));
    description->setTextColor(Qt::yellow);

    isDone = new QCheckBox(day->verticalLayoutWidget);
    isDone->setEnabled(false);

    (day->verticalLayout)->addWidget(isDone);

    connect(isDone, SIGNAL(stateChanged(int)), this, SLOT(changeBackground()));
    connect(description, SIGNAL(textChanged()), this, SLOT(changeTaskEnable()));
}

bool Task::isTaskEmpty() {
    if(getDescription() == "" && isStatusEmpty())
        return true;
    else
        return false;
}

bool Task::isStatusToDo() {
    if(isDone->isEnabled() && !isDone->isChecked())
        return true;
    else
        return false;
}

bool Task::isStatusDone() {
    if(isDone->isEnabled() && isDone->isChecked())
        return true;
    else
        return false;
}

bool Task::isStatusEmpty() {
    if(!isDone->isEnabled() && !isDone->isChecked())
        return true;
    else
        return false;
}


void Task::setStatusToDo() {
    isDone->setEnabled(true);
    isDone->setChecked(false);
    changeBackground();
}

void Task::setStatusDone() {
    isDone->setEnabled(true);
    isDone->setChecked(true);
    changeBackground();
}

void Task::setStatusEmpty() {
    isDone->setEnabled(false);
    isDone->setChecked(false);
    changeBackground();
}

void Task::changeBackground() {
    QPalette palette;
    palette.setColor(QPalette::Text, Qt::yellow);

    if(isStatusToDo()) {
        palette.setColor(QPalette::Base, Qt::darkRed);
        description->setPalette(palette);
    }
    else if(isStatusDone()) {
        palette.setColor(QPalette::Base, Qt::darkGreen);
        description->setPalette(palette);
    }
    else {
        palette.setColor(QPalette::Base, Qt::white);
        description->setPalette(palette);
    }
}

void Task::changeTaskEnable() {
    if(getDescription() == "") { // done/todo -> empty
        setStatusEmpty();
    }
    else if(!(isStatusToDo() || isStatusDone())) { // empty -> todo
        setStatusToDo();
    }
}


void Task::clearTask() {
    description->clear();
    setStatusEmpty();
}
