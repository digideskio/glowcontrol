import QtQuick 2.0
import Ubuntu.Components 1.3
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

    GlowControl {
        id: glowcontrol
    }

    Connections {
        target: Qt.application
        onActiveChanged:
            if (Qt.application.active) {
                glowcontrol.gainApplicationFocus();
            } else {
                glowcontrol.loseApplicationFocus();
            }
    }

    PageStack {
        id: pageStack
        Component.onCompleted: push(Qt.resolvedUrl("Bulbs.qml"))

        anchors {
            fill: undefined // unset the default to make the other anchors work
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
    }
}
