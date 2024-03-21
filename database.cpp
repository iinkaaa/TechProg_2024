#include "dataBase.h"
#include <QCoreApplication>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QVariant>

int db::insert(QSqlQuery query, const QString login, const QString password){
    //добавить запись в таблицу
    //*query.prepare("insert into user(login, password) values (\"admin\", \"123\")");
    query.prepare("INSERT INTO User(login, password) VALUES (:login, :password)");
    query.bindValue(":password", password);
    query.bindValue(":login", login);
    return 0;
}


int db::select(QSqlQuery query){
    //выбрать все записи бд
    query.exec("SELECT * FROM User");

    //вывести на экран
    QSqlRecord rec = query.record();
    const int loginIndex = rec.indexOf("login");
    const int passwordIndex = rec.indexOf("password");

    while(query.next())
        qDebug()<<query.value(loginIndex).toString()<<"\t"<<query.value(passwordIndex)<<"\n";
    return 0;
}


