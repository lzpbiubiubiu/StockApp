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
            id: basicContent
            anchors.fill: parent
            spacing: 2

            Rectangle {
                 Layout.fillWidth: true
                 Layout.fillHeight: true
                 color: "#EBEBEB"

                 // 图标
                 AnimatedImage {
                     id: icon
                     width: 80
                     height: 80
                     source: "qrc:/Resources/Images/loading.gif"
                     smooth: true
                     visible: false
                     anchors.centerIn: parent

                 }

                 Image {
                     id: errorImage
                     source: "qrc:/Resources/Images/ware_bk.svg"
                     visible: 0 === wareView.count
                     anchors.centerIn: parent
                 }

                 //错误提示
                 Text {
                     id: errorText
                     visible: 0 === wareView.count
                     anchors.top: errorImage.bottom
                     anchors.horizontalCenter: parent.horizontalCenter
                     font.family: UIConfig.fontFamily
                     font.pixelSize: 18
                     font.weight: Font.Medium
                     color: "#1E293B"
                     horizontalAlignment: Text.AlignHCenter
                     maximumLineCount: 5
                     wrapMode: Text.WordWrap
                     elide: Text.ElideRight
                     text: "获取商品失败"
                 }

                 GridView {
                     id: wareView

                     property bool headerVisible: false
                     property bool headerHold: false
                     property real moveStartContentY: 0

                     anchors.fill: parent
                     anchors.margins: 4
                     //visible: !page.fetching
                     currentIndex: 0
                     clip: true
                     boundsBehavior:Flickable.DragOverBounds
                     cellWidth: width/ 2
                     cellHeight: 180
                     model: page.stockWarePanel.model
                     delegate: Rectangle {
                         width: wareView.cellWidth
                         height: wareView.cellHeight
                         color: "transparent"

                         Rectangle {
                             anchors.fill: parent
                             anchors.margins: 4
                             color: "white"
                             radius: 8

                             ColumnLayout {
                                 anchors.fill: parent
                                 spacing: 4
                                 anchors.margins: 4

                                 Item {
                                     Layout.fillWidth: true
                                     Layout.preferredHeight: 102

                                     Image {
                                         id: wareImage
                                         anchors.fill: parent
                                         //source: "qrc:/Resources/Images/default_ware.svg"
                                         source: page.stockWarePanel.loadLocalWare(imgUrl)
                                     }

                                     // 商品编码码
                                     Item {
                                         width: wareImage.width
                                         height: 14
                                         anchors.bottom: wareImage.bottom
                                         anchors.bottomMargin: 2

                                         // ID
                                         RowLayout {
                                             anchors.fill: parent
                                             spacing: 4

                                             //条码标记
                                             Rectangle {
                                                 Layout.leftMargin: 2
                                                 implicitWidth: 14
                                                 implicitHeight: 14
                                                 color: "#E6F9F0"
                                                 radius: 2

                                                 Text {
                                                     id: title
                                                     anchors.centerIn: parent
                                                     text: "#"
                                                     font.pixelSize: 11
                                                     font.family: UIConfig.fontFamily
                                                     font.weight: Font.Normal
                                                     color: "#0FC269"
                                                     verticalAlignment: Text.AlignVCenter
                                                 }
                                             }

                                             Text {
                                                 visible: false
                                                 Layout.fillHeight: true
                                                 Layout.fillWidth: true
                                                 Layout.alignment: Qt.AlignVCenter
                                                 font.family: UIConfig.fontFamily
                                                 font.pixelSize: 8
                                                 font.weight: Font.Normal
                                                 maximumLineCount: 1
                                                 elide: Text.ElideRight
                                                 color: "#0FC269"
                                                 text: code
                                                 horizontalAlignment: Text.AlignLeft
                                                 verticalAlignment: Text.AlignVCenter
                                                 clip: true
                                             }
                                         }
                                     }
                                 }

                                 Item {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 20

                                    Text {
                                        id: wareNameText
                                        anchors.left: parent.left
                                        leftPadding: 4
                                        width: parent.width * 2 / 3
                                        height: parent.height
                                        font.pixelSize: 12
                                        font.weight: Font.Bold
                                        font.family: UIConfig.fontFamily
                                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                                        elide: Text.ElideRight
                                        maximumLineCount: 1
                                        horizontalAlignment: Text.AlignLeft
                                        verticalAlignment: Text.AlignVCenter
                                        color: "#0F172A"
                                        text: name
                                    }

                                    Text {
                                        anchors.right: parent.right
                                        width: parent.width * 1 / 3
                                        height: parent.height
                                        rightPadding: 4
                                        font.pixelSize: 12
                                        font.weight: Font.Bold
                                        font.family: UIConfig.fontFamily
                                        horizontalAlignment: Text.AlignRight
                                        verticalAlignment: Text.AlignVCenter
                                        color: disable ? "red" : "#0F172A"
                                        text: disable ? "库存不足" : "X" + stock
                                    }
                                 }

                                 Item {
                                     Layout.fillWidth: true
                                     Layout.fillHeight: true

                                     Item {
                                         width: (parent.width - 8) / 2
                                         height: parent.height
                                         anchors.left: parent.left

                                         ColumnLayout {
                                            anchors.fill: parent

                                            Text {
                                                Layout.fillWidth: true
                                                Layout.fillHeight: true
                                                leftPadding: 4
                                                font.pixelSize: 11
                                                font.weight: Font.Bold
                                                font.family: UIConfig.fontFamily
                                                horizontalAlignment: Text.AlignLeft
                                                verticalAlignment: Text.AlignVCenter
                                                color: "red"
                                                text: wholesalePrice

                                            }

                                            UIButton {
                                                Layout.fillWidth: true
                                                Layout.preferredHeight: 25
                                                Layout.alignment: Qt.AlignHCenter
                                                text: "批发"
                                                fontSize: 11
                                                fontWeight: Font.Bold
                                                enabled: !disable
                                                textColor: "#315EFB"
                                                color: "#EFF6FF"
                                                pressColor: "#BFDBFE"
                                                onClicked: {
                                                    page.controller.wholesaleWare(index)
                                                }
                                            }

                                         }

                                     }

                                     Item {
                                         width: (parent.width - 8) / 2
                                         height: parent.height
                                         anchors.leftMargin: 8
                                         anchors.right: parent.right

                                         ColumnLayout {
                                            anchors.fill: parent

                                            Text {
                                                Layout.fillWidth: true
                                                Layout.fillHeight: true
                                                font.pixelSize: 11
                                                rightPadding: 4
                                                font.weight: Font.Bold
                                                font.family: UIConfig.fontFamily
                                                horizontalAlignment: Text.AlignRight
                                                verticalAlignment: Text.AlignVCenter
                                                color: "red"
                                                text: retailPrice

                                            }

                                            UIButton {
                                                Layout.fillWidth: true
                                                Layout.preferredHeight: 25
                                                Layout.alignment: Qt.AlignHCenter
                                                text: "零售"
                                                enabled: !disable
                                                fontSize: 11
                                                fontWeight: Font.Bold
                                                textColor: "#315EFB"
                                                color: "#EFF6FF"
                                                pressColor: "#BFDBFE"
                                                onClicked: {
                                                    page.controller.retailWare(index)
                                                }

                                            }

                                         }

                                     }
                                 }
                             }
                         }

                         // 禁用遮罩层
                         Rectangle {
                             anchors.fill: parent
                             color: "#FBFBFB"
                             visible: false//disable
                             opacity: 0.5
                         }
                     }
                     onHeaderVisibleChanged: if(!headerVisible) {headerHold = false}
                     onContentYChanged: {
                        if(dragging && atYBeginning && !headerVisible ){
                            headerVisible = true
                         }
                     }

                     onDraggingChanged: {
                        if(dragging){
                            console.log("pullStart")
                            moveStartContentY = contentY
                        }
                        else{
                            if(atYBeginning && headerVisible) {
                                headerHold = true
                            }else if(null !== headerItem){
                                headerVisible = false
                                headerHold = false
                            }
                        }
                     }

                     header: headerVisible ? cmpHeader : null

                     onHeaderHoldChanged:{
                         if(headerHold)
                             page.controller.refreshWares()
                     }

                     Component{
                         id: cmpHeader
                         Rectangle{
                             color: "transparent"
                             width: wareView.width
                             height: 25

                             Text {
                                 anchors.centerIn: parent
                                 font.family: UIConfig.fontFamily
                                 font.pixelSize: 14
                                 font.weight: Font.Medium
                                 color: "#1E293B"
                                 text: wareView.headerHold ? "刷新中......" : "松开刷新"
                             }
                         }
                     }

                     Connections {
                        target: page
                        onFetchingChanged:{
                            if(!page.fetching && wareView.headerVisible)
                            {
                                wareView.headerVisible = false
                            }
                        }
                     }
                 }
            }
        }

    }

    // 状态栏
    footer: FunctionBar {
        id: functionBar
        implicitHeight: 51

    }
}
