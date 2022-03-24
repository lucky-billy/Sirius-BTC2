#ifndef QYBLUETOOTHCLIENT_H
#define QYBLUETOOTHCLIENT_H

#include <QObject>

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>

class QYBlueToothClient : public QObject
{
    Q_OBJECT

public:
    explicit QYBlueToothClient(QObject *parent = nullptr);

    // 开始扫描蓝牙设备
    Q_INVOKABLE void startDiscovery();

    // 发送数据
    Q_INVOKABLE void sendData(QString data);

protected:
    // 搜索到设备
    void findNewDevice(const QBluetoothDeviceInfo &info);

    // 读取数据
    void readData();

signals:
    // 发送信息给应用层
//    void sendMessage(int type);

private:
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QBluetoothSocket *m_socket;
};

#endif // QYBLUETOOTHCLIENT_H
