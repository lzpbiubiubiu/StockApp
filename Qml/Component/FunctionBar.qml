import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import Component 1.0
import Other 1.0

Rectangle {

    // 界面索引
    enum PageIndex {

        /** 首页界面 */
        PAGE_MAIN = 0,

        /** 出库界面 */
        PAGE_SALE,

        /** 入库界面 */
        PAGE_STORAGE,

        /** 报表界面 */
        PAGE_REPORT
    }

    id: root
    implicitHeight: 51
    color: "#F5F5F5"

    Rectangle {
        width: parent.width
        height: 1
        anchors.top: parent.top
    }
    RowLayout {
        width: parent.width
        height: 50
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 4

        Item {
            id: homeItem
            Layout.preferredWidth: (parent.width - 20) / (mainWindow.superUser ? 4 : 3)
            Layout.fillHeight: true

            UILabelButton {
                id: homeButton
                anchors.centerIn: parent
                width: 54
                height:parent.height
                source: FunctionBar.PageIndex.PAGE_MAIN === mainWindow.pageIndex ? "qrc:/Resources/Images/menu_main_s.svg" : "qrc:/Resources/Images/menu_main_n.svg"
                position: "top"
                imageWidth: 20
                imageHeight: 20
                text: "首页"
                fontSize: 11
                spacing: 4
                fontWeight: Font.Bold
                textColor: FunctionBar.PageIndex.PAGE_MAIN === mainWindow.pageIndex ? "#0F62FE" : "#191919"
                onSignalClicked: {
                    mainWindow.switchMainPage()
                }
            }
        }

        Item {
            id: saleItem
            Layout.preferredWidth: (parent.width - 20) / (mainWindow.superUser ? 4 : 3)
            Layout.fillHeight: true

            UILabelButton {
                id: saleButton
                anchors.centerIn: parent
                width: 54
                height:parent.height
                source: FunctionBar.PageIndex.PAGE_SALE === mainWindow.pageIndex ? "qrc:/Resources/Images/menu_sale_s.svg" : "qrc:/Resources/Images/menu_sale_n.svg"
                position: "top"
                imageWidth: 20
                imageHeight: 20
                text: "出库"
                fontSize: 11
                spacing: 4
                fontWeight: Font.Bold
                textColor: FunctionBar.PageIndex.PAGE_SALE === mainWindow.pageIndex ? "#0F62FE" : "#191919"
                onSignalClicked: {
                    mainWindow.switchSalePage()
                }
            }

            //按钮右上角出库商品品数显示
            Rectangle {
                anchors.right: saleButton.right
                anchors.top: saleButton.top
                width: saleText.width > 12 ? saleText.width : 12
                height: 12
                color: "#FF545E"
                visible: mainWindow.saleWareKind > 0

                Text {
                    id: saleText
                    anchors.centerIn: parent
                    text: mainWindow.saleWareKind
                    font.pixelSize: 10
                    font.family: UIConfig.fontFamily
                    font.weight: Font.Bold
                    color: "#FFFFFF"
                }
                radius: 6
            }
        }

        Item {
            id: returnItem
            Layout.preferredWidth: (parent.width - 20) / (mainWindow.superUser ? 4 : 3)
            Layout.fillHeight: true
            visible: mainWindow.superUser

            UILabelButton {
                id: returnButton
                anchors.centerIn: parent
                width: 54
                height:parent.height
                source: FunctionBar.PageIndex.PAGE_STORAGE === mainWindow.pageIndex ? "qrc:/Resources/Images/menu_storage_s.svg" : "qrc:/Resources/Images/menu_storage_n.svg"
                position: "top"
                imageWidth: 20
                imageHeight: 20
                text: "入库"
                fontSize: 11
                spacing: 4
                fontWeight: Font.Bold
                textColor: FunctionBar.PageIndex.PAGE_STORAGE === mainWindow.pageIndex ? "#0F62FE" : "#191919"
                onSignalClicked: {
                    mainWindow.switchStoragePage();
                }
            }

            //按钮右上角入库商品品数显示
            Rectangle {
                anchors.right: returnButton.right
                anchors.top: returnButton.top
                width: returnText.width > 12 ? returnText.width : 12
                height: 12
                color: "#FF545E"
                visible: mainWindow.returnWareKind > 0

                Text {
                    id: returnText
                    anchors.centerIn: parent
                    text: mainWindow.returnWareKind
                    font.pixelSize: 10
                    font.family: UIConfig.fontFamily
                    font.weight: Font.Bold
                    color: "#FFFFFF"
                }
                radius: 6
            }
        }

        Item {
            id: statementItem
            Layout.preferredWidth: (parent.width - 20) / (mainWindow.superUser ? 4 : 3)
            Layout.fillHeight: true

            UILabelButton {
                id: statementButton
                anchors.centerIn: parent
                width: 54
                height:parent.height
                source: FunctionBar.PageIndex.PAGE_REPORT === mainWindow.pageIndex ? "qrc:/Resources/Images/menu_report_s.svg" : "qrc:/Resources/Images/menu_report_n.svg"
                position: "top"
                imageWidth: 20
                imageHeight: 20
                text: "报表"
                fontSize: 11
                spacing: 4
                fontWeight: Font.Bold
                textColor: FunctionBar.PageIndex.PAGE_REPORT === mainWindow.pageIndex ? "#0F62FE" : "#191919"
                onSignalClicked: {
                    mainWindow.switchReportPage();
                }
            }
        }
    }
}
