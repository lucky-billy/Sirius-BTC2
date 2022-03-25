#include "qybluetoothserver.h"
#include <QBluetoothLocalDevice>
#include <QDebug>

QYBlueToothServer::QYBlueToothServer(QObject *parent) : QObject(parent)
{
    m_server = nullptr;
    m_socket = nullptr;
}

QYBlueToothServer::~QYBlueToothServer()
{
    // 销毁服务器
    destroyServer();
}

void QYBlueToothServer::createServer()
{
    // 本地设备信息
    QBluetoothLocalDevice m_localDevice;

    // 蓝牙串口通信服务的UUID码 "00001101-0000-1000-8000-00805F9B34FB"
    static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

    // 创建服务器
    m_server = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(m_server, &QBluetoothServer::newConnection, [&](){ clientConnected(); });

    // 开启监听
    bool ret = m_server->listen(m_localDevice.address());
    if ( ret ) {
        qDebug() << "Bluetooth server starts listening for " << m_localDevice.address().toString();
    } else {
        emit sendMessage(1);
        return;
    }

    /*
    * 一个服务器要想工作，必须得让人知道它在那，然后跟它连接上。因此，为了让其它设备可以发现它，
    * 我们需要在系统的 SDP(Service Discovery Protocol) 服务发现协议上构建描述其属性的记录。
    * 这些属性可以封装在一个 QBluetoothServerInfo 对象中。
    */

    // 创建服务，类UUID必须至少包含一个条目
    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList, classId);

    classId.prepend(QVariant::fromValue(QBluetoothUuid(serviceUuid)));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    // 设置服务名称、描述和提供者
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("QYBTCS"));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription, tr("QY BlueTooth Server"));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("qt-project.org"));

    // 设置服务 UUID
    m_serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

    // 设置服务可以被其他设备发现
    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList, publicBrowse);

    // 设置协议描述符列表
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));

    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(m_server->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));

    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                               protocolDescriptorList);

    // 注册服务
    ret = m_serviceInfo.registerService(m_localDevice.address());
    if ( ret ) {
        qDebug() << "Bluetooth service registration succeeded !";
    } else {
        emit sendMessage(2);
    }
}

void QYBlueToothServer::destroyServer()
{
    // 注销服务
    if ( m_serviceInfo.isValid() ) {
        bool ret = m_serviceInfo.unregisterService();
        if ( ret ) {
            qDebug() << "Bluetooth service unregiste successfully !";
        } else {
            qDebug() << "Bluetooth service unregiste failed !";
        }
    }

    // 销毁服务器
    if ( m_server != nullptr ) {
        delete m_server;
        m_server = nullptr;
    }
}

void QYBlueToothServer::clientConnected()
{
    m_socket = m_server->nextPendingConnection();

    connect(m_socket, &QBluetoothSocket::connected, [&](){ emit sendMessage(3); });

    connect(m_socket, &QBluetoothSocket::disconnected, [&](){ emit sendMessage(4); m_socket->deleteLater(); });

    connect(m_socket, &QBluetoothSocket::readyRead, [&](){ readData(); });
}

void QYBlueToothServer::readData()
{
    QByteArray data = m_socket->readAll();
    QString str = QString::fromLatin1(data);
    qDebug() << "readData: " + str;
}

void QYBlueToothServer::sendData(QString data)
{
    if ( !m_socket->isOpen() ) {
        emit sendMessage(5);
        return;
    }

    qDebug() << "sendData: " + data;
    m_socket->write(data.toLatin1());
}
