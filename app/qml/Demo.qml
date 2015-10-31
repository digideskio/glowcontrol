import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.0 as ListItem
import Glowcontrol 1.0

Page {
    id: demoPage

    title: i18n.tr("Demo")


    QtObject {
        id: demoBulb
        property string label: "Demo Bulb"
        property real brightness: 0.5
        property var color: LifxHelper.colorToHSKB("red")
        property bool power: true
    }


    Column {
        id: root

        anchors {
            left: parent.left
            right: parent.right
        }

        Rectangle {
            color: "#fafafa"

            anchors {
                left: parent.left
                right: parent.right
                margins: units.gu(2)
            }
            height: units.gu(20)

            Icon {
                id: backdrop
                anchors.centerIn: parent
                height: units.gu(10)
                width: units.gu(10)
                name: "display-brightness-max"
                color: "black"
            }

            Icon {
                anchors.centerIn: parent
                height: backdrop.height
                width: backdrop.width
                name: backdrop.name
                color: {
                    62978: "red",
                    5525: "orange",
                    7615: "yellow",
                    16173: "green",
                    29814: "cyan",
                    43634: "blue",
                    50486: "purple",
                    58275: "pink",
                }[demoBulb.color.hue]
                opacity: demoBulb.power ? demoBulb.brightness : 0

                Behavior on opacity {
                    NumberAnimation { duration: UbuntuAnimation.FastDuration }
                }

            }
        }

        BulbControl {
            anchors {
                left: parent.left
                right: parent.right
            }
            bulb: demoBulb
        }
    }
}
