#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

class DatabaseManager {
public:
    DatabaseManager() {
        // Инициализация базы данных
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("users.db");

        // Открываем базу данных
        if (!db.open()) {
            qDebug() << "Error while open database" << db.lastError().text();
        } else {
            qDebug() << "Database open";
        }

        // Создаем таблицу пользователей
        createTable();
        createLeaderboardTable();
        // clearUsersTable();
    }

    ~DatabaseManager() {
        // Закрываем базу данных при уничтожении объекта
        db.close();
    }

    bool addUser(const QString& login, const QString& password) {
        // Проверяем, существует ли пользователь с таким логином
        if (userExists(login)) {
            qDebug() << "Username already take";
            return false;
        }

        // Добавляем пользователя в таблицу
        QSqlQuery query;
        query.prepare("INSERT INTO users (login, password) VALUES (:login, :password)");
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        if (!query.exec()) {
            qDebug() << "Error add user" << query.lastError().text();
                return false;
        }

//        qDebug() << "Пользователь добавлен успешно";
        return true;
    }
    bool userExists(const QString& login) {
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM users WHERE login = :login");
        query.bindValue(":login", login);
        if (!query.exec()) {
                qDebug() << query.lastError().text();
                    return false;
        }
        query.next();
        int count = query.value(0).toInt();
        return count > 0;
    }

    bool checkCredentials(const QString& login, const QString& password) {
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM users WHERE login = :login AND password = :password");
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        if (!query.exec()) {
                    qDebug() << query.lastError().text();
                        return false;
        }
        query.next();
        int count = query.value(0).toInt();
        return count > 0;
    }


    bool createLeaderboardTable() {

        QSqlQuery query(db);
        QString createTableQuery =
            "CREATE TABLE IF NOT EXISTS leaderboard ("
            "login TEXT PRIMARY KEY,"
            "record INTEGER NOT NULL DEFAULT 0"
            ");";

        if (!query.exec(createTableQuery)) {
            qDebug() << "Error creating leaderboard table:" << query.lastError();
            return false;
        }

        return true;
    }
    bool insertOrUpdateLeaderboard(const QString& login, int record) {


        QSqlQuery query;

        QString insertOrUpdateQuery =
            "INSERT INTO leaderboard (login, record) VALUES (:login, :record) "
            "ON CONFLICT(login) DO UPDATE SET record = :record;";

        query.prepare(insertOrUpdateQuery);
        query.bindValue(":login", login);
        query.bindValue(":record", record);

        if (!query.exec()) {
            qDebug() << "Error inserting or updating leaderboard:" << query.lastError();
            return false;
        }

        return true;
    }

    QList<QPair<QString, int>> getLeaderboard() {
        QList<QPair<QString, int>> leaderboard;

        QSqlQuery query;
        QString selectQuery = "SELECT login, record FROM leaderboard ORDER BY record DESC;";

        if (!query.exec(selectQuery)) {
            qDebug() << "Error retrieving leaderboard:" << query.lastError();
            return leaderboard;
        }

        while (query.next()) {
            QString login = query.value(0).toString();
            int record = query.value(1).toInt();
            leaderboard.append(qMakePair(login, record));
        }

        return leaderboard;
    }


    int getRecordByLogin(const QString& login) {

        QSqlQuery query;
        QString selectQuery = "SELECT record FROM leaderboard WHERE login = :login;";

        query.prepare(selectQuery);
        query.bindValue(":login", login);

        if (!query.exec()) {
            qDebug() << "Error retrieving record by login:" << query.lastError();
            return 0;
        }

        if (query.next()) {
            return query.value(0).toInt();
        } else {
            qDebug() << "No record found for login:" << login;
            return 0;
        }
    }

private:
    QSqlDatabase db;

    void createTable() {
        QSqlQuery query;
        if (!query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT UNIQUE, password TEXT)")) {
            qDebug() << query.lastError().text();
        }
    }

    bool clearUsersTable() {
        QSqlQuery query;
        if (!query.exec("DELETE FROM users")) {
            qDebug() << query.lastError().text();
                return false;
        }
        return true;
    }
};

#endif // DATABASE_H
