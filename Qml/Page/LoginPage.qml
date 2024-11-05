import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import Page 1.0
import Component 1.0
import Other 1.0

/** 登录页 */
Page {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "white"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (!account.containsMouse && !password.containsMouse) {
                    account.focus = false
                    password.focus = false
                }

            }
        }

        Item {
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.topMargin: 16
            width: settingIcon.width + settingText.implicitWidth + 8
            height: settingIcon.height + 8

            // 图标
            Image {
                id:settingIcon
                width: 14
                height: 14
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 4
                source: "qrc:/Resources/Images/setting.svg"
                antialiasing: true
            }

            Text {
                id: settingText
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: settingIcon.right
                anchors.rightMargin: 4
                font.family: UIConfig.fontFamily
                font.pixelSize: 12
                font.weight: Font.Medium
                color: "#000000"
                text: "设置"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    page.controller.editAppIp()
                }
            }
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.rightMargin: 12
            enabled: page.loginEnabled
            spacing: 0

            // 图标
            Image {
                id: icon
                width: implicitWidth
                height: implicitHeight
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 42
                source: "qrc:/Resources/Images/user_icon.svg"
                antialiasing: true
            }

            // 账号
            Text {
                Layout.leftMargin: 16
                Layout.topMargin: 12
                font.family: UIConfig.fontFamily
                font.pixelSize: 14
                font.weight: Font.Bold
                color: "#0F172A"
                text: "账号"
            }

            // 账号输入框
            UITextInput {
                id: account
                placeholderText: "请输入账号"
                Layout.leftMargin: 16
                Layout.topMargin: 4
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                font.pixelSize: 12
                font.family: UIConfig.fontFamily
                font.weight: Font.Bold
                validator: RegExpValidator { regExp: /^[a-zA-Z0-9]*$/ }
                maximumLength: 20
                echoMode: TextInput.Normal
                text: page.lastUserAccount
            }

            // 密码
            Text {
                Layout.topMargin: 8
                Layout.leftMargin: 16
                font.family: UIConfig.fontFamily
                font.pixelSize: 14
                font.weight: Font.Bold
                color: "#0F172A"
                text: "密码"
            }

            // 密码输入框
            UITextInput {
                id: password
                placeholderText: "请输入密码"
                Layout.topMargin: 4
                Layout.leftMargin: 16
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                font.pixelSize: 12
                font.family: UIConfig.fontFamily
                font.weight: Font.Bold
                maximumLength: 20
                validator: RegExpValidator { regExp: /^[a-zA-Z0-9]*$/ }
                isPassword: true
                echoMode: TextInput.Password
            }

            UIButton {
                id: loginButton
                Layout.preferredHeight: 30
                Layout.preferredWidth: 100
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20
                radius: 15
                text: "登录"
                textColor: "#FFFFFF"
                fontWeight: Font.Bold
                fontSize: 14
                enabled: "" !== account.text && "" !== password.text
                color: "#315EFB"

                onClicked: {
                    page.controller.login(account.text, password.text)
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }


        }
    }

    /** 显示设置面板 */
    function showSettingPanel(ip, port) {
        var json = {
            "serverIp": ip,
            "serverPort": port,
            "accept": function(ip, port) {
                page.controller.setAppIp(ip, port)
            }
        }
        var component = Qt.createComponent("qrc:/Qml/Component/SettingIpDialog.qml")
        if(component.status === Component.Ready) {
            var object = component.createObject(root, json)
            object.open()
        } else {
            console.error(component.errorString())
        }
    }
}
