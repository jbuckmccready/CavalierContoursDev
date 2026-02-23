import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1680
    height: 1050
    minimumWidth: 1024
    minimumHeight: 680
    title: qsTr("Cavalier Contours")

    header: TabBar {
        id: tabBar
        width: parent.width

        TabButton {
            width: tabBar.width / 4
            text: "Polyline Offset"
        }
        TabButton {
            width: tabBar.width / 4
            text: "Polyline Combine"
        }
        TabButton {
            width: tabBar.width / 4
            text: "Polyline Offset Islands"
        }
        TabButton {
            width: tabBar.width / 4
            text: "Hilbert Curve"
        }
    }

    Rectangle {
        anchors.fill: parent
        border.width: 2
        border.color: "grey"

        StackLayout {
            anchors.fill: parent
            anchors.margins: 2
            currentIndex: tabBar.currentIndex

            PlineOffsetScene {}
            PlineCombineScene {}
            PlineOffsetIslandsScene {}
            HilbertCurveScene {}
        }
    }
}
