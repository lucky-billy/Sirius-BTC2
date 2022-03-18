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

    // 发送数据
    void sendData(QString data);

protected:
    // 搜索到设备
    void findNewDevice(const QBluetoothDeviceInfo &info);

    // 读取数据
    void readData();

signals:

private:
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QBluetoothSocket *m_socket;
};

#endif // QYBLUETOOTHCLIENT_H
