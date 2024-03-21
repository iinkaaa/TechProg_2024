#ifndef DATABASE_H
#define DATABASE_H

#include <QCoreApplication>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QVariant>

class db : public QObject
{
    Q_OBJECT
public:
    int insert(QSqlQuery query, const QString login, const QString password);
    int select(QSqlQuery query);
public slots:
private:
};


#endif // DATABASE_H
