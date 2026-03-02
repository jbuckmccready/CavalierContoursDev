import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Polyline 1.0

SplitView {
    GeometrySceneView {
        SplitView.fillWidth: true
        SplitView.fillHeight: true
        SplitView.minimumWidth: 320

        PlineCombineAlgorithmView {
            id: algorithmView
            anchors.fill: parent
            plineCombineMode: combineModeComboBox.currentIndex
        }
    }

    ScrollView {
        id: controlPanel
        clip: true
        contentWidth: availableWidth
        SplitView.minimumWidth: 270
        SplitView.preferredWidth: 320
        SplitView.maximumWidth: 440
        SplitView.fillHeight: true

        ColumnLayout {
            width: controlPanel.availableWidth
            spacing: 8

            CheckBox {
                Layout.fillWidth: true
                text: "Show Vertexes"
                checked: algorithmView.showVertexes
                onCheckedChanged: {
                    algorithmView.showVertexes = checked;
                }
            }

            CheckBox {
                Layout.fillWidth: true
                text: "Show Intersects"
                checked: algorithmView.showIntersects
                onCheckedChanged: {
                    algorithmView.showIntersects = checked;
                }
            }

            GroupBox {
                title: "Combine Mode"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent

                    ComboBox {
                        id: combineModeComboBox
                        Layout.fillWidth: true
                        model: ["None", "Union", "Exclude", "Intersect", "XOR", "Coincident Slices"]
                    }

                    CheckBox {
                        Layout.fillWidth: true
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
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent

                    CheckBox {
                        Layout.fillWidth: true
                        text: "Show Test Point"
                        checked: algorithmView.showWindingNumberPoint
                        onCheckedChanged: {
                            algorithmView.showWindingNumberPoint = checked;
                        }
                    }

                    Text {
                        id: windingNumberText
                        Layout.fillWidth: true
                        wrapMode: Text.Wrap
                        text: "Winding Number: " + algorithmView.windingNumber
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
