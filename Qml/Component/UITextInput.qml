import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import "./"
import Other 1.0

// 文本输入框控件
UIFocusArea {
    id:root
    areaRadius: 4

    /** TextField 占位文本 */
    property alias placeholderText: textField.placeholderText

    /** TextField 文本类型 */
    property alias echoMode: textField.echoMode

    /** TextField 正则表达式 */
    property alias validator: textField.validator

    /** TextField 输入文本 */
    property alias text: textField.text

    property alias font: textField.font

    property alias horizontalAlignment: textField.horizontalAlignment

    /** TextField 文本数量限制 */
    property alias maximumLength: textField.maximumLength

    property alias color: rectangle.color

    property alias border: rectangle.border

    /** 文本输入框 */
    property alias textField: textField

    property alias textFieldObjectName: textField.objectName

    property alias activeFocusOnPress: textField.activeFocusOnPress

    /** 密码输入框标识 */
    property bool isPassword: false

    /** 显示清楚按钮 */
    property bool showClear: true

    /** 文本输入框左侧边距 */
    property int leftMargin: 2

    /** 文本输入框右侧边距 */
    property int rightMargin: 4

    /** 焦点变化信号 */
    signal signalOnFocusChanged(bool focus)

    /** 点击信号 */
    signal signalOnClicked()

    /** ESC按键信号 */
    signal signalOnEscapePressed()

    /** 回车按键信号 */
    signal signalOnEnterPressed()

    /** 输入框数据改变信号 */
    signal signalTextChanged()

    Rectangle {
        id: rectangle
        color: "#EFF1F6"
        border.color: "#F5F5F5"
        border.width: 1
        height:40
        radius: 2

        RowLayout{
            anchors.fill: parent
            spacing: 0

            // 输入框
            TextField {
                id: textField
                Layout.alignment: Qt.AlignVCenter
                Layout.leftMargin: leftMargin
                Layout.rightMargin: rightMargin
                Layout.minimumHeight: 24
                Layout.fillWidth: true
                Layout.fillHeight: true
                focus: true
                clip: true
                selectionColor: "#0078D7"
                selectedTextColor: "white"
                horizontalAlignment: TextInput.AlignLeft
                background: Rectangle {
                    border.width: 1
                    opacity: 0
                }
                padding:0
                color:"#0F172A"
                //输入框文本样式
                font {
                    pixelSize: 14//parent.height * 0.35
                    weight:Font.Bold
                }

                onTextChanged: {
                    root.signalTextChanged(text)
                }

                //焦点变化
                onActiveFocusChanged: {
                    if(activeFocus) {
                        selectAll()
                    }
                    root.signalOnFocusChanged(activeFocus)
                }

                Keys.priority: Keys.AfterItem
                Keys.forwardTo: [root]

                // 按下ESC按键
                Keys.onEscapePressed: {
                    signalOnEscapePressed()
                    event.accepted = false
                }

                // 按下回车键
                Keys.onEnterPressed: {
                    signalOnEnterPressed()
                }

                Keys.onReturnPressed: {
                    signalOnEnterPressed()
                }
            }

            // 清空按钮
            UIImageButton {
                id: clearButton
                focusPolicy: Qt.NoFocus
                Layout.alignment: Qt.AlignVCenter
                Layout.rightMargin: 2
                Layout.preferredWidth: 24
                Layout.preferredHeight: 24
                imageWidth:14
                imageHeight:14
                visible: showClear && textField.activeFocus && !textField.readOnly && textField.text.length > 0 //按钮显示/隐藏逻辑
                imageSource: "qrc:/Resources/Images/clear_dark.svg"
                imageHoverSource: "qrc:/Resources/Images/clear_dark.svg"
                imagePressSource: "qrc:/Resources/Images/clear_dark.svg"
                onSignalClicked: textField.text = ""
            }

            // 显示/隐藏密码按钮
            UICheckBox {
                id: eyeButton
                Layout.preferredWidth: 24
                Layout.preferredHeight: 24
                imageWidth:14
                Layout.alignment: Qt.AlignVCenter
                focusPolicy: Qt.NoFocus
                visible: isPassword && textField.text.length > 0 && textField.focus //按钮显示/隐藏逻辑
                checkedImg: "qrc:/Resources/Images/show_pwd_dark.svg"
                unCheckedImg: "qrc:/Resources/Images/hide_pwd_dark.svg"

                // 按钮状态切换
                onSignalChecked: {
                    if(isChecked)
                        textField.echoMode = TextInput.Normal
                    else
                        textField.echoMode = TextInput.Password
                }
            }
        }

        MouseArea {
            id:uiMouseArea
            x: textField.x
            y: textField.y
            width: textField.width
            height: textField.height
            propagateComposedEvents: true

            onClicked: {
                mouse.accepted = false
                signalOnClicked()
            }

            onPressed: {
                mouse.accepted = false
                signalOnClicked()
            }
        }
    }
}

