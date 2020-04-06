#ifndef DATABASEFUNCTIONS_H
#define DATABASEFUNCTIONS_H

#include <QtSql/QtSql>
#include <QDate>
#include <QString>
#include <QFile>
#include "week.h"
#include "day.h"
#include "task.h"


void connectDatabase();

void saveWeek(Week* week);

void readWeek(Week* week);


#endif // DATABASEFUNCTIONS_H
