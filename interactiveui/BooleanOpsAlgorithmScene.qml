import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Polyline 1.0

SplitView {
  GeometrySceneView {
    SplitView.fillWidth: true
    PlineBoolOpsAlgorithmView {
      id: algorithmView
      anchors.fill: parent
    }
  }

  ColumnLayout {
    anchors.topMargin: 5
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    SplitView.preferredWidth: 300

    GroupBox {
      title: "Winding Number"
      leftInset: 5
      Text {
        id: windingNumberText
        text: algorithmView.windingNumber
      }
    }
    Item {
      Layout.fillHeight: true
    }
  }
}
