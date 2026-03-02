import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Polyline 1.0

SplitView {
    GeometrySceneView {
        SplitView.fillWidth: true
        SplitView.fillHeight: true
        SplitView.minimumWidth: 320

        PlineOffsetIslandsAlgorithmView {
            id: algorithmView
            anchors.fill: parent
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
                }
            }

            CheckBox {
                Layout.fillWidth: true
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
}
