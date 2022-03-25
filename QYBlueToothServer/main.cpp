#include <QApplication>
#include "qybluetoothserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	
    QYBlueToothServer blueToothServer;
	blueToothServer.createServer();
	
    return a.exec();
}
