#ifndef QTCPCONNECTIONHANDLER_H
#define QTCPCONNECTIONHANDLER_H

#include <QObject>
#include "amqpcpp.h"


namespace AMQP_QT {

class QTcpClient;


/**
 * @brief The QTcpConnectionHandler class
 */
class QTcpConnectionHandler : public QObject, public AMQP::ConnectionHandler
{
    Q_OBJECT

public:
    QTcpConnectionHandler(std::shared_ptr<QTcpClient> pTcpClient);
    virtual ~QTcpConnectionHandler();

    // 设置心跳间隔, 单位s
    void setHeartbeatInterval(int interval);
    // 获取心跳间隔
    uint16_t getHeartbeatInterval() const;

    // 数据待发送出去
    virtual void onData(AMQP::Connection *connection, const char *data, size_t size) override;
    // Rmq登录成功
    virtual void onReady(AMQP::Connection *connection) override;
    // 发生错误，一般发生此错误后连接不再可用
    virtual void onError(AMQP::Connection *connection, const char *message) override;
    // 对端关闭连接
    virtual void onClosed(AMQP::Connection *connection) override;

protected:
    virtual uint16_t onNegotiate(AMQP::Connection *connection, uint16_t interval) override;

signals:
    void sigStartHeartbeatTimer(int);

private:
    std::shared_ptr<QTcpClient> m_pTcpClient = nullptr;

    int m_heartbeatInterval = 0;
};



} //namespace AMQP_QT


#endif // QTCPCONNECTIONHANDLER_H
