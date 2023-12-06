#include "QTcpClient.h"
#include <QDebug>
#include <QDataStream>


namespace AMQP_QT {


QTcpClient::QTcpClient(QString addr_host, uint32_t addr_port, QObject *parent)
    :QObject(parent), m_host(addr_host), m_port(addr_port)
{
    m_pSock = std::make_shared<QTcpSocket>(this);
}

QTcpClient::~QTcpClient()
{
    m_pSock->abort();
}

bool QTcpClient::NewConnect()
{
    m_pSock->abort();
    m_pSock->connectToHost(m_host, m_port);
    if(!m_pSock->waitForConnected(1000 * 5)) {
        qCritical() << __FUNCTION__ << ", connect server tiemout! host:" << m_host << ", port:" << m_port;
        return false;
    }

    connect(m_pSock.get(), SIGNAL(readyRead()), this, SLOT(OnGetMsg()));
    connect(m_pSock.get(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnSocketErr(QAbstractSocket::SocketError)));

    return true;
}

bool QTcpClient::SendData(const QByteArray &msg)
{
    try {
        m_pSock->write(msg);
    }
    catch (const std::exception &e) {
        m_errMessage = "Send Data Failed: " + QString(e.what());
        return false;
    }

    return true;
}

void QTcpClient::OnErrMsg(const QString &msg)
{
    m_errMessage = msg;
    qDebug() << __FUNCTION__ << m_errMessage;
}

void QTcpClient::OnGetMsg()
{
    QByteArray m_message = m_pSock->readAll();

    emit sigParseTcpMsg(m_message);
}

void QTcpClient::OnSocketErr(QAbstractSocket::SocketError)
{
    m_errMessage = m_pSock->errorString();
    emit sigSocketErr(m_errMessage);
    qCritical() << __FUNCTION__ << m_errMessage;
}


} //namespace AMQP_QT
