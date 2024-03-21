
#include "mytcpserver.h"
#include "functionsforserver.h"
#include "functionsforserver.cpp"
#include "dataBase.h"
#include "database.cpp"
#include <QDebug>
#include <QCoreApplication>
#include <QString>

MyTcpServer::~MyTcpServer()
{

    mTcpServer->close();
    //server_status=0;
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    // DataBase::getInstance();
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        //server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    //   if(server_status==1){
    mTcpSocket = mTcpServer->nextPendingConnection();
    mTcpSocket->write("Hello, World!!! I am echo server!\r\n");
    connect(mTcpSocket, &QTcpSocket::readyRead,this,&MyTcpServer::slotServerRead);
    connect(mTcpSocket,&QTcpSocket::disconnected,this,&MyTcpServer::slotClientDisconnected);
    qDebug()<<"New coonection\n";
    // }
}

/*void MyTcpServer::db(){
    //QCoreApplication app(argc, argv);
    //создаем базу данных типа qsqlite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydb.sqlite");

    if(!db.open())
        qDebug()<<db.lastError().text();

    //добавить запись путем sql запроса создать запрос к бд

    QSqlQuery query(db);
    query.exec("CREATE TABLE User(""login VARCHAR(30) NOT NULL, " "password VARCHAR(30) NOT NULL"")");

    //db.close();
    //a.exec();

    return;
}*/

void MyTcpServer::slotServerRead(){
    //QCoreApplication app(argc, argv);
    //создаем базу данных типа qsqlite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydb.sqlite");
    if(!db.open())
        qDebug()<<db.lastError().text();
    //добавить запись путем sql запроса создать запрос к бд
    QSqlQuery query(db);
    query.exec("CREATE TABLE User(""login VARCHAR(30) NOT NULL, " "password VARCHAR(30) NOT NULL"")");
    //db.close();
    //a.exec();


    QString res = "";
    while(mTcpSocket->bytesAvailable()>0)
    {
        QByteArray array =mTcpSocket->readAll();
        res.append(array);
    }
    qDebug()<<res;
    Func func;
    mTcpSocket->write(Func::parsing(res, query));
    query.exec("SELECT * FROM User");

    db.close();
}

void MyTcpServer::slotClientDisconnected(){
    mTcpSocket->close();
}

