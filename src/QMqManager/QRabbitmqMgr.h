#ifndef QRABBITMQMGR_H
#define QRABBITMQMGR_H

#include <QMap>
#include <QObject>
#include <QTimer>
#include <QMutex>
#include "amqpcpp/exchangetype.h"
#include "QTcpClient.h"


namespace AMQP {
class Connection;
class Channel;
class Message;
}


namespace AMQP_QT {

class QTcpConnectionHandler;

typedef struct _mqinfo
{
    QString ip = "127.0.0.1";
    uint16_t port = 5672;
    QString loginName = "guest";
    QString loginPwd = "guest";
    QString exchangeName = "";
    QString exchangeType = "topic";
    QString queueName = "";
    QString vhost = "/";
    QString routingKey = ""; // 用于发送端发布消息
    QString bindingKey = ""; // 接收端绑定queue时使用
} MqInfo;

// 定义exchangeType对应关系
const QMap<QString, AMQP::ExchangeType> MqExTypeMap {
    {"topic", AMQP::topic},
    {"direct", AMQP::direct},
    {"fanout", AMQP::fanout},
    {"headers", AMQP::headers},
    {"consistent_hash", AMQP::consistent_hash}
};


/**
 * @brief The QRabbitmqMgr class 基于AMQPCPP补充后的封装
 */
class QRabbitmqMgr : public QObject
{
    Q_OBJECT
public:
    enum MqRoles {
        MqNone = 0x00,
        MqConsumer = 0x01,
        MqPublisher = 0x10
        //MqBoth = 0x11
    };

public:
    QRabbitmqMgr(const MqInfo &mqinfo, MqRoles role = MqNone, int hbInterval = 0);
    ~QRabbitmqMgr();

    // 初始化时调用，启动连接、建立通道、创建组件并开启消费
    bool StartMqInstance();
    // 发送消息时调用。特别注意：该函数非线程安全，需要确保在单一线程中调用
    bool PublishMsg(const QString &msg);
    // 开始消费数据
    bool StartConsumeMsg();
    // 清空消息队列，需保证queue已创建好
    bool PurgeMsgQueue();
    // 获取错误信息
    QString getErrorMessage() const;


protected:
    // 初始化时调用，设置RabbitMq基本信息
    bool Init(const MqInfo &mqInfo);
    // 释放实例
    void ReleaseMqInstance();
    //  bindQueue
    void BindQueueExchange();
    // 返回状态
    void OnStatusChange(const bool isOk);
    // 返回错误信息
    void OnPrintErrMsg(const QString &err);
    // 收到消息
    void OnConsumeRecved(const AMQP::Message &message, uint64_t deliveryTag, bool redelivered);

protected slots:
    // 消费者接收到数据后发出此信号，请勿进行耗时操作
    void OnParseTcpMessage(const QByteArray& msg);
    // 若设置心跳，发送心跳数据
    void OnStartHeartbeatTimer(int interval);
    // 处理Tcp错误信息
    void OnTcpErrHandle(const QString &err);

signals:
    void sigRecvedDataReady(const QByteArray& data);
    void sigMqConnectError();

private:
    bool CreateMqChannel();
    bool CloseMqChannel();
    bool CloseMqConnection();
    bool CreateMqExchange(const QString &exchangeName, const QString &exchangeType);
    bool CreateMqQueue(const QString &queueName);
    bool BindQueue(const QString &queueName, const QString &exchangeName, const QString &routingKey);
    bool SetQosValue(const uint16_t val);

    // 注册回调函数
    void ChannelOkCb();
    void ChannelErrCb(const char *msg);
    void ChannelCloseErrCb(const char *msg);
    void CreatMqExchangeErrCb(const char *msg);
    void CreatMqQueueErrCb(const char *msg);
    void BindQueueErrCb(const char *msg);
    void SetQosValueErrCb(const char *msg);
    void ConsumeErrorCb(const char *msg);


private:
    MqRoles m_role;
    MqInfo m_mqInfo;
    std::shared_ptr<QTcpClient> m_pTcpClient = nullptr;
    std::shared_ptr<QTcpConnectionHandler> m_pHandler = nullptr;
    std::shared_ptr<AMQP::Connection > m_connection = nullptr;
    std::shared_ptr<AMQP::Channel> m_channel = nullptr;

    QMutex m_channelMutex;
    std::shared_ptr<QTimer> m_heartbeatTimer = nullptr;
    int m_heartbeatInterval = 0;    //心跳时间，单位:秒
    int m_mqConnErrIndex = 0;
    QString m_errMessag;
};



} //namespace AMQP_QT



#endif // QRABBITMQMGR_H
