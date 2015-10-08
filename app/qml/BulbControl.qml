import QtQuick 2.0
import Ubuntu.Components 1.2
import Ubuntu.Components.ListItems 1.2 as ListItem

Item {
    id: bulbRoot
    property var bulb: null

    Row {
        id: bulbRow
        property int blocks: width / 10
        anchors {
            top: bulbRoot.top
        }

        width: root.width
        height: bulbRoot.height - more.height

        Rectangle {
            color: "#fafafa"
            width: 4 * parent.blocks
            height: bulbRow.height
            Label {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    right: parent.right
                    margins: units.gu(2)
                }
                text: bulb.label
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideMiddle
            }
        }

        Rectangle {
            color: "#fafafa"
            width: 4 * parent.blocks
            height: bulbRow.height
            Slider {
                id: brightnessSlider
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    right: parent.right
                    margins: units.gu(2)
                }

                function formatValue(v) {
                    return (v / 65535).toFixed(2);
                }

                minimumValue: 0
                maximumValue: 65535
                value: bulb.brightness
                live: true

                onValueChanged: bulb.brightness = value

                Connections {
                    target: bulb
                    onBrightnessChanged: brightnessSlider.value = brightness
                }
            }
        }


        Rectangle {
            color: "#fafafa"
            width: 2 * parent.blocks
            height: bulbRow.height

            AbstractButton {
                id: powerButton
                property bool powered: bulb.power
                anchors {
                    fill: parent
                    margins: units.gu(1)
                }
                iconName: 'system-shutdown'
                opacity: powered ? 1 : 0.5
                onClicked: bulb.power = !powered

                style: Component {
                    Icon {
                        name: powerButton.iconName
                        height: powerButton.height
                        width: height
                    }
                }

                Behavior on opacity {
                    NumberAnimation { duration: UbuntuAnimation.FastDuration }
                }

                Connections {
                    target: bulb
                    onPowerChanged: powerButton.powered = power
                }
            }
        }
    }

    Column {
        id: more
        anchors {
            left: parent.left
            right: parent.right
            top: bulbRow.bottom
        }
        height: units.gu(4)

        ListItem.ThinDivider {
            id: moreBefore
        }

        AbstractButton {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: parent.height - moreBefore.height - moreAfter.height

            Icon {
                width: units.gu(2)
                anchors.centerIn: parent
                name: 'go-down'
            }
            onClicked: console.log('Clicked more...')
        }
        ListItem.Divider {
            id: moreAfter
        }
    }
}
