import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Polyline 1.0

SplitView {
  GeometrySceneView {
    SplitView.fillWidth: true
    PlineCombineAlgorithmView {
      id: algorithmView
      anchors.fill: parent
      plineCombineMode: combineModeComboBox.currentIndex
    }
  }

  ColumnLayout {
    anchors.topMargin: 5
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    SplitView.preferredWidth: 300

    CheckBox {
      text: "Show Vertexes"
      checked: algorithmView.showVertexes
      onCheckedChanged: {
        algorithmView.showVertexes = checked;
      }
    }

    CheckBox {
      text: "Show Intersects"
      checked: algorithmView.showIntersects
      onCheckedChanged: {
        algorithmView.showIntersects = checked;
      }
    }

    GroupBox {
      title: "Combine Mode"
      leftInset: 5
      ColumnLayout {
        ComboBox {
          id: combineModeComboBox
          leftInset: 5
          model: ["None", "Union", "Exclude", "Intersect", "XOR", "Coincident Slices"]
          implicitWidth: 200
        }
        CheckBox {
          text: "Flip Arg Order"
          checked: algorithmView.flipArgOrder
          onCheckedChanged: {
            algorithmView.flipArgOrder = checked;
          }
        }
      }
    }


    GroupBox {
      title: "Winding Number"
      leftInset: 5
      ColumnLayout {
        CheckBox {
          text: "Show Test Point"
          checked: algorithmView.showWindingNumberPoint
          onCheckedChanged: {
            algorithmView.showWindingNumberPoint = checked;
          }
        }

        Text {
          id: windingNumberText
          leftPadding: 6
          text: "Winding Number: " + algorithmView.windingNumber
        }
      }
    }

    Item {
      Layout.fillHeight: true
    }
  }
}
