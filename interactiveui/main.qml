import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Shapes 1.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Polyline 1.0

Window {
  id: mainWindow
  visible: true
  width: 1680
  height: 1050
  title: qsTr("Hello World")
  SplitView {
    anchors.fill: parent

    Rectangle {
      id: rect
      SplitView.fillWidth: true
      border.width: 2
      border.color: "black"
      clip: true

      Rectangle {
        id: plineSceneItem
        width: 20000
        height: 20000
        // initially center (0,0)
        x: (1680-300)/2 - 10000
        y: 1050/2 - 10000
        border.width: 1/scaler.xScale
        border.color: "blue"

        transform: Scale {
          id: scaler
          origin.x: pinchArea.m_x2
          origin.y: pinchArea.m_y2
          xScale: pinchArea.m_zoom2
          yScale: pinchArea.m_zoom2
        }

        PinchArea {
          id: pinchArea
          anchors.fill: parent
          property real m_x1: 0
          property real m_y1: 0
          property real m_y2: 0
          property real m_x2: 0
          property real m_zoom1: 1
          property real m_zoom2: 1
          property real m_max: 5
          property real m_min: 0.05

          onPinchStarted: {
            m_x1 = scaler.origin.x
            m_y1 = scaler.origin.y
            m_x2 = pinch.startCenter.x
            m_y2 = pinch.startCenter.y
            plineSceneItem.x = plineSceneItem.x + (pinchArea.m_x1-pinchArea.m_x2)*(1-pinchArea.m_zoom1)
            plineSceneItem.y = plineSceneItem.y + (pinchArea.m_y1-pinchArea.m_y2)*(1-pinchArea.m_zoom1)
          }
          onPinchUpdated: {
            m_zoom1 = scaler.xScale
            var dz = pinch.scale-pinch.previousScale
            var newZoom = m_zoom1+dz
            if (newZoom <= m_max && newZoom >= m_min) {
              m_zoom2 = newZoom
            }
          }

          MouseArea {
            id: dragArea
            hoverEnabled: true
            anchors.fill: parent
            drag.target: plineSceneItem
            drag.filterChildren: true
            onWheel: {
              pinchArea.m_x1 = scaler.origin.x
              pinchArea.m_y1 = scaler.origin.y
              pinchArea.m_zoom1 = scaler.xScale

              pinchArea.m_x2 = mouseX
              pinchArea.m_y2 = mouseY

              var newZoom
              if (wheel.angleDelta.y > 0) {
                newZoom = pinchArea.m_zoom1+0.15
                if (newZoom <= pinchArea.m_max) {
                  pinchArea.m_zoom2 = newZoom
                } else {
                  pinchArea.m_zoom2 = pinchArea.m_max
                }
              } else {
                newZoom = pinchArea.m_zoom1-0.15
                if (newZoom >= pinchArea.m_min) {
                  pinchArea.m_zoom2 = newZoom
                } else {
                  pinchArea.m_zoom2 = pinchArea.m_min
                }
              }
              plineSceneItem.x = plineSceneItem.x + (pinchArea.m_x1-pinchArea.m_x2)*(1-pinchArea.m_zoom1)
              plineSceneItem.y = plineSceneItem.y + (pinchArea.m_y1-pinchArea.m_y2)*(1-pinchArea.m_zoom1)
            }
            MouseArea {
              anchors.fill: parent
            }
          }
        }

        PlineOffsetAlgorithmView {
          id: polylineView
          anchors.fill: parent
          onScaleChanged: console.log("scale changed")
          spatialIndexTarget: spatialIndexTargetComboBox.currentIndex
          selfIntersectsTarget: selfIntersectsTargetComboBox.currentIndex
          finishedPolyline: finishedPlinesComboBox.currentIndex
        }

        Rectangle {
          id: horizontalAxis
          x: parent.width / 2
          height: parent.height
          color: "black"
          width: 1 / pinchArea.m_zoom2
        }
        Rectangle {
          id: verticalAxis
          y: parent.width / 2
          width: parent.width
          color: "black"
          height: 1 / pinchArea.m_zoom2
        }

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
}
