#include "qybluetoothserver.h"
#include <QDebug>

// 蓝牙串口通信服务的UUID码 "00001101-0000-1000-8000-00805F9B34FB"
static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

QYBlueToothServer::QYBlueToothServer(QObject *parent) : QObject(parent)
{
    m_localDevice = new QBluetoothLocalDevice(this);

    // 创建服务器
    createServer();
}

QYBlueToothServer::~QYBlueToothServer()
{
    // 销毁服务器
    destroyServer();
}

void QYBlueToothServer::createServer()
{
   // 创建服务器
   m_server = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
   connect(m_server, &QBluetoothServer::newConnection, [&](){ clientConnected(); });

   bool ret = m_server->listen(m_localDevice->address());
   if ( !ret ) {
       qDebug() << "Cannot bind server to " << m_localDevice->address().toString();
       return;
   }

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

   // 设置服务可发现性
   m_serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                              QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));

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
   m_serviceInfo.registerService(m_localDevice->address());
}

void QYBlueToothServer::destroyServer()
{
    // 注销服务
    m_serviceInfo.unregisterService();

    // 销毁服务器
    delete m_server;
    m_server = nullptr;
}

void QYBlueToothServer::clientConnected()
{
    m_socket = m_server->nextPendingConnection();
    if ( !m_socket ) {
        qDebug() << "Invalid client !";
        return;
    }

    connect(m_socket, &QBluetoothSocket::connected, [](){ qDebug() << "Client connect successfully !"; });

    connect(m_socket, &QBluetoothSocket::disconnected, [&](){ m_socket->deleteLater(); });

    connect(m_socket, &QBluetoothSocket::readyRead, [&](){ readData(); });
}

void QYBlueToothServer::readData()
{
    if ( !m_socket ) {
        qDebug() << "Invalid client !";
        return;
    }

    QByteArray data = m_socket->readAll();
    QString str = QString::fromLatin1(data);
    qDebug() << "readData: " + str;
}

void QYBlueToothServer::sendData(QString data)
{
    if ( !m_socket ) {
        qDebug() << "Invalid client !";
        return;
    }

    qDebug() << "sendData: " + data;
    m_socket->write(data.toLatin1());
}
