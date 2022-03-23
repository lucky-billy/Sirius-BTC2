import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.6

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Client")

    TextField {
        id: textField
        x: 68
        y: 57
        placeholderText: qsTr("Text Field")
    }

    Button {
        id: button
        x: 68
        y: 169
        text: qsTr("Start")
        onClicked: client.startDiscovery()
    }

    Button {
        id: button1
        x: 68
        y: 284
        text: qsTr("Send")
        onClicked: client.sendData(textField.text)
    }


}
