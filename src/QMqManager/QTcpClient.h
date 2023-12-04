#ifndef QTCPCLIENT_H
#define QTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>


namespace AMQP_QT {


/**
 * @brief The QTcpClient class
 */
class QTcpClient : public QObject
{
    Q_OBJECT
public:
    QTcpClient(QString addr_host, uint32_t addr_port, QObject *parent = nullptr);
    ~QTcpClient();

    bool NewConnect();
    bool SendData(const QByteArray &msg);
    void OnErrMsg(const QString &msg);

protected slots:
    void OnGetMsg();
    void OnSocketErr(QAbstractSocket::SocketError);

signals:
    void sigParseTcpMsg(const QByteArray&);
    void sigSocketErr(const QString&);

private:
    QString m_host;
    uint32_t m_port;

    std::shared_ptr<QTcpSocket> m_pSock;
    QString m_errMessage;
};


} //namespace AMQP_QT


#endif // QTCPCLIENT_H
