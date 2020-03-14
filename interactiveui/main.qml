import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

ApplicationWindow {
  id: mainWindow
  visible: true
  width: 1680
  height: 1050
  title: qsTr("Cavalier Contours")
  header:  TabBar {
    id: tabBar
    TabButton {
      text: "Offset"
      width: implicitWidth
    }
    TabButton {
      text: "Boolean Ops"
      width: implicitWidth
    }
  }

  Page {
    anchors.fill: parent
    StackLayout {
      anchors.fill: parent
      currentIndex: tabBar.currentIndex
      Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        border.width: 2
        border.color: "grey"
        OffsetAlgorithmScene {
          anchors.fill: parent
          anchors.margins: 2
        }
      }

      Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        border.width: 2
        border.color: "grey"
        BooleanOpsAlgorithmScene {
          anchors.fill: parent
          anchors.margins: 2
        }

      }
    }
  }

}
