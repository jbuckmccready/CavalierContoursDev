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
      text: "Polyline Offset"
    }
    TabButton {
      text: "Polyline Combine"
    }
    TabButton {
      text: "Polyline Offset Islands"
    }
    TabButton {
      text: "Hilbert Curve"
    }
  }

  Page {
    anchors.fill: parent
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

}
