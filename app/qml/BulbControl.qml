import QtQuick 2.0
import Ubuntu.Components 1.2

Item {
    id: bulbRoot
    property var bulb: null

    Row {
        id: bulbRow
        width: root.width
        spacing: units.gu(2)
        property int blocks: (width - (spacing * 2)) / 10

        Rectangle {
            width: 2 * parent.blocks
            height: bulbRoot.height - more.height
            Label {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    right: parent.right
                    margins: units.gu(2)
                }
                text: bulb.label
            }
        }

        Rectangle {
            width: 6 * parent.blocks
            height: bulbRoot.height - more.height
            Slider {
                id: brightnessSlider
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    right: parent.right
                    margins: units.gu(2)
                }

                property int brightness: bulb.brightness

                function formatValue(v) {
                    return (v / 65535).toFixed(2);
                }

                minimumValue: 0
                maximumValue: 65535
                value: brightness
                live: true

                onValueChanged: bulb.brightness = value

                Connections {
                    target: bulb
                    onBrightnessChanged: brightnessSlider.brightness = brightness
                }
            }
        }


        Rectangle {
            width: 2 * parent.blocks
            height: bulbRoot.height - more.height

            Button {
                id: powerButton
                property bool powered: bulb.power
                anchors {
                    verticalCenter: parent.verticalCenter
                    horizontalCenter: parent.horizontalCenter

                }
                width: units.gu(6)
                height: units.gu(6)
                iconName: 'system-shutdown'
                onClicked: bulb.power = !powered

                Connections {
                    target: bulb
                    onPowerChanged: powerButton.powered = power
                }
            }
        }
    }

    Rectangle {
        id: more
        anchors {
            left: parent.left
            right: parent.right
            top: bulbRow.bottom
        }
        height: units.gu(3)

        AbstractButton {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: parent.height

            Icon {
                width: units.gu(2)
                anchors.centerIn: parent
                name: 'go-down'
            }
            onClicked: console.log('Clicked more...')
        }
    }
}
