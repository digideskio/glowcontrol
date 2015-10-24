import QtQuick 2.1
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Glowcontrol 1.0
// import Ubuntu.Components.ListItems 1.2 as ListItem

Rectangle {

    signal colorSelected(var color);

    id: bulbMore
    color: "white"

    RowLayout {
        id: row
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - units.gu(2)

        ColumnLayout {
            spacing: units.gu(1)
            Layout.fillWidth: true

            Repeater {
                id: whites
                model: [
                    "#FDF5E6", // oldlace
                    "#FAEBD7", // antiquewhite
                    "#FFE4B5", // moccasin
                    "#F5DEB3", // wheat
                    "#DEB887"  // burlywood
                ]
                delegate: Rectangle {
                    color: modelData
                    Layout.minimumHeight: units.gu(3)
                    Layout.fillWidth: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var cToKelvin = {
                                "#FDF5E6": 9000,
                                "#FAEBD7": 5000,
                                "#FFE4B5": 4500,
                                "#F5DEB3": 3000,
                                "#DEB887": 2500
                            };
                            colorSelected({
                                kelvin: cToKelvin[modelData],
                                saturation: 0
                            });
                        }
                    }
                }
            }
        }

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

                        MouseArea {
                            anchors.fill: parent
                            onClicked: colorSelected(
                                LifxHelper.colorToHSKB(p.lcolor, parent.opacity)
                            )
                        }
                    }
                }
            }
        }
    }
}
