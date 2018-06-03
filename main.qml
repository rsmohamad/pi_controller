import QtQuick 2.9
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11

ApplicationWindow {

    //title of the application
    title: qsTr("Hello World")
    width: 1024
    height: 640
    visible: true
    id: appWindow

    //menu containing two menu items
    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    //Content Area

    //a button in the middle of the content area
    GridLayout {
        id: grid
        columns: 2

        LineCh {
            width: appWindow.width * 0.8;
            height: appWindow.height * 0.95;
        }

        ColumnLayout {
            spacing: 10
            Layout.alignment: Qt.AlignRight

            Button {
                text: "Start";
            }

            Button {
                text: "Exit"
            }

            Button {
                text: "Load CSV"
            }
        }
    }
}

