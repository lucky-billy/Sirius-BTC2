#ifndef QYBLUETOOTHSERVER_H
#define QYBLUETOOTHSERVER_H

#include <QObject>

#include <QBluetoothLocalDevice>
#include <QBluetoothServiceInfo>
#include <QBluetoothServer>
#include <QBluetoothSocket>

class QYBlueToothServer : public QObject
{
    Q_OBJECT

public:
    explicit QYBlueToothServer(QObject *parent = nullptr);
    ~QYBlueToothServer();

    // 发送数据
    Q_INVOKABLE void sendData(QString data);

protected:
    // 创建服务器
    void createServer();

    // 销毁服务器
    void destroyServer();

    // 客户端接入
    void clientConnected();

    // 读取数据
    void readData();

signals:
    // 发送信息给应用层
//    void sendMessage(int type);

private:
    QBluetoothLocalDevice *m_localDevice;
    QBluetoothServiceInfo m_serviceInfo;
    QBluetoothServer *m_server;
    QBluetoothSocket *m_socket;
};

#endif // QYBLUETOOTHSERVER_H
