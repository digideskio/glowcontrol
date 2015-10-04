import QtQuick 2.0
import Ubuntu.Components 1.1
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

    // Removes the old toolbar and enables new features of the new header.
    useDeprecatedToolbar: false

    width: units.gu(100)
    height: units.gu(76)

    Page {
        title: i18n.tr("LIFX for Ubuntu Tøtsj")

        GlowControl {
            id: glowcontrol

            // onDiscoveryEnded: {
            //     console.warn('discov end');
            //     activity.running = false;
            // }
        }

        Column {

            anchors {
                fill: parent
            }

            Repeater {
                model: glowcontrol.bulbs

                ListItem.Standard {
                    text: modelData.label
                    control: Button {
                        property bool powered: modelData.power
                        id: b
                        iconName: 'system-shutdown'
                        opacity: powered ? 1 : 0.5
                        onClicked: modelData.power = !powered
                        width: units.gu(5)
                    }

                    Connections {
                        target: modelData
                        onPowerChanged: b.powered = power
                    }
                }
            }
        }
    }
}

