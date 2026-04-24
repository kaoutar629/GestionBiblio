#ifndef MODELS_H_INCLUDED
#define MODELS_H_INCLUDED
#include <QString>
#include <QDate>

struct Client {
    int id = 0;
    QString name;
    QString cin;
    QString phone;
    QString email;
};

struct Vehicle {
    int id = 0;
    QString brand;
    QString model;
    QString plate;
    bool available = true;
    double dailyRate = 0.0;
};

struct Reservation {
    int id = 0;
    int clientId = 0;
    int vehicleId = 0;
    QDate startDate;
    QDate endDate;
    QString status;
};
struct User {
    int id = 0 ;
    QString username;
    QString password;
};

#endif
