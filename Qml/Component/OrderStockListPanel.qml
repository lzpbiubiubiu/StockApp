import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import Component 1.0
import Other 1.0

/**
 * 订单备货清单面板
 */

UIDrawer {
    id: root

    // 页面序号
    enum PageIndex {

        /** 订单备货清单页面 */
        PAGE_ORDER_STOCK_LIST = 0,

        /** 订单详情页面 */
        PAGE_ORDER_DETAIL
    }

    /** 当前页面序号 */
    property int __currentPageIndex: OrderStockListPanel.PageIndex.PAGE_ORDER_STOCK_LIST

    /** 当前订单列表选择序号 */
    property int __currentListIndex: -1

    width: parent.width
    height: parent.height
    color: "transparent"

    edge: bottomEdge
    closeOnClickOutside: true

    //内容区域
    contentItem: OrderStockListPanel.PageIndex.PAGE_ORDER_DETAIL === root.__currentPageIndex ? orderDetailContent : orderContent

    // 订单备货清单
    Component {
        id: orderContent

        Rectangle {
            width: root.width
            height: root.height * 0.95
            radius: 8
            clip: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40

                    Text {
                        anchors.centerIn: parent
                        width: parent.width - 50
                        text: "备货清单"
                        elide: Text.ElideRight
                        font.family: UIConfig.fontFamily
                        font.pixelSize: 14
                        font.weight: Font.Bold
                        color: "#000000"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    UIImageButton {
                        id: cancelButton

                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        width: 40
                        height: 40
                        anchors.verticalCenter: parent.verticalCenter
                        imageSource: "qrc:/Resources/Images/panel_header_close.svg"
                        imageHoverSource: "qrc:/Resources/Images/panel_header_close.svg"
                        imagePressSource: "qrc:/Resources/Images/panel_header_close.svg"

                        onSignalClicked: root.close()
                    }
                }

                Rectangle {
                     Layout.fillWidth: true
                     Layout.fillHeight: true

                     ListView {
                         id: listView
                         focus: true
                         anchors.fill: parent
                         anchors.margins: 4
                         boundsBehavior:Flickable.StopAtBounds
                         maximumFlickVelocity: 2500
                         currentIndex: root.__currentListIndex
                         cacheBuffer: 300
                         clip: true
                         model: [{"receipt": "1234567897", "dateTime": "2024-11-06 12:30:00", "orderAmt": 123.65, "promotionAmt": 14.23, "price": 10},
                             {"receipt": "1234567897", "dateTime": "2024-11-06 12:30:00", "orderAmt": 123.65, "promotionAmt": 14.23, "price": 10},
                             {"receipt": "1234567897", "dateTime": "2024-11-06 12:30:00", "orderAmt": 123.65, "promotionAmt": 14.23, "price": 10},
                             {"receipt": "1234567897", "dateTime": "2024-11-06 12:30:00", "orderAmt": 123.65, "promotionAmt": 14.23, "price": 10}]
                         delegate: Rectangle {
                             width: parent.width
                             height: 70
                             color: index === listView.currentIndex ? "#EFF6FF" : "white"
                             clip: true

                             Rectangle {
                                 width: parent.width
                                 height: parent.height
                                 color: "transparent"

                                 MouseArea{
                                     anchors.fill: parent
                                     onClicked: {
                                         root.__currentListIndex = index
                                         root.__currentPageIndex = OrderStockListPanel.PageIndex.PAGE_ORDER_DETAIL
                                     }
                                 }

                                 ColumnLayout {
                                     anchors.fill: parent
                                     anchors.leftMargin: 8
                                     anchors.rightMargin: 8
                                     spacing: 0

                                     Item {
                                         Layout.fillWidth: true
                                         Layout.preferredHeight: 15
                                         Layout.topMargin: 4
                                         RowLayout {
                                             anchors.fill: parent
                                             Text {
                                                 font.pixelSize: 12
                                                 font.family: UIConfig.fontFamily
                                                 font.weight: Font.Bold
                                                 horizontalAlignment: Text.AlignLeft
                                                 verticalAlignment: Text.AlignVCenter
                                                 color: "#000000"
                                                 text: modelData.receipt
                                             }

                                             Item {
                                                 Layout.fillWidth: true
                                                 Layout.fillHeight: true
                                             }

                                             Text {
                                                 font.pixelSize: 12
                                                 font.family: UIConfig.fontFamily
                                                 font.weight: Font.Bold
                                                 horizontalAlignment: Text.AlignRight
                                                 verticalAlignment: Text.AlignVCenter
                                                 color: "#000000"
                                                 text: "￥" + modelData.orderAmt
                                             }
                                         }
                                     }

                                     Item {
                                         Layout.fillWidth: true
                                         Layout.preferredHeight: 15
                                         Layout.topMargin: 4

                                         RowLayout {
                                             anchors.fill: parent
                                             spacing: 8

                                             Text {
                                                 font.pixelSize: 10
                                                 font.family: UIConfig.fontFamily
                                                 font.weight: Font.Medium
                                                 horizontalAlignment: Text.AlignLeft
                                                 verticalAlignment: Text.AlignVCenter
                                                 color: "#808080"
                                                 text: modelData.dateTime
                                             }

                                             Item {
                                                 Layout.fillWidth: true
                                                 Layout.fillHeight: true
                                             }

                                             Text {
                                                 font.pixelSize: 10
                                                 font.family: UIConfig.fontFamily
                                                 font.weight: Font.Medium
                                                 horizontalAlignment: Text.AlignRight
                                                 verticalAlignment: Text.AlignVCenter
                                                 color: "#808080"
                                                 text: "优惠: " + modelData.promotionAmt
                                             }

                                             Text {
                                                 font.pixelSize: 10
                                                 font.family: UIConfig.fontFamily
                                                 font.weight: Font.Medium
                                                 horizontalAlignment: Text.AlignRight
                                                 verticalAlignment: Text.AlignVCenter
                                                 color: "#808080"
                                                 text: "配送费: " + modelData.price
                                             }
                                         }
                                     }

                                     Rectangle {
                                         Layout.fillWidth: true
                                         Layout.fillHeight: true
                                         color: "transparent"
                                         Layout.bottomMargin: 4

                                         UIButton {
                                             id: submitButton
                                             width: 80
                                             height: 20
                                             anchors.right: parent.right
                                             anchors.verticalCenter: parent.verticalCenter
                                             radius: 10
                                             text: "出库"
                                             textColor: "#FFFFFF"
                                             fontWeight: Font.Bold
                                             fontSize: 12
                                             color: "#315EFB"
                                             focusPolicy: Qt.NoFocus

                                             onClicked: {
                                             }
                                         }
                                     }
                                 }

                                 Rectangle {
                                     anchors.bottom: parent.bottom
                                     width: parent.width
                                     height: 1
                                     color: "#E2E5EB"
                                     visible: index !== listView.currentIndex
                                 }
                             }
                         }

                         // 背景层
                         Rectangle {
                             anchors.fill: parent
                             visible: listView.count <= 0
                             color: "transparent"

                             Image {
                                 id: bkImage
                                 anchors.centerIn: parent
                                 source: "qrc:/Resources/Images/ware_bk.svg"
                             }

                             Text {
                                 id: bkText
                                 text: "购物车为空"
                                 color: "#1E293B"
                                 font.family: UIConfig.fontFamily
                                 font.pixelSize: 14
                                 font.weight: Font.Bold
                                 anchors.top: bkImage.bottom
                                 anchors.topMargin: 8
                                 anchors.horizontalCenter: parent.horizontalCenter
                             }
                         }
                     }

                }
            }
        }
    }

    // 订单详情
    Component {
        id: orderDetailContent

        Rectangle {
            width: root.width
            height: root.height * 0.95
            radius: 8
            clip: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40

                    UIImageButton {
                        id: backButton

                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        imageSource: "qrc:/Resources/Images/back_arraow.svg"
                        imageHoverSource: "qrc:/Resources/Images/back_arraow.svg"
                        imagePressSource: "qrc:/Resources/Images/back_arraow.svg"

                        onSignalClicked: {
                            root.__currentPageIndex = OrderStockListPanel.PageIndex.PAGE_ORDER_STOCK_LIST
                        }
                    }

                    Text {
                        anchors.centerIn: parent
                        width: parent.width - 50
                        text: "清单明细"
                        elide: Text.ElideRight
                        font.family: UIConfig.fontFamily
                        font.pixelSize: 14
                        font.weight: Font.Bold
                        color: "#000000"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    UIImageButton {
                        id: cancelButton

                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        width: 40
                        height: 40
                        anchors.verticalCenter: parent.verticalCenter
                        imageSource: "qrc:/Resources/Images/panel_header_close.svg"
                        imageHoverSource: "qrc:/Resources/Images/panel_header_close.svg"
                        imagePressSource: "qrc:/Resources/Images/panel_header_close.svg"

                        onSignalClicked: root.close()
                    }
                }

                Text {
                    font.pixelSize: 13
                    font.weight: Font.Bold
                    font.family: UIConfig.fontFamily
                    Layout.leftMargin: 8
                    Layout.rightMargin: 8
                    color: "#0F172A"
                    text: "小票号: "
                }

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 90
                    Layout.topMargin: 4
                    Layout.leftMargin: 8
                    Layout.rightMargin: 8

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 4

                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 43

                            RowLayout {
                                anchors.fill: parent
                                spacing: 4

                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    color: "#F6F8FB"
                                    radius: 2

                                    ColumnLayout {
                                        anchors.fill: parent
                                        anchors.margins: 4
                                        spacing: 0

                                        Text {
                                            font.pixelSize: 10
                                            font.weight: Font.Medium
                                            font.family: UIConfig.fontFamily
                                            color: "#0F172A"
                                            text: "订单金额:"
                                        }

                                        Text {
                                            font.pixelSize: 12
                                            font.weight: Font.Bold
                                            font.family: UIConfig.fontFamily
                                            color: "#0F172A"
                                            text: "12.355"
                                        }
                                    }
                                }

                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    color: "#F6F8FB"
                                    radius: 2

                                    ColumnLayout {
                                        anchors.fill: parent
                                        anchors.margins: 4
                                        spacing: 0

                                        Text {
                                            font.pixelSize: 10
                                            font.weight: Font.Medium
                                            font.family: UIConfig.fontFamily
                                            color: "#0F172A"
                                            text: "总数量："
                                        }

                                        Text {
                                            font.pixelSize: 12
                                            font.weight: Font.Bold
                                            font.family: UIConfig.fontFamily
                                            color: "#0F172A"
                                            text: "1111"
                                        }
                                    }
                                }
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 43

                            RowLayout {
                                anchors.fill: parent
                                spacing: 4

                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    color: "#F6F8FB"
                                    radius: 2

                                    ColumnLayout {
                                        anchors.fill: parent
                                        anchors.margins: 4
                                        spacing: 0

                                        Text {
                                            font.pixelSize: 10
                                            font.weight: Font.Medium
                                            font.family: UIConfig.fontFamily
                                            color: "#0F172A"
                                            text: "促销金额: "
                                        }

                                        Text {
                                            font.pixelSize: 12
                                            font.weight: Font.Bold
                                            font.family: UIConfig.fontFamily
                                            color: "#0F172A"
                                            text: "13543"
                                        }
                                    }
                                }

                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    color: "#F6F8FB"
                                    radius: 2

                                    ColumnLayout {
                                        anchors.fill: parent
                                        anchors.margins: 4
                                        spacing: 0

                                        Text {
                                            font.pixelSize: 10
                                            font.weight: Font.Medium
                                            font.family: UIConfig.fontFamily
                                            color: "#0F172A"
                                            text: "配送费："
                                        }

                                        Text {
                                            font.pixelSize: 12
                                            font.weight: Font.Bold
                                            font.family: UIConfig.fontFamily
                                            color: "#0F172A"
                                            text: "12345"
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Text {
                    font.pixelSize: 10
                    font.weight: Font.Bold
                    font.family: UIConfig.fontFamily
                    Layout.topMargin: 8
                    Layout.leftMargin: 8
                    Layout.rightMargin: 8
                    color: "#EA580C"
                    text: "订单商品列表"
                }

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    color: "#FFFFFF"
                    radius: 0

                    ListView {
                        id: listView
                        focus: true
                        anchors.fill: parent
                        anchors.margins: 8
                        boundsBehavior:Flickable.StopAtBounds
                        maximumFlickVelocity: 2500
                        currentIndex: -1
                        cacheBuffer: 300
                        clip: true
                        model: [{"receipt": "1234567897", "dateTime": "2024-11-06 12:30:00", "orderAmt": 123.65, "promotionAmt": 14.23, "price": 10},
                            {"receipt": "1234567897", "dateTime": "2024-11-06 12:30:00", "orderAmt": 123.65, "promotionAmt": 14.23, "price": 10},
                            {"receipt": "1234567897", "dateTime": "2024-11-06 12:30:00", "orderAmt": 123.65, "promotionAmt": 14.23, "price": 10},
                            {"receipt": "1234567897", "dateTime": "2024-11-06 12:30:00", "orderAmt": 123.65, "promotionAmt": 14.23, "price": 10}]
                        delegate: Rectangle {
                            width: parent.width
                            height: 65
                            clip: true

                            Rectangle {
                                width: parent.width
                                height: parent.height
                                color: "transparent"

                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 0

                                    Item {
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 20
                                        Layout.topMargin: 4
                                        RowLayout {
                                            anchors.fill: parent
                                            Text {
                                                font.pixelSize: 12
                                                font.family: UIConfig.fontFamily
                                                font.weight: Font.Bold
                                                horizontalAlignment: Text.AlignLeft
                                                verticalAlignment: Text.AlignVCenter
                                                color: "#000000"
                                                text: "点光炮"
                                            }

                                            Item {
                                                Layout.fillWidth: true
                                                Layout.fillHeight: true
                                            }

                                            Text {
                                                font.pixelSize: 12
                                                font.family: UIConfig.fontFamily
                                                font.weight: Font.Bold
                                                horizontalAlignment: Text.AlignRight
                                                verticalAlignment: Text.AlignVCenter
                                                color: "#000000"
                                                text: "X10"
                                            }
                                        }
                                    }

                                    Text {
                                        font.pixelSize: 10
                                        font.family: UIConfig.fontFamily
                                        font.weight: Font.Medium
                                        horizontalAlignment: Text.AlignLeft
                                        verticalAlignment: Text.AlignVCenter
                                        color: "#808080"
                                        text: "20240001"
                                    }

                                    Item {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true

                                        RowLayout {
                                            anchors.fill: parent
                                            spacing: 8

                                            Text {
                                                font.pixelSize: 10
                                                font.family: UIConfig.fontFamily
                                                font.weight: Font.Medium
                                                horizontalAlignment: Text.AlignLeft
                                                verticalAlignment: Text.AlignVCenter
                                                color: "#808080"
                                                text: "单价:" + "100.00"
                                            }

                                            Item {
                                                Layout.fillWidth: true
                                                Layout.fillHeight: true
                                            }

                                            Text {
                                                font.pixelSize: 10
                                                font.family: UIConfig.fontFamily
                                                font.weight: Font.Medium
                                                horizontalAlignment: Text.AlignRight
                                                verticalAlignment: Text.AlignVCenter
                                                color: "#808080"
                                                text: "优惠: " + modelData.promotionAmt
                                            }

                                            Text {
                                                font.pixelSize: 10
                                                font.family: UIConfig.fontFamily
                                                font.weight: Font.Medium
                                                horizontalAlignment: Text.AlignRight
                                                verticalAlignment: Text.AlignVCenter
                                                color: "#000000"
                                                text: "小计: " + "1234.35"
                                            }
                                        }
                                    }

                                }

                                Rectangle {
                                    anchors.bottom: parent.bottom
                                    width: parent.width
                                    height: 1
                                    color: "#E2E5EB"
                                    visible: index !== listView.currentIndex
                                }
                            }
                        }
                    }

                    // 数字键盘
                    UINumPad {
                        radius: 8
                        id: numPad
                        color: "white"
                        visible: false
                        Layout.preferredHeight: root.height * 0.35
                        Layout.fillWidth: true
                        Layout.bottomMargin: 8
                        enterKeyText: "确定"
                        cancelBtnText: "清除"
                        onSignalCancelButtonClicked: {
                            numPad.visible = false
                            rec.visible = true
                            rec.forceActiveFocus()
                        }
                    }

                    Rectangle {
                        id: rec
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 40
                        z: 1

                        UIButton {
                            id: acceptButton
                            anchors.centerIn: parent
                            height: 24
                            width: 150
                            radius: 12
                            text: "修改订单"
                            textColor: "#FFFFFF"
                            fontWeight: Font.Bold
                            fontSize: 13
                            color: "#315EFB"

                            onClicked: {
                            }
                        }
                    }
                }
            }
        }
    }
}
