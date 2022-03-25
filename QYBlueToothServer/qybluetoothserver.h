#ifndef QYBLUETOOTHSERVER_H
#define QYBLUETOOTHSERVER_H

#include <QObject>
#include <QBluetoothServiceInfo>
#include <QBluetoothServer>
#include <QBluetoothSocket>

class QYBlueToothServer : public QObject
{
    Q_OBJECT

public:
    explicit QYBlueToothServer(QObject *parent = nullptr);
    ~QYBlueToothServer();

    // 创建服务器
    Q_INVOKABLE void createServer();

    // 销毁服务器
    Q_INVOKABLE void destroyServer();

    // 发送数据
    Q_INVOKABLE void sendData(QString data);

protected:
    // 客户端接入
    void clientConnected();

    // 读取数据
    void readData();

signals:
    // 发送信息给应用层
    void sendMessage(int messageType);

private:
    QBluetoothServiceInfo m_serviceInfo;
    QBluetoothServer *m_server;
    QBluetoothSocket *m_socket;
};

#endif // QYBLUETOOTHSERVER_H
