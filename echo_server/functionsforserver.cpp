

#include "functionsforserver.h"
#include <QDebug>
#include <QCoreApplication>
#include<QString>

QByteArray Func::reg(const QString& username, const QString& password, const QString& confirmPassword)
{
    if (password == confirmPassword) {
        qDebug() << "Registered user: " << username << " with password: " << password;
        return QByteArray("Registration successful");
    } else {
        qDebug() << "Passwords do not match!";
        return QByteArray("Registration failed: passwords do not match");
    }
}

QByteArray Func::auth(const QString& login, const QString& password)
{
    const QString correctLogin = "admin";
    const QString correctPassword = "password";

    if (login == correctLogin && password == correctPassword) {
        return QByteArray("Authorization successful");
    } else {
        return QByteArray("Authorization failed");
    }
}

QByteArray Func::parsing (QString data_from_client){
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
         return auth(data_from_client_list.at(0), data_from_client_list.at(1));
     else if (nameOfFunc == "reg")
         return reg(data_from_client_list.at(0), data_from_client_list.at(1), data_from_client_list.at(2));
     else
         return "error";

}


