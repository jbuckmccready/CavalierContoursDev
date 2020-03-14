import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Polyline 1.0

SplitView {
  GeometrySceneView {
    SplitView.fillWidth: true
    PlineOffsetAlgorithmView {
      id: polylineView
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
            from: -15
            to: 15
            value: polylineView.plineOffset
            onValueChanged: {
              polylineView.plineOffset = value;
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
          text: polylineView.repeatOffsetCount
          onTextChanged: {
            let c = parseInt(text);
            if (isNaN(c)) {
              return;
            }
            polylineView.repeatOffsetCount = c;
          }
        }

      }

    }

    CheckBox {
      text: "Show Original Polyline Vertexes"
      checked: polylineView.showOrigPlineVertexes
      onCheckedChanged: {
        polylineView.showOrigPlineVertexes = checked;
      }
    }

    GroupBox {
      title: "Raw Offsets"
      leftInset: 5
      ColumnLayout {
        CheckBox {
          id: showRawOffsetCheckBox
          text: "Show Raw Offset Polyline"
          checked: polylineView.showRawOffsetPolyline
          onCheckedChanged: {
            polylineView.showRawOffsetPolyline = checked;
          }
        }

        CheckBox {
          enabled: showRawOffsetCheckBox.checked
          text: "Show Dual Raw Offset Polyline"
          checked: polylineView.showDualRawOffsetPolyline
          onCheckedChanged: {
            polylineView.showDualRawOffsetPolyline = checked;
          }
        }

        CheckBox {
          enabled: showRawOffsetCheckBox.checked
          text: "Show Raw Offset Polyline Vertexes"
          checked: polylineView.showRawOffsetPlineVertexes
          onCheckedChanged: {
            polylineView.showRawOffsetPlineVertexes = checked;
          }
        }

        CheckBox {
          text: "Show Raw Offset Segments"
          checked: polylineView.showRawOffsetSegments
          onCheckedChanged: {
            polylineView.showRawOffsetSegments = checked;
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
      checked: polylineView.showEndPointIntersectCircles
      onCheckedChanged: {
        polylineView.showEndPointIntersectCircles = checked;
      }
    }

    Item {
      Layout.fillHeight: true
    }
  }
}
