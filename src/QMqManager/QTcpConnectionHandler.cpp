#include "QTcpConnectionHandler.h"
#include <QDebug>
#include <QtGlobal>
#include "QTcpClient.h"


using namespace std;

namespace AMQP_QT {

QTcpConnectionHandler::QTcpConnectionHandler(shared_ptr<QTcpClient> pTcpClient)
    : QObject(nullptr), m_pTcpClient(pTcpClient)
{
    //
}

QTcpConnectionHandler::~QTcpConnectionHandler()
{
    //
}

void QTcpConnectionHandler::setHeartbeatInterval(int interval)
{
    m_heartbeatInterval = interval;
}

uint16_t QTcpConnectionHandler::getHeartbeatInterval() const
{
    return m_heartbeatInterval;
}

void QTcpConnectionHandler::onData(AMQP::Connection *connection, const char *data, size_t size)
{
    Q_UNUSED(connection)

    try {
        if (!m_pTcpClient) {
            return;
        }

        QByteArray msg(data, size);
        //qDebug() << "QTcpConnectionHandler::onData, send:" << msg;
        m_pTcpClient->SendData(msg);
    }
    catch (const std::exception&e) {
        m_pTcpClient->OnErrMsg("To MqServer, Send Data Failed: " + QString(e.what()));
    }
}

void QTcpConnectionHandler::onReady(AMQP::Connection *connection)
{
    Q_UNUSED(connection)

    qDebug() << "QTcpConnectionHandler::onReady, OK.";
}

void QTcpConnectionHandler::onError(AMQP::Connection *connection, const char *message)
{
    Q_UNUSED(connection)

    m_pTcpClient->OnErrMsg("RabbitMq Failed: " + QString(message));
}

void QTcpConnectionHandler::onClosed(AMQP::Connection *connection)
{
    Q_UNUSED(connection)

    m_pTcpClient->OnErrMsg(QString("RabbitMq Server Close Connection."));
}

uint16_t QTcpConnectionHandler::onNegotiate(AMQP::Connection *connection, uint16_t interval)
{
    Q_UNUSED(connection)

    if(m_heartbeatInterval <= 0) {
        m_heartbeatInterval = 0;
    }

    if(m_heartbeatInterval > 0) {
        m_heartbeatInterval = qMin(interval, (uint16_t)m_heartbeatInterval);
        emit sigStartHeartbeatTimer(m_heartbeatInterval);
    }

    return m_heartbeatInterval;
}



} //namespace AMQP_QT
