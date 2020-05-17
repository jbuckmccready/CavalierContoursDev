import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Polyline 1.0

SplitView {
  GeometrySceneView {
    SplitView.fillWidth: true
    PlineOffsetIslandsAlgorithmView {
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
      title: "Offset"
      leftInset: 5
      ColumnLayout {
        Item {
          id: offsetItem
          implicitWidth: offsetSlider.implicitWidth
          implicitHeight: offsetTextField.implicitHeight + offsetSlider.implicitHeight + minText.implicitHeight
          TextField {
            id: offsetTextField
            anchors.top: offsetItem.top
            validator: DoubleValidator { bottom: offsetSlider.from; top: offsetSlider.to }
            text: if (!focus) { offsetSlider.value } else ""
            onTextChanged: {
              let f = parseFloat(text);
              if (isNaN(f)) {
                return;
              }

              offsetSlider.value = f;
            }

          }

          Slider {
            id: offsetSlider
            anchors.top: offsetTextField.bottom
            from: -15
            to: 15
            value: algorithmView.offsetDelta
            onValueChanged: {
              algorithmView.offsetDelta = value;
            }
          }

          Text {
            id: minText
            anchors.left: offsetSlider.left
            anchors.top: offsetSlider.bottom
            text: offsetSlider.from
          }
          Text {
            id: maxText
            anchors.right: offsetSlider.right
            anchors.top: offsetSlider.bottom
            text: offsetSlider.to
          }
        }

        Label {
          topPadding: 5
          text: "Offset Count"
        }

        TextField {
          validator: IntValidator { bottom: 0; top: 1000 }
          text: algorithmView.offsetCount
          onTextChanged: {
            let c = parseInt(text);
            if (isNaN(c)) {
              return;
            }
            algorithmView.offsetCount = c;
          }
        }

      }

    }

    CheckBox {
      text: "Show Vertexes"
      checked: algorithmView.showVertexes
      onCheckedChanged: {
        algorithmView.showVertexes = checked;
      }
    }

    Item {
      Layout.fillHeight: true
    }
  }
}
