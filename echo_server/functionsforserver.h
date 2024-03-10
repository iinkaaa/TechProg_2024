#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>

class Func : public QObject
{
    Q_OBJECT
public:
    static QByteArray parsing(QString);
    static QByteArray auth(const QString&, const QString&);
    static QByteArray reg(const QString&, const QString&, const QString&);
    //QByteArray lookmystat(QString, QString);
public slots:
private:
};

#endif // FUNCTIONSFORSERVER_H
