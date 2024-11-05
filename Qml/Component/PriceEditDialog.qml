import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import Component 1.0
import Other 1.0

/**
 * 单品改价面板
 *
 * 用户参数:
 * userData.index 商品索引
 * userData.name 商品名
 * userData.code 商品编码
 * userData.price 商品原价（分）
 * userData.discountPrice 商品促销价（分）
 */

UIDialog {
    id: root

    /** 用户参数 */
    property var userData: null

    /** 确认回调 */
    property var accept: null

    /** 取消回调 */
    property var reject: null

    /** 改价后价格 */
    property var __changedPrice: 0

    width: parent.width
    height: parent.height
    color: "transparent"
    Loader {
        id: loader
        anchors.centerIn: parent
        sourceComponent: content
        width: root.width
        height: root.height * 0.7
    }

    onAccepted: function() {
        root.close()
        if(root.accept && typeof root.accept === 'function')
            root.accept(userData.index, root.__changedPrice)
    }

    onRejected: function() {
        root.close()
        if(root.reject && typeof root.reject === 'function')
            root.reject()
    }


    Component {
        id: content

        Rectangle {
            id: msgbk
            width: root.width
            height: root.height * 0.7
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
                        text: "单品改价"
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

                // 商品名称
                Text {
                    id: wareName
                    Layout.preferredHeight: 20
                    Layout.fillWidth: true
                    Layout.leftMargin: 12
                    font.pixelSize: 14
                    font.family: UIConfig.fontFamily
                    font.weight: Font.Bold
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideRight
                    color: "#000000"
                    text: userData.name
                }

                // 码
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 18
                    Layout.leftMargin: 12
                    Layout.topMargin: 8
                    // ID
                    RowLayout {
                        anchors.fill: parent
                        spacing: 4

                        //条码标记
                        Rectangle {
                            implicitWidth: 18
                            implicitHeight: 18
                            color: "#E6F9F0"
                            radius: 2

                            Text {
                                anchors.centerIn: parent
                                text: "#"
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Normal
                                color: "#0FC269"
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        Text {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter
                            font.family: UIConfig.fontFamily
                            font.pixelSize: 10
                            font.weight: Font.Bold
                            maximumLineCount: 1
                            elide: Text.ElideRight
                            color: "#616E80"
                            text: userData.code
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            clip: true
                        }
                    }
                }

                // 价格
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    Layout.rightMargin: 12
                    Layout.leftMargin: 12
                    Layout.topMargin: 8

                    RowLayout {
                        anchors.fill:parent
                        spacing: 8

                        // 执行价
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight:true
                            color: "#FFF7ED"
                            radius: 2

                            Text {
                                id: idDiscountPrice
                                anchors.fill: parent
                                anchors.topMargin: 2
                                anchors.leftMargin: 8

                                text: "折扣价￥：" + (userData.discountPrice / 100).toFixed(2)
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Bold
                                color: "#616E80"
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        // 原价
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight:true
                            color: "#F6F8FB"
                            radius: 2

                            Text {
                                id: idOriginalPrice
                                anchors.fill: parent
                                anchors.topMargin: 2
                                anchors.leftMargin: 8
                                text: "原价￥: " + (userData.price / 100).toFixed(2)
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Bold
                                color: "#616E80"
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                    }
                }

                // 输入价格
                UITextInput {
                    id: warePriceInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    Layout.rightMargin: 12
                    Layout.leftMargin: 12
                    Layout.topMargin: 8
                    color: "#EFF1F6"
                    font.pixelSize: 12
                    font.family: UIConfig.fontFamily
                    font.weight: Font.Bold
                    placeholderText: "请输入修改后的价格"
                    activeFocusOnPress: false
                    validator: RegExpValidator{regExp: /(0|[1-9]\d{0,7})(\.)\d{0,2}$/}

                    onSignalTextChanged: {
                        errorText.text = ""
                    }

                    onSignalOnClicked: {
                        warePriceInput.forceActiveFocus()
                    }

                    onSignalOnEnterPressed: {
                        var input = parseFloat(warePriceInput.text)
                        if("" === warePriceInput.text || isNaN(input))
                        {
                            errorText.text = "请输入有效金额"
                            return
                        }
                        root.__changedPrice = input * 100
                        root.accepted()
                    }
                }

                Connections {
                    target: root
                    onSignalOpened: {
                        warePriceInput.forceActiveFocus()
                    }
                }
                // 错误提示
                Text {
                    id: errorText
                    Layout.preferredHeight: 10
                    Layout.fillWidth: true
                    Layout.topMargin: 4
                    Layout.leftMargin: 12
                    font.family: UIConfig.fontFamily
                    font.pixelSize: 10
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                    color: "#FC323E"
                }

                // 数字键盘
                UINumPad {
                    radius: 8
                    id: numPad
                    color: "white"
                    Layout.topMargin: 4
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.bottomMargin: 8
                    enterKeyText: "确定"
                    cancelBtnText: "清除"
                    onSignalCancelButtonClicked: warePriceInput.text = ""
                }

            }
        }
    }
}
