#include "functionsforserver.h"
#include <QDebug>
#include <QCoreApplication>
#include<QString>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QVariant>


QByteArray Func::reg(const QString& username, const QString& password, const QString& confirmPassword, QSqlQuery query)
{
    if (password == confirmPassword) {
        qDebug() << "Registered user: " << username << " with password: " << password;
        query.prepare("INSERT INTO User(login, password) VALUES (:login, :password)");
        query.bindValue(":password", password);
        query.bindValue(":login", username);
        return QByteArray("Registration successful");
    } else {
        qDebug() << "Passwords do not match!";
        return QByteArray("Registration failed: passwords do not match");
    }
}

QByteArray Func::auth(const QString& login, const QString& password, QSqlQuery query)
{
    const QString correctLogin = "admin";
    const QString correctPassword = "password";

    if (login == correctLogin && password == correctPassword) {
        return QByteArray("Authorization successful");
    } else {
        return QByteArray("Authorization failed");
    }
}

QByteArray Func::parsing (QString data_from_client, QSqlQuery query){
    /* data_from_client = NameOfFunc&Param1&Param2&Param3
     * Цель парсинга:
     * return NameOfFunc(Param1, Param2, Param3)
     */
    data_from_client.chop(2);
    QStringList data_from_client_list = data_from_client.split(QLatin1Char('&'));
    // data_from_client_list = "NameOfFunc" "Param1" "Param2" "Param3"
    QString nameOfFunc = data_from_client_list.front();
    data_from_client_list.pop_front();
    if (nameOfFunc == "auth")
        return auth(data_from_client_list.at(0), data_from_client_list.at(1), query);
    else if (nameOfFunc == "reg")
        return reg(data_from_client_list.at(0), data_from_client_list.at(1), data_from_client_list.at(2), query);
    else
        return "";

}

