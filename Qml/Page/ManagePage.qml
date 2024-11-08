import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import Component 1.0
import Other 1.0

Page {
    id: root

    Rectangle {
        id: main
        anchors.fill: parent
        color: "transparent"

        ColumnLayout {
            anchors.fill: parent
            spacing: 16

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            UIButton {
                id: storageBtn
                Layout.preferredWidth: 150
                Layout.preferredHeight: 30
                Layout.alignment: Qt.AlignHCenter
                radius: 15
                text: "商品入库"
                textColor: "#FFFFFF"
                fontWeight: Font.Bold
                fontSize: 12
                color: "#315EFB"
                focusPolicy: Qt.NoFocus

                onClicked: {
                    root.showStoragePanel()
                }
            }

            UIButton {
                Layout.preferredWidth: 150
                Layout.preferredHeight: 30
                Layout.alignment: Qt.AlignHCenter
                radius: 15
                text: "备货订单"
                textColor: "#FFFFFF"
                fontWeight: Font.Bold
                fontSize: 12
                color: "#315EFB"
                focusPolicy: Qt.NoFocus
                onClicked: {
                    root.showOrderStockListPanel()
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

        }
    }

    // 状态栏
    footer: FunctionBar {
        id: functionBar
        implicitHeight: 51

    }

    /** 显示商品入库面板 */
    function showStoragePanel() {
        var component = Qt.createComponent("qrc:/Qml/Component/StoragePanel.qml")
        if(component.status === Component.Ready) {
            var object = component.createObject(parent)
            object.anchors.fill = parent
            object.open()
        } else {
            console.error(component.errorString())
        }
    }

    /** 显示订单备货清单面板 */
    function showOrderStockListPanel() {
        var component = Qt.createComponent("qrc:/Qml/Component/OrderStockListPanel.qml")
        if(component.status === Component.Ready) {
            var object = component.createObject(parent)
            object.anchors.fill = parent
            object.open()
        } else {
            console.error(component.errorString())
        }
    }

}
