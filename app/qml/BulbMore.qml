import QtQuick 2.1
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.2
import Glowcontrol 1.0
// import Ubuntu.Components.ListItems 1.2 as ListItem

Rectangle {
    id: bulbMore
    color: "white"

    RowLayout {
        id: row
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - units.gu(2)
        Repeater {
            id: colors
            model: LifxHelper.colors

            delegate:  ColumnLayout {
                id: colorCol
                spacing: units.gu(1)

                Layout.fillWidth: true

                Repeater {
                    id: p
                    property string lcolor: modelData
                    model: 5

                    delegate: Rectangle {
                        color: p.lcolor
                        opacity: Math.max(0.1, (1.0 / p.count) * index)

                        Layout.minimumHeight: units.gu(3)
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }
}
