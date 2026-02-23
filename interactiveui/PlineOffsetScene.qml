import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Polyline 1.0

SplitView {
    GeometrySceneView {
        SplitView.fillWidth: true
        SplitView.fillHeight: true
        SplitView.minimumWidth: 320

        PlineOffsetAlgorithmView {
            id: algorithmView
            anchors.fill: parent
            spatialIndexTarget: spatialIndexTargetComboBox.currentIndex
            selfIntersectsTarget: selfIntersectsTargetComboBox.currentIndex
            finishedPolyline: finishedPlinesComboBox.currentIndex
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

            GroupBox {
                title: "Offset"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent

                    Item {
                        id: offsetItem
                        Layout.fillWidth: true
                        implicitHeight: offsetTextField.implicitHeight + offsetSlider.implicitHeight + minText.implicitHeight

                        TextField {
                            id: offsetTextField
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: parent.top
                            validator: DoubleValidator {
                                bottom: offsetSlider.from
                                top: offsetSlider.to
                            }
                            text: if (!focus) {
                                offsetSlider.value;
                            } else
                                ""
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
                            anchors.left: parent.left
                            anchors.right: parent.right
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
                        Layout.fillWidth: true
                        validator: IntValidator {
                            bottom: 0
                            top: 1000
                        }
                        text: algorithmView.offsetCount
                        onTextChanged: {
                            let c = parseInt(text);
                            if (isNaN(c)) {
                                return;
                            }
                            algorithmView.offsetCount = c;
                        }
                    }

                    CheckBox {
                        Layout.fillWidth: true
                        text: "Show Last Pruned Raw Offsets"
                        checked: algorithmView.showLastPrunedRawOffsets
                        onCheckedChanged: {
                            algorithmView.showLastPrunedRawOffsets = checked;
                        }
                    }
                }
            }

            CheckBox {
                Layout.fillWidth: true
                text: "Show Original Polyline Vertexes"
                checked: algorithmView.showOrigPlineVertexes
                onCheckedChanged: {
                    algorithmView.showOrigPlineVertexes = checked;
                }
            }

            GroupBox {
                title: "Raw Offsets"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent

                    CheckBox {
                        id: showRawOffsetCheckBox
                        Layout.fillWidth: true
                        text: "Show Raw Offset Polyline"
                        checked: algorithmView.showRawOffsetPolyline
                        onCheckedChanged: {
                            algorithmView.showRawOffsetPolyline = checked;
                        }
                    }

                    CheckBox {
                        Layout.fillWidth: true
                        enabled: showRawOffsetCheckBox.checked
                        text: "Show Dual Raw Offset Polyline"
                        checked: algorithmView.showDualRawOffsetPolyline
                        onCheckedChanged: {
                            algorithmView.showDualRawOffsetPolyline = checked;
                        }
                    }

                    CheckBox {
                        Layout.fillWidth: true
                        enabled: showRawOffsetCheckBox.checked
                        text: "Show Raw Offset Polyline Vertexes"
                        checked: algorithmView.showRawOffsetPlineVertexes
                        onCheckedChanged: {
                            algorithmView.showRawOffsetPlineVertexes = checked;
                        }
                    }

                    CheckBox {
                        Layout.fillWidth: true
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
                Layout.fillWidth: true

                ComboBox {
                    id: selfIntersectsTargetComboBox
                    width: parent.width
                    model: ["None", "Original Polyline", "Raw Offset Polyline"]
                }
            }

            GroupBox {
                title: "Finished Polylines"
                Layout.fillWidth: true

                ComboBox {
                    id: finishedPlinesComboBox
                    width: parent.width
                    model: ["None", "Slices", "DualSlices", "Joined"]
                }
            }

            GroupBox {
                title: "Spatial Index"
                Layout.fillWidth: true

                ComboBox {
                    id: spatialIndexTargetComboBox
                    width: parent.width
                    model: ["None", "Original Polyline", "Raw Offset Polyline"]
                }
            }

            CheckBox {
                Layout.fillWidth: true
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
}
