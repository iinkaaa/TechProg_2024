#include <QCoreApplication>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QVariant>

/**int db(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //создаем базу данных типа qsqlite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Test.db");

    if(!db.open())
        qDebug()<<db.lastError().text();

    //добавить запись путем sql запроса создать запрос к бд

    QSqlQuery query(db);
    query.exec("CREATE TABLE User(""login VARCHAR(30) NOT NULL, " "password VARCHAR(30) NOT NULL"")");

    return 0;
}**/

