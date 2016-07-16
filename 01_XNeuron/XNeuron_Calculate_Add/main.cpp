#include <QCoreApplication>
#include "../XNeuron/gradienttraining.h"
#include "QDateTime"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GradientTraining XNADD;

    QList<QList<double>> i;
    QList<double> o;
    XNADD.setWeight(0, 1.5, 0.3, 2);

    i.append(QList<double>() << 1.0 << 1.5 << 2.0);
    i.append(QList<double>() << 3.5 << 2.8 << 3.25);
    i.append(QList<double>() << 15.25 << 14 << 100);
    o.append(1.0 + 1.5 + 2.0);
    o.append(3.5 + 2.8 + 3.25);
    o.append(15.25 + 14 + 100);

    QDateTime start = QDateTime::currentDateTime();
    qDebug() << "start time: " << start << "\n";
    XNADD.train(i, o, ActivityFunction::Line);
    QDateTime end = QDateTime::currentDateTime();
    qDebug() << "end time: " << start << "\n";
    qDebug() << "diff time: " << start.msecsTo(end) << "\n";


    return a.exec();
}
