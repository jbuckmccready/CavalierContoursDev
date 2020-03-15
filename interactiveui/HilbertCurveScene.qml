import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import DemoFuncs 1.0

SplitView {
  ScrollView {
    clip: true
    SplitView.fillWidth: true
    contentWidth: hilbertVisual.implicitWidth * scale.xScale
    contentHeight: hilbertVisual.implicitHeight * scale.xScale
    Column {
      id: hilbertVisual
      spacing: 0
      transform: Scale {
        id: scale
        xScale: scaleSlider.value * 8/hilbertGrid.gridDim
        yScale: xScale
      }
      Grid {
        id: hilbertGrid
        property int gridDim: 8
        rows: gridDim
        columns: gridDim


        function scaleForHilbert(minValue, totalExtent, value) {
          const hilbertMax = (1 << 16) - 1;
          return Math.floor(hilbertMax * (value - minValue) / totalExtent);
        }

        function hilbertIndex(xIdx, yIdx) {
          var hx = scaleForHilbert(0, hilbertGrid.gridDim - 1, xIdx);
          var hy = scaleForHilbert(0, hilbertGrid.gridDim - 1, yIdx);
          return DemoFuncs.hilbertXYToIndex(hx, hy);
        }

        ListModel {
          id: hilbertGridModel
        }
        ListModel {
          id: hilbertRowModel
        }


        function toLinearIndex(xIdx, yIdx) {
          return xIdx + yIdx * columns;
        }

        function direction(fromCell, toCell) {
          if (fromCell === undefined || toCell === undefined) {
            return "";
          }

          if (fromCell.xIndex === toCell.xIndex) {
            if (fromCell.yIndex - 1 === toCell.yIndex) {
              return "up";
            }
            if (fromCell.yIndex === toCell.yIndex - 1) {
              return "down";
            }
          } else if (fromCell.yIndex === toCell.yIndex)  {
            if (fromCell.xIndex - 1 === toCell.xIndex) {
              return "left";
            }

            if (fromCell.xIndex === toCell.xIndex - 1) {
              return "right";
            }
          }

          return "";
        }

        function refreshModels() {
          var hModel = [];
          for(var yIdx = 0; yIdx < gridDim; ++yIdx) {
            for (var xIdx = 0; xIdx < gridDim; ++xIdx) {
              var hIdx = hilbertIndex(xIdx, yIdx);
              hModel.push({
                            xIndex: xIdx,
                            yIndex: yIdx,
                            hIndex: hIdx,
                            scaledHIndex: hIdx / 4294967295.0,
                            prevNeighbor: -1,
                            nextNeighbor: -1
                          });
            }
          }

          var copy = hModel.slice();
          copy.sort(function (a, b) { return a.hIndex - b.hIndex});
          copy[0].nextNeighbor = 1;
          for (var i = 1; i < copy.length - 1; ++i) {
            copy[i].nextNeighbor = i + 1;
            copy[i].prevNeighbor = i - 1;
          }

          copy[copy.length - 1].prevNeighbor = copy.length - 2;

          hilbertRowModel.clear();
          hilbertGridModel.clear();

          for (var k = 0; k < copy.length; ++k) {
            var c = copy[k];
            c.prevDir = direction(c, copy[c.prevNeighbor]);
            c.nextDir = direction(c, copy[c.nextNeighbor]);
            hilbertRowModel.append(c);
          }

          for (var j = 0; j < hModel.length; ++j) {
            hilbertGridModel.append(hModel[j]);
          }
        }

        onGridDimChanged: {
          refreshModels();
        }

        Component.onCompleted: {
          refreshModels();
        }

        Repeater {
          model: hilbertGridModel
          Rectangle {
            id: cell
            width: 100
            height: 100
            border.width: showGridLinesCheckBox.checked ? 1 : 0
            color: colorCellsCheckBox.checked ? hilbertGrid.colorMap(model.scaledHIndex) : "white"

            Item {
              id: connectors
              anchors.fill: parent
              property int connectorWidth: 8

              Rectangle {
                color: "grey"
                property string direction: model.prevDir

                x: direction === "left" ? 0 : parent.width / 2
                y: direction === "up" ? 0 : parent.height / 2
                width: connectors.connectorWidth + (direction === "left" || direction === "right" ? parent.width / 2 : 0)
                height: connectors.connectorWidth + (direction ===  "up" || direction === "down" ? parent.height / 2 : 0)
              }

              Rectangle {
                color: "grey"
                property string direction: model.nextDir

                x: direction === "left" ? 0 : parent.width / 2
                y: direction === "up" ? 0 : parent.height / 2
                width: connectors.connectorWidth + (direction === "left" || direction === "right" ? parent.width / 2 : 0)
                height: connectors.connectorWidth + (direction ===  "up" || direction === "down" ? parent.height / 2 : 0)
              }

              Text {
                anchors.fill: parent
                visible: showWeightingsCheckBox.checked
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 20
                text: model.scaledHIndex.toFixed(2)
              }

            }
          }
        }


        function colorMap(value) {
          if (value < 0.2) {
            return "#ffa600";
          }

          if (value < 0.4) {
            return "#ff6361";
          }

          if (value < 0.6) {
            return "#bc5090";
          }

          if (value < 0.8) {
            return "#58508d";
          }

          if (value <= 1.0) {
            return "#003f5c";
          }

        }
      }

      Flow {
        anchors.left: hilbertGrid.left
        anchors.right: hilbertGrid.right
        spacing: 0
        padding: 0
        Repeater {
          model: hilbertRowModel
          Rectangle {
            id: linearCell
            width: 50
            height: 60
            border.width: 1
            color: colorCellsCheckBox.checked ? hilbertGrid.colorMap(model.scaledHIndex) : "white"
            Text {
              anchors.fill: parent
              visible: showWeightingsCheckBox.checked
              verticalAlignment: Text.AlignVCenter
              horizontalAlignment: Text.AlignHCenter
              text: model.scaledHIndex.toFixed(2)
            }
          }
        }

      }

    }
  }

  ColumnLayout {
    anchors.topMargin: 5
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    SplitView.preferredWidth: 300

    GroupBox {
      Layout.fillWidth: true
      title: "Description"
      Text {
        wrapMode: Text.Wrap
        anchors.fill: parent
        text: "Draws the Hilbert space filling curve. Note only when the grid dimension is a power of 2 " +
              "(2, 4, 8, etc.) does the curve connect properly. This is because the hilbert values are " +
              "mapped to [0 -> 2^32 - 1]."
      }
    }

    GroupBox {
      leftInset: 5
      title: "Scale = " + Math.round(scaleSlider.value * 100) + "%"
      Slider {
        id: scaleSlider
        from: 0.2
        to: 3
        stepSize: 0.1
        value: 1.0
      }
    }

    GroupBox {
      leftInset: 5
      title: "Grid Dimension = " + hilbertGrid.gridDim
      Slider {
        from: 2
        to: 16
        stepSize: 1
        value: hilbertGrid.gridDim
        onValueChanged: {
          hilbertGrid.gridDim = value;
        }
      }
    }

    CheckBox {
      id: showWeightingsCheckBox
      text: "Show Weightings"
      checked: true
    }

    CheckBox {
      id: showGridLinesCheckBox
      text: "Show Grid Lines"
      checked: false
    }

    CheckBox {
      id: colorCellsCheckBox
      text: "Color Cells"
      checked: true
    }

    Item {
      Layout.fillHeight: true
    }
  }
}



