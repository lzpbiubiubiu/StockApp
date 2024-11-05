import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import Component 1.0
import Other 1.0

/**
 * 单品赠送面板
 *
 * 用户参数:
 * userData.index 商品索引
 * userData.name 商品名
 * userData.code 商品编码
 * userData.maxPresentationNum 最大可赠送数量（与原商品数量相同）
 * userData.presentationNum 赠送数量
 * userData.accept 回调函数 : function(index, maxPresentationNum, presentationNum)
 */

UIDialog {
    id: root

    /** 用户参数 */
    property var userData: null

    /** 确认回调 */
    property var accept: null

    /** 取消回调 */
    property var reject: null

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
            root.accept(userData.index, userData.maxPresentationNum, userData.presentationNum)
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
                        text: "单品赠送"
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
                    Layout.preferredHeight: 15
                    Layout.fillWidth: true
                    Layout.leftMargin: 12
                    Layout.topMargin: 8

                    font.pixelSize: 14
                    font.family: UIConfig.fontFamily
                    font.weight: Font.Bold
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
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

                // 输入赠送数量
                UITextInput {
                    id: countInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    Layout.rightMargin: 12
                    Layout.leftMargin: 12
                    Layout.topMargin: 8
                    color: "#EFF1F6"
                    font.pixelSize: 12
                    font.family: UIConfig.fontFamily
                    font.weight: Font.Bold
                    placeholderText: "请输入商品的赠送数量"
                    maximumLength: 3
                    activeFocusOnPress: false
                    validator: RegExpValidator {regExp: /^[0-9]+$/}

                    onSignalTextChanged: {
                        errorText.text = ""
                    }

                    onSignalOnClicked: {
                        countInput.forceActiveFocus()
                    }
                    onSignalOnEnterPressed: {
                        var input = parseInt(countInput.text)
                        if("" === countInput.text || isNaN(input))
                        {
                            errorText.text = "数量不能为空"
                            return
                        }

                        if(input > root.userData.maxPresentationNum)
                        {
                            errorText.text = "赠送数量不能超过商品数量"
                            return
                        }
                        root.userData.presentationNum = input
                        root.accepted()
                    }
                }

                Connections {
                    target: root
                    onSignalOpened: {
                        countInput.forceActiveFocus()
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
                    Layout.topMargin: 16
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    enterKeyText: "确定"
                    cancelBtnText: "清除"
                    Layout.bottomMargin: 8
                    onSignalCancelButtonClicked: countInput.text = ""
                }
            }
        }
    }
}
