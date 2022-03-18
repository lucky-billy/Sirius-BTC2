#include "qybluetoothclient.h"
#include <QDebug>

// 蓝牙串口通信服务的UUID码 "00001101-0000-1000-8000-00805F9B34FB"
static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

QYBlueToothClient::QYBlueToothClient(QObject *parent) : QObject(parent)
{
    m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

    connect(m_socket, &QBluetoothSocket::connected, [](){ qDebug() << "Connect to server successfully !"; });
    connect(m_socket, &QBluetoothSocket::disconnected, [&](){ m_socket->deleteLater(); });
    connect(m_socket, &QBluetoothSocket::readyRead, [&](){ readData(); });


    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            [&](const QBluetoothDeviceInfo &info){ findNewDevice(info); });
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            [](){ qDebug() << "Device discovery finished !"; });

    // 开始扫描蓝牙设备
    m_discoveryAgent->start();
}

void QYBlueToothClient::findNewDevice(const QBluetoothDeviceInfo &info)
{
    qDebug() << "name: " + info.name();
    qDebug() << "address: " + info.address().toString();
    qDebug() << "deviceUuid: " + info.deviceUuid().toString();

    m_socket->connectToService(info.address(), QBluetoothUuid(serviceUuid) ,QIODevice::ReadWrite);
}

void QYBlueToothClient::readData()
{
    if ( !m_socket ) {
        qDebug() << "Invalid client !";
        return;
    }

    QByteArray data = m_socket->readAll();
    QString str = QString::fromLatin1(data);
    qDebug() << "readData: " + str;
}

void QYBlueToothClient::sendData(QString data)
{
    if ( !m_socket ) {
        qDebug() << "Invalid client !";
        return;
    }

    qDebug() << "sendData: " + data;
    m_socket->write(data.toLatin1());
}
