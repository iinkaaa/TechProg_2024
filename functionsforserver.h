#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QVariant>

class Func : public QObject
{
    Q_OBJECT
public:
    static QByteArray parsing(QString, QSqlQuery);
    static QByteArray auth(const QString&, const QString&, QSqlQuery);
    static QByteArray reg(const QString&, const QString&, const QString&, QSqlQuery);
    //QByteArray lookmystat(QString, QString);
public slots:
private:
};

#endif // FUNCTIONSFORSERVER_H
