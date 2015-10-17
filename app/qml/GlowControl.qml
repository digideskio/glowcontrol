import QtQuick 2.0
import Ubuntu.Components 1.2
import Ubuntu.Components.ListItems 1.0 as ListItem
import Glowcontrol 1.0

/*!
    \brief MainView with Tabs element.
           First Tab has a single Label and
           second Tab has a single ToolbarAction.
*/

MainView {
    // objectName for functional testing purposes (autopilot-qt5)
    objectName: "mainView"

    // Note! applicationName needs to match the "name" field of the click manifest
    applicationName: "GlowControl"

    /*
     This property enables the application to change orientation
     when the device is rotated. The default is false.
    */
    //automaticOrientation: true

    width: units.gu(100)
    height: units.gu(76)

    Page {
        id: root
        title: i18n.tr("GlowControl for LIFX®")

        Connections {
            target: Qt.application
            onActiveChanged:
                if (Qt.application.active) {
                    glowcontrol.gainApplicationFocus();
                } else {
                    glowcontrol.loseApplicationFocus();
                }
        }

        GlowControl {
            id: glowcontrol
        }

        Flickable {
            id: flickster
            anchors.fill: parent
            contentHeight: contentItem.childrenRect.height
            boundsBehavior: Flickable.DragAndOvershootBounds

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

                    // ListItem.Standard {
                    //     text: modelData.label + (modelData.color)
                    //     control: Button {
                    //         id: b
                    //         iconName: 'system-shutdown'
                    //         opacity: powered ? 1 : 0.5
                    //         onClicked: modelData.power = !powered
                    //         width: units.gu(5)
                    //     }

                    // }
                }

                Label {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    visible: !bulbs.count
                    text: i18n.tr("No LIFX® bulbs found.")
                    horizontalAlignment: Text.AlignHCenter
                    fontSize: "large"
                }


                Item {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    height: units.gu(10)

                    Label {
                        opacity: 0.75
                        anchors.centerIn: parent
                        text: i18n.tr(
                            "GlowControl for LIFX® v%1. © Jonas Grønås Drange."
                        ).arg(glowcontrol.version)
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }


            // boundsBehavior: (contentHeight > root.height) ?
            //                  Flickable.DragAndOvershootBounds :
            //                  Flickable.StopAtBounds
            // flickableDirection: Flickable.VerticalFlick

            // Grid {
            //     id: grid
            //     columns: 2
            //     spacing: 2

            //     anchors {
            //         left: parent.left
            //         right: parent.right
            //     }

            //     Repeater {
            //         model: glowcontrol.bulbs

            //         Rectangle {
            //             id: rect
            //             property bool powered: modelData.power
            //             height: units.gu(20)
            //             width: grid.width / grid.columns

            //             color: modelData.color
            //             opacity: powered ? 1 : 0.5

            //             Label {
            //                 anchors.fill: parent
            //                 text: modelData.label
            //             }

            //             Connections {
            //                 target: modelData
            //                 onPowerChanged: rect.powered = power
            //             }
            //         }
            //         // ListItem.Standard {
            //         //     text: modelData.label + (modelData.color)
            //         //     control: Button {
            //         //         id: b
            //         //         iconName: 'system-shutdown'
            //         //         opacity: powered ? 1 : 0.5
            //         //         onClicked: modelData.power = !powered
            //         //         width: units.gu(5)
            //         //     }

            //         // }
            //     }
            // }
        }
    }
}

