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

            Rectangle {
                 Layout.fillWidth: true
                 Layout.fillHeight: true
                 color: "#EBEBEB"

                 ListView {
                     id: listView
                     focus: true
                     anchors.fill: parent
                     anchors.leftMargin: 8
                     anchors.rightMargin: 8
                     anchors.topMargin: 8
                     anchors.bottomMargin: 2
                     boundsBehavior:Flickable.StopAtBounds
                     maximumFlickVelocity: 2500
                     spacing: 10
                     cacheBuffer: 300
                     clip: true
                     model: page.saleWarePanel.model
                     delegate: Rectangle {
                         width: parent.width
                         height: 100
                         color: "white"
                         radius: 8
                         clip: true

                         property bool showHideArea: false

                         property bool moveShowArea: false
                         MouseArea {
                             id: swipeArea
                             property int originX: 0
                             anchors.fill: parent
                             onPressed: {
                                 swipeArea.originX = mouse.x
                             }
                             onPositionChanged: {
                                 if (mouse.x - swipeArea.originX < -10) {
                                     moveShowArea = true
                                 }
                                 else if(mouse.x - swipeArea.originX > 10){
                                     moveShowArea = false
                                 }
                             }
                         }

                         Rectangle {
                            id: wareContent
                            width: parent.width
                            height: parent.height
                            radius: 8
                            z: 1
                            onXChanged: {
                                if(0 === x){
                                    showHideArea = false
                                }
                                else{
                                    showHideArea = true
                                }
                            }

                            RowLayout {
                               anchors.fill: parent
                               anchors.margins: 8

                               Item {
                                   Layout.preferredWidth: 72
                                   Layout.fillHeight: true

                                   Image {
                                       id: wareImage
                                       anchors.centerIn: parent
                                       width: parent.width
                                       source: "qrc:/Resources/Images/default_ware.svg"
                                   }

                                  Rectangle {
                                     anchors.centerIn: parent
                                     width: wareImage.width - 8
                                     height: 20
                                     radius: 10
                                     visible: disable
                                     color: "#FF0000"
                                     opacity: 0.7

                                     Text {
                                         anchors.centerIn: parent
                                         font.pixelSize: 12
                                         font.family: UIConfig.fontFamily
                                         font.weight: Font.Bold
                                         horizontalAlignment: Text.AlignHCenter
                                         verticalAlignment: Text.AlignVCenter
                                         color: "#FFFFFF"
                                         text: "库存不足"
                                     }
                                  }
                               }

                               Item {
                                   Layout.fillHeight: true
                                   Layout.fillWidth: true

                                   ColumnLayout {
                                       anchors.fill: parent

                                       Text {
                                           id: wareNameText
                                           Layout.fillWidth: true
                                           leftPadding: 4
                                           font.pixelSize: 15
                                           font.family: UIConfig.fontFamily
                                           font.weight: Font.Bold
                                           wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                                           elide: Text.ElideRight
                                           maximumLineCount: 2
                                           horizontalAlignment: Text.AlignLeft
                                           verticalAlignment: Text.AlignVCenter
                                           color: gifts ? "#FF00FF" : "#0F172A"
                                           text: gifts ? "(赠品)" + name : name
                                       }

                                       Text {
                                           visible: disable
                                           Layout.fillWidth: true
                                           Layout.topMargin: -10
                                           leftPadding: 4
                                           font.pixelSize: 12
                                           font.family: UIConfig.fontFamily
                                           font.weight: Font.Medium
                                           horizontalAlignment: Text.AlignLeft
                                           verticalAlignment: Text.AlignTop
                                           color: "#D3D3D3"
                                           text: "当前剩余库存：" + stock
                                       }

                                       Item {
                                           id: bottom
                                           Layout.fillWidth: true
                                           Layout.preferredHeight: 30

                                           Item {
                                               anchors.left: parent.left
                                               width: parent.width / 2
                                               height: parent.height

                                               Text {
                                                   id: warePriceText
                                                   anchors.left: parent.left
                                                   width: parent.width
                                                   height: parent.height
                                                   leftPadding: 4
                                                   font.pixelSize: 10
                                                   font.weight: Font.Bold
                                                   font.family: UIConfig.fontFamily
                                                   horizontalAlignment: Text.AlignLeft
                                                   verticalAlignment: Text.AlignBottom
                                                   color: "#FF0000"
                                                   textFormat: Text.RichText
                                                   text: "%1<span style='font-size:14px'>%2</span>".arg("￥").arg(price)
                                               }

                                               MouseArea {
                                                   anchors.fill: parent
                                                   onClicked: {
                                                       if(!gifts)
                                                       {
                                                           page.controller.editWarePrice(index)
                                                       }
                                                   }
                                               }
                                           }


                                           Item {
                                               width: parent.width / 2
                                               height: parent.height
                                               anchors.right: parent.right
                                               RowLayout {
                                                   anchors.fill: parent

                                                   // 减按钮
                                                   UIImageButton {
                                                       borderColor: "#FF0000"
                                                       Layout.preferredWidth: implicitWidth
                                                       Layout.fillHeight: true
                                                       imageSource: "qrc:/Resources/Images/reduce.svg"
                                                       imageHoverSource: "qrc:/Resources/Images/reduce_pressed.svg"
                                                       imagePressSource: "qrc:/Resources/Images/reduce_pressed.svg"
                                                       imageWidth: 30
                                                       imageHeight: 30
                                                       onSignalClicked: {
                                                           page.controller.reduceWareCount(index)
                                                       }
                                                   }

                                                   Item {
                                                       Layout.fillWidth: true
                                                       Layout.fillHeight: true

                                                       Connections {
                                                           target: mainWindow
                                                           onShowNumPadChanged: {
                                                               if(!mainWindow.showNumPad && countInput.visible)
                                                               {
                                                                   countInput.visible = false
                                                                   countText.visible = true
                                                               }
                                                           }
                                                       }

                                                       Rectangle {
                                                           id: rectangle
                                                           color: "#EFF1F6"
                                                           border.color: "#F5F5F5"
                                                           border.width: 1
                                                           anchors.fill: parent
                                                           radius: 2

                                                           Text {
                                                               id: countText
                                                               anchors.centerIn: parent
                                                               font.pixelSize: 12
                                                               font.weight: Font.Bold
                                                               font.family: UIConfig.fontFamily
                                                               horizontalAlignment: Text.AlignHCenter
                                                               verticalAlignment: Text.AlignVCenter
                                                               text: count
                                                               visible: !countInput.visible
                                                               color:"#0F172A"
                                                           }
                                                       }


                                                       UITextInput {
                                                           id: countInput
                                                           anchors.fill: parent
                                                           validator: RegExpValidator {regExp: /^[0-9]+$/}
                                                           leftMargin: 0
                                                           rightMargin: 0
                                                           maximumLength: 3
                                                           font.pixelSize: 12
                                                           font.family: UIConfig.fontFamily
                                                           font.weight: Font.Bold
                                                           horizontalAlignment: TextInput.AlignHCenter
                                                           activeFocusOnPress: false
                                                           visible: false
                                                           showClear: false
                                                           onSignalOnFocusChanged: {
                                                               if(!focus){
                                                                   mainWindow.showNumPad = false
                                                               }
                                                           }
                                                           onSignalOnEnterPressed: {
                                                               if(countInput.visible)
                                                               {
                                                                   countInput.visible = false
                                                                   countText.visible = true
                                                                   mainWindow.showNumPad = false
                                                               }
                                                               page.controller.editWareCount(index, parseInt(countInput.text))
                                                           }
                                                       }

                                                       MouseArea {
                                                           anchors.fill: parent
                                                           onClicked: {
                                                               if(countText.visible)
                                                               {
                                                                   listView.currentIndex = index
                                                                   countInput.visible = true
                                                                   countInput.text = countText.text
                                                                   countInput.forceActiveFocus()
                                                                   countText.visible = false
                                                                   mainWindow.showNumPad = true
                                                               }
                                                           }
                                                       }
                                                   }


                                                   // 加按钮
                                                   UIImageButton {
                                                       borderColor: "#FF0000"
                                                       Layout.preferredWidth: implicitWidth
                                                       Layout.fillHeight: true
                                                       imageSource: "qrc:/Resources/Images/increase.svg"
                                                       imageHoverSource: "qrc:/Resources/Images/increase_pressed.svg"
                                                       imagePressSource: "qrc:/Resources/Images/increase_pressed.svg"
                                                       imageWidth: 30
                                                       imageHeight: 30
                                                       onSignalClicked: {
                                                           page.controller.addWareCount(index)
                                                       }
                                                   }
                                               }
                                           }
                                       }
                                   }
                               }
                            }

                            Behavior on x {
                                NumberAnimation  {
                                    duration: 300
                                    easing.type: Easing.InOutQuad
                                }
                            }
                         }

                         Rectangle {
                            id: presentationRec
                            width: 50
                            height: parent.height
                            anchors.right: deleteRec.left
                            visible: showHideArea && !gifts
                            color: "#FF00FF"

                            Text {
                                anchors.centerIn: parent
                                font.pixelSize: 14
                                font.weight: Font.Bold
                                font.family: UIConfig.fontFamily
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                text: "赠送"
                                color: "#FFFFFF"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    page.controller.editWarePresentation(index)
                                    moveShowArea = false
                                }
                            }
                         }

                         Rectangle {
                            id: deleteRec
                            width: 50
                            height: parent.height
                            anchors.right: parent.right
                            visible: showHideArea
                            color: "red"

                            Rectangle {
                                width: parent.width / 2
                                height: parent.height
                                anchors.right: parent.right
                                color: "red"
                                radius: 8

                            }
                            Text {
                                anchors.centerIn: parent
                                font.pixelSize: 14
                                font.weight: Font.Bold
                                font.family: UIConfig.fontFamily
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                text: "删除"
                                color: "#FFFFFF"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    page.controller.deleteWareItem(index)
                                    showHideArea = false
                                }
                            }
                         }

                         // Binding to update the x position of area1 based on showTwoAreas
                         Binding {
                             target: wareContent
                             property: "x"
                             value: moveShowArea ? -(deleteRec.width + (!gifts ? presentationRec.width : 0)) : 0
                             when: moveShowArea
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

            Item {
                id: numRec
                Layout.fillWidth: true
                Layout.preferredHeight: (root.height * 0.4) - 101
                visible: mainWindow.showNumPad
            }

            Rectangle {
                 Layout.fillWidth: true
                 Layout.preferredHeight: 50
                 color: "#FFFFFF"
                 visible: listView.count > 0

                 // 直接扣减库存选项
                 CheckBox {
                     id: chkuat
                     visible: false
                     anchors.left: parent.left
                     anchors.top: parent.top
                     anchors.topMargin: -4
                     width: 150
                     height: 16
                     spacing: 4
                     z: 1

                     indicator: Image {
                         x: chkuat.leftPadding
                         y: chkuatText.height / 2 - 2
                         width: 16
                         height: 16
                         source: chkuat.checked ? "qrc:/Resources/Images/box_checked.svg" : "qrc:/Resources/Images/box_unchecked.svg"
                     }

                     contentItem: Text {
                         id: chkuatText
                         text: "直接扣减库存"
                         height: parent.height
                         font.pixelSize: 10
                         font.family: UIConfig.fontFamily
                         font.weight: Font.Medium
                         color: "#000000"
                         verticalAlignment: Text.AlignVCenter
                         leftPadding: chkuat.indicator.width + chkuat.spacing
                     }

                     onClicked: {}
                 }

                 UIButton {
                     id: deleteErrorWaresButton
                     visible: page.hasTradeWare
                     width: 90
                     height: 20
                     anchors.verticalCenter:parent.verticalCenter
                     anchors.left: parent.left
                     anchors.leftMargin: 8
                     radius: 10
                     text: "删除异常商品"
                     textColor: "#FFFFFF"
                     fontWeight: Font.Bold
                     fontSize: 12
                     color: "#315EFB"
                     onClicked: {
                        page.controller.deleteErrorWares()
                     }
                 }

                 Item {
                    height: parent.height
                    width: totalPrice.implicitWidth
                    anchors.right: submitButton.left
                    anchors.rightMargin: 8

                    Text {
                        id: totalPrice
                        height: parent.height / 2
                        font.pixelSize: 10
                        font.family: UIConfig.fontFamily
                        font.weight: Font.Medium
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignBottom
                        textFormat: Text.RichText
                        text: "合计: <span style='font-size:10px; color:red; font-weight: normal'>%1</span><span style='font-size:12px; color:red; font-weight: bold'>%2</span>".arg("￥").arg((page.orderAmount / 100).toFixed(2))
                    }

                    Text {
                        id: promotionPrice
                        anchors.top: totalPrice.bottom
                        height: parent.height / 2
                        font.pixelSize: 8
                        font.family: UIConfig.fontFamily
                        font.weight: Font.Medium
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignTop
                        textFormat: Text.RichText
                        color: "#808080"
                        text: "优惠减:￥" + (page.promotionAmount / 100).toFixed(2)
                    }

                 }

                 UIButton {
                     id: submitButton
                     width: 80
                     height: 30
                     anchors.verticalCenter:parent.verticalCenter
                     anchors.right: parent.right
                     anchors.rightMargin: 8
                     radius: 15
                     text: "结算"
                     textColor: "#FFFFFF"
                     fontWeight: Font.Bold
                     fontSize: 14
                     color: "#315EFB"

                     onClicked: {
                         root.showCheckoutOrderPanel()
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

    /** 显示修改单价面板 */
    function showPriceEditPanel(data) {
        var json = {
            "userData": data,
            "accept": function(index, price) {
                page.controller.changeWarePrice(index, price)
            }
        }

        var component = Qt.createComponent("qrc:/Qml/Component/PriceEditDialog.qml")
        if(component.status === Component.Ready) {
            var object = component.createObject(root, json)
            object.open()
        } else {
            console.error(component.errorString())
        }
    }

    /** 显示检查订单面板 */
    function showCheckoutOrderPanel() {
        var json = {
            "userData": {"orderAmount": page.orderAmount, "promotionAmount": page.promotionAmount},
            "accept": function(returnData) {
                page.controller.submitOrder(returnData.wholeOrderPromoAmt, returnData.deliveryFeesAmt, returnData.remarkText)
            }
        }
        var component = Qt.createComponent("qrc:/Qml/Component/CheckoutOrderPanel.qml")
        if(component.status === Component.Ready) {
            var object = component.createObject(parent, json)
            object.anchors.fill = parent
            object.open()
        } else {
            console.error(component.errorString())
        }
    }

    /** 显示单品赠送面板 */
    function showWarePresentationPanel(data) {
        if((!data["showEditPresentationNum"])){
            page.controller.warePresentation(data.index)
        }
        else{
            var json = {
                "userData": data,
                "accept": function(index, maxPresentationNum, presentationNum) {
                    page.controller.warePresentation(index, maxPresentationNum, presentationNum)
                }
            }

            var component = Qt.createComponent("qrc:/Qml/Component/WarePresentationDialog.qml")
            if(component.status === Component.Ready) {
                var object = component.createObject(root, json)
                object.open()
            } else {
                console.error(component.errorString())
            }
        }
    }
}
