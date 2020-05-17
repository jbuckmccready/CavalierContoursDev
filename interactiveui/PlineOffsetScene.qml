import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Polyline 1.0

SplitView {
  GeometrySceneView {
    SplitView.fillWidth: true
    PlineOffsetAlgorithmView {
      id: algorithmView
      anchors.fill: parent
      spatialIndexTarget: spatialIndexTargetComboBox.currentIndex
      selfIntersectsTarget: selfIntersectsTargetComboBox.currentIndex
      finishedPolyline: finishedPlinesComboBox.currentIndex
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
            from: -40
            to: 40
            value: algorithmView.plineOffset
            onValueChanged: {
              algorithmView.plineOffset = value;
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
      text: "Show Original Polyline Vertexes"
      checked: algorithmView.showOrigPlineVertexes
      onCheckedChanged: {
        algorithmView.showOrigPlineVertexes = checked;
      }
    }

    GroupBox {
      title: "Raw Offsets"
      leftInset: 5
      ColumnLayout {
        CheckBox {
          id: showRawOffsetCheckBox
          text: "Show Raw Offset Polyline"
          checked: algorithmView.showRawOffsetPolyline
          onCheckedChanged: {
            algorithmView.showRawOffsetPolyline = checked;
          }
        }

        CheckBox {
          enabled: showRawOffsetCheckBox.checked
          text: "Show Dual Raw Offset Polyline"
          checked: algorithmView.showDualRawOffsetPolyline
          onCheckedChanged: {
            algorithmView.showDualRawOffsetPolyline = checked;
          }
        }

        CheckBox {
          enabled: showRawOffsetCheckBox.checked
          text: "Show Raw Offset Polyline Vertexes"
          checked: algorithmView.showRawOffsetPlineVertexes
          onCheckedChanged: {
            algorithmView.showRawOffsetPlineVertexes = checked;
          }
        }

        CheckBox {
          text: "Show Raw Offset Segments"
          checked: algorithmView.showRawOffsetSegments
          onCheckedChanged: {
            algorithmView.showRawOffsetSegments = checked;
          }
        }

      }
    }

    GroupBox {
      title: "Self Intersects"
      leftInset: 5
      ComboBox {
        id: selfIntersectsTargetComboBox
        leftInset: 5
        model: ["None", "Original Polyline", "Raw Offset Polyline"]
        implicitWidth: 200
      }
    }

    GroupBox {
      title: "Finished Polylines"
      leftInset: 5
      ComboBox {
        id: finishedPlinesComboBox
        leftInset: 5
        model: ["None", "Slices", "DualSlices", "Joined"]
      }
    }

    GroupBox {
      title: "Spatial Index"
      leftInset: 5
      ComboBox {
        id: spatialIndexTargetComboBox
        leftInset: 5
        model: ["None", "Original Polyline", "Raw Offset Polyline"]
        implicitWidth: 200
      }
    }

    CheckBox {
      text: "Show End Intersect Circles"
      checked: algorithmView.showEndPointIntersectCircles
      onCheckedChanged: {
        algorithmView.showEndPointIntersectCircles = checked;
      }
    }

    Item {
      Layout.fillHeight: true
    }
  }
}
