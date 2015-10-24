import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.0 as ListItem

Page {
    id: root
    title: i18n.tr("GlowControl for LIFX®")
    visible: false

    head.actions: [
        Action {
            iconName: "settings"
            onTriggered: pageStack.push(Qt.resolvedUrl("Settings.qml"))
        }
    ]

    Flickable {
        id: flickster
        anchors.fill: parent
        contentHeight: contentItem.childrenRect.height
        boundsBehavior: Flickable.DragAndOvershootBounds
        visible: bulbs.count

        Column {
            id: contentItem

            anchors {
                left: parent.left
                right: parent.right
            }

            add: Transition {
                NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: UbuntuAnimation.FastDuration }
                NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: UbuntuAnimation.FastDuration }
            }

             move: Transition {
                    NumberAnimation { properties: "x,y"; duration: UbuntuAnimation.FastDuration; easing.type: Easing.OutBounce }

                    // ensure opacity and scale values return to 1.0
                    NumberAnimation { property: "opacity"; to: 1.0 }
                    NumberAnimation { property: "scale"; to: 1.0 }
                }

            Repeater {
                id: bulbs
                model: glowcontrol.bulbs
                width: root.width

                BulbControl {
                    bulb: lifxbulb
                    width: root.width
                }
            }
        }
    }

    Label {
        anchors.centerIn: parent
        wrapMode: Text.Wrap
        visible: !bulbs.count
        text: i18n.tr("No LIFX® bulbs found.")
        horizontalAlignment: Text.AlignHCenter
        fontSize: "large"
    }
}
