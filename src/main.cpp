#include "QRabbitmqMgr.h"
#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QDebug>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AMQP_QT::MqInfo mq;
    mq.ip = "192.168.0.100";
	mq.port = 5672;
    mq.loginName = "admin";
    mq.loginPwd = "admin";
    mq.exchangeName = "";   //hanglok.event.ex
    mq.queueName = "test.only.queue";
    mq.vhost = "Hanglok";

    AMQP_QT::QRabbitmqMgr rmq(mq, AMQP_QT::QRabbitmqMgr::MqPublisher, 60);
    while(!rmq.StartMqInstance()) {
        qDebug() << "Error, StartMqInstance," << rmq.getErrorMessage();
        QThread::sleep(5);
    }

    AMQP_QT::QRabbitmqMgr::connect(&rmq, &AMQP_QT::QRabbitmqMgr::sigRecvedDataReady, [](const QByteArray& data){
        qDebug() << "-----test, Consumer get msg:" << data;
    });


    QTimer::singleShot(1000*2, [&rmq](){
        // clear queue
        // rmq.PurgeMsgQueue();
        // receieve message
        if(!rmq.StartConsumeMsg()) {
            qDebug() << "Error, StartConsumeMsg," << rmq.getErrorMessage();
        }
        // send message
        if(!rmq.PublishMsg("I love you")) {
            qDebug() << "Error, PublishMsg," << rmq.getErrorMessage();
        }
    });

    return a.exec();
}
