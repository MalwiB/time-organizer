#include "databasefunctions.h"

void connectDatabase() {
    QSqlDatabase tasksDatabase;
    tasksDatabase = QSqlDatabase::addDatabase("QSQLITE");
    tasksDatabase.setDatabaseName("tasks.db");

    if(! tasksDatabase.open()) {
        qDebug() << "Connect database: database cannot be opened.";
        qDebug() << tasksDatabase.lastError();
        exit(EXIT_FAILURE);
    }

    QSqlQuery createTableQuery;
    QString createTableQ = "CREATE TABLE IF NOT EXISTS tasks ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "day VARCHAR(10) NOT NULL,"
                "number INTEGER NOT NULL,"
                "description VARCHAR(100) NOT NULL,"
                "isToDo INTEGER NOT NULL"
                ");";

    qDebug() << "CREATE TABLE IF NOT EXISTS tasks ...";
    if(! createTableQuery.exec(createTableQ)) {
        qDebug() << createTableQuery.lastError();
        exit(EXIT_FAILURE);
    }
}


void insertIntoTable(QString description_inProg, QString isToDo_inProg, QString date, int number) {
    QSqlQuery insertQuery;
    QString insertQ = "INSERT INTO tasks (day, number, description, isToDo) VALUES (\'";
    insertQ.append(date);
    insertQ.append("\', \'");
    insertQ.append(QString::number(number));
    insertQ.append("\', \'");
    insertQ.append(description_inProg);
    insertQ.append("\', \'");
    insertQ.append(isToDo_inProg);
    insertQ.append("\');");

    qDebug() << insertQ;
    if(! insertQuery.exec(insertQ)) {
        qDebug() << insertQuery.lastError();
        exit(EXIT_FAILURE);
    }
}


void updateTaskInTable(QString description_inProg, QString isToDo_inProg, QString date, int number) {
    QSqlQuery updateQuery;
    QString updateQ = "UPDATE tasks SET description=\'";
    updateQ.append(description_inProg);
    updateQ.append("\', isToDo=");
    updateQ.append(isToDo_inProg);
    updateQ.append(" WHERE day=\'");
    updateQ.append(date);
    updateQ.append("\' AND number=");
    updateQ.append(QString::number(number));
    updateQ.append(";");

    qDebug() << updateQ;
    if(! updateQuery.exec(updateQ)) {
        qDebug() << updateQuery.lastError();
        exit(EXIT_FAILURE);
    }
}

void deleteTaskFromTable(QString date, int number) {
    QSqlQuery deleteQuery;
    QString deleteQ = "DELETE FROM tasks WHERE day=\'";
    deleteQ.append(date);
    deleteQ.append("\' AND number=");
    deleteQ.append(QString::number(number));
    deleteQ.append(";");

    qDebug() << deleteQ;
    if(! deleteQuery.exec(deleteQ)) {
        qDebug() << deleteQuery.lastError();
        exit(EXIT_FAILURE);
    }
}

QSqlQuery selectTaskFromTable(QString date, int number) {
    QSqlQuery selectQuery;
    QString selectQ = "SELECT * FROM tasks WHERE day=\'";
    selectQ.append(date);
    selectQ.append("\' AND number=");
    selectQ.append(QString::number(number));
    selectQ.append(";");

    if(! selectQuery.exec(selectQ)) {
        qDebug() << selectQ;
        qDebug() << selectQuery.lastError();
        exit(EXIT_FAILURE);
    }

    return selectQuery;
}

void saveWeek(Week* week) {
    for(int i=0; i<7; i++) {
        Day* day = week->days[i];
        QString date = day->date->text();

        for(int task_number=0; task_number <10; task_number++) {
            Task* task = day->tasks[task_number];

            QString description_inProg = task->getDescription();
            QString isToDo_inProg;  // 1 is ToDo task, 0 is NONE/DONE task
            if(task->isStatusToDo())
                isToDo_inProg = "1";
            else
                isToDo_inProg = "0";

            QSqlQuery selectQuery = selectTaskFromTable(date, task_number);
            if(selectQuery.next()) {    // if task already is in table
                QString description_inDb = selectQuery.value("description").toString();
                QString isToDo_inDb = selectQuery.value("isToDo").toString();

                //  if tasks in PROGR and DB differ
                if(!(description_inDb == description_inProg && isToDo_inDb == isToDo_inProg)) {
                    if(task->isTaskEmpty())   // if task is empty in program - DELETE
                        deleteTaskFromTable(date, task_number);
                    else    // task data had changed - UPDATE
                        updateTaskInTable(description_inProg, isToDo_inProg, date, task_number);
                }
            }
            else {  // if task is not in table yet and is not empty - INSERT
                if(! task->isTaskEmpty())
                    insertIntoTable(description_inProg, isToDo_inProg, date, task_number);
            }
        }
    }
}


void readWeek(Week* week) {
    for(int i=0; i<7; i++) {
        Day* day = week->days[i];
        QString date = day->date->text();

        for(int task_number=0; task_number <10; task_number++) {
            Task* task = day->tasks[task_number];

            QSqlQuery selectQuery = selectTaskFromTable(date, task_number);

            if(selectQuery.next()) {
                QString description_inDb = selectQuery.value("description").toString();
                task->description->setText(description_inDb);

                bool isToDo_inDb = selectQuery.value("isToDo").toBool();
                if(isToDo_inDb)
                    task->setStatusToDo();
                else
                    task->setStatusDone();
            }
        }
    }
}
