import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import Component 1.0
import Other 1.0

/**
 * ip地址设置面板
 *
 */

UIDialog {
    id: root

    /** 确认回调 */
    property var accept: null

    /** 取消回调 */
    property var reject: null

    /** ip地址 */
    property string serverIp: "127.0.0.1"

    /** 端口 */
    property int serverPort: 9090

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
        {
            root.accept(root.serverIp, root.serverPort)
        }
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
                        text: "服务IP地址设置"
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

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    Layout.rightMargin: 12
                    Layout.leftMargin: 12
                    Layout.topMargin: 8

                    RowLayout {
                        anchors.fill: parent
                        spacing: 8

                        // ip地址
                        Text {
                            Layout.preferredWidth: 40
                            Layout.fillHeight: true
                            font.family: UIConfig.fontFamily
                            font.pixelSize: 14
                            font.weight: Font.Bold
                            color: "#0F172A"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            text: "Ip:"
                        }

                        UITextInput {
                            id: ipInput
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            color: "#EFF1F6"
                            font.pixelSize: 12
                            font.family: UIConfig.fontFamily
                            font.weight: Font.Bold
                            activeFocusOnPress: false
                            text: root.serverIp
                            validator: RegExpValidator { regExp: /^[0-9\.]*$/ }

                            onSignalOnClicked: {
                                ipInput.forceActiveFocus()
                            }

                            onSignalTextChanged: {
                                errorText.text = ""
                                root.serverIp = text
                            }

                            onSignalOnEnterPressed: {
                                if(__checkInput(ipInput, portInput, errorText))
                                {
                                    root.accepted()
                                }
                            }
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    Layout.rightMargin: 12
                    Layout.leftMargin: 12
                    Layout.topMargin: 8

                    RowLayout {
                        anchors.fill: parent
                        spacing: 8

                        // Port端口
                        Text {
                            Layout.preferredWidth: 40
                            Layout.fillHeight: true
                            font.family: UIConfig.fontFamily
                            font.pixelSize: 14
                            font.weight: Font.Bold
                            color: "#0F172A"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            text: "Port:"
                        }

                        UITextInput {
                            id: portInput
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            color: "#EFF1F6"
                            font.pixelSize: 12
                            font.family: UIConfig.fontFamily
                            font.weight: Font.Bold
                            activeFocusOnPress: false
                            text: root.serverPort
                            validator: RegExpValidator {regExp: /^[0-9]+$/}
                            onSignalOnClicked: {
                                portInput.forceActiveFocus()
                            }

                            onSignalTextChanged: {
                                errorText.text = ""
                                root.serverPort = parseInt(text)
                            }

                            onSignalOnEnterPressed: {
                                if(__checkInput(ipInput, portInput, errorText))
                                {
                                    root.accepted()
                                }
                            }
                        }
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

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                // 数字键盘
                UINumPad {
                    radius: 8
                    id: numPad
                    color: "white"
                    Layout.topMargin: 4
                    Layout.preferredHeight: root.height * 0.4
                    Layout.fillWidth: true
                    Layout.bottomMargin: 8
                    enterKeyText: "确定"
                    cancelBtnText: "清除"
                }

            }
        }
    }

    /** 检查输入 */
    function __checkInput(ipInput, portInput)
    {
        var ipRegExp = /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
        var regExp = new RegExp(ipRegExp);

        if (!regExp.test(root.serverIp)) {
            ipInput.forceActiveFocus()
            errorText.text = "请输入有效IP"
            return false
        }
        if (!(root.serverPort > 0)) {
            portInput.forceActiveFocus()
            errorText.text = "请输入有效端口"
            return false
        }
        return true
    }
}
