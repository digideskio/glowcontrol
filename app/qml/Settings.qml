import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.0 as ListItem

Page {
    id: settingsPage

    title: i18n.tr("Settings")

    Column {

        anchors {
            left: parent.left
            right: parent.right
        }

        ListItem.Header { text: i18n.tr("About") }

        ListItem.Standard {
            text: i18n.tr("Help & Support")
            progression: true
            onClicked: Qt.openUrlExternally("https://github.com/semafor/glowcontrol");
        }

        ListItem.Standard {
            text: i18n.tr("Demo")
            progression: true
            onClicked: pageStack.push(Qt.resolvedUrl("Demo.qml"))
        }

        ListItem.Caption {
            text: i18n.tr(
                "GlowControl for LIFX® v%1."
            ).arg(glowcontrol.version)
        }
    }
}
