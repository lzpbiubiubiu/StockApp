import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Component 1.0
import Other 1.0

UIDrawer {
    id: root

    // 输入类型
    enum InputType {

        /** 单行输入 */
        TYPE_TEXT_INPUT = 0,

        /** 多行输入 */
        TYPE_TEXT_AREA
    }

    /** 确认回调 */
    property var accept: null

    /** 取消回调 */
    property var reject: null

    /** 标题信息 */
    property string panelTitle: ""

    /** 输入框信息 */
    property string panelText: ""

    /** 输入类型 */
    property int inputType: InputTextPanel.InputType.TYPE_TEXT_INPUT

    edge: bottomEdge
    onSignalClosed:{
        if(root.reject && typeof root.reject === 'function')
            root.reject()
    }

    //内容区域
    contentItem: {
        if(InputTextPanel.InputType.TYPE_TEXT_AREA === root.inputType){
            areaContent
        }
        else{
            textcontent
        }
    }

    Component {
        id: textcontent

        Rectangle {
            id: textRec
            width: root.width
            height: root.height * 0.75
            radius: 8
            clip: true
            color: "#FFFFFF"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (!textInput.containsMouse) {
                        textInput.focus = false
                        itemId.visible = false
                    }

                }
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 1

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40

                    Text {
                        anchors.centerIn: parent
                        width: parent.width - 50
                        text: root.panelTitle
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

                        onSignalClicked: {
                            root.close()
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: 16
                        spacing: 0

                        UITextInput {
                            id: textInput
                            Layout.fillWidth: true
                            Layout.preferredHeight: 30
                            Layout.rightMargin: 12
                            Layout.leftMargin: 12
                            color: "#EFF1F6"
                            font.pixelSize: 12
                            font.family: UIConfig.fontFamily
                            font.weight: Font.Bold
                            placeholderText: "请输入..."
                            text: panelText
                            //validator: RegExpValidator{regExp: /(0|[1-9]\d{0,7})(\.)\d{0,2}$/}

                            //焦点变化
                            onActiveFocusChanged: {
                                if(activeFocus)
                                    itemId.visible = true
                                else
                                    itemId.visible = false
                            }

                            onSignalOnEnterPressed: {
                                root.close()
                                if(root.accept && typeof root.accept === 'function')
                                    root.accept(textInput.text)
                            }
                        }

                        // 错误提示
                        Text {
                            id: errorText
                            Layout.leftMargin: 12
                            Layout.fillWidth: true
                            font.family: UIConfig.fontFamily
                            font.pixelSize: 10
                            font.weight: Font.Normal
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignTop
                            color: "#FC323E"
                            visible: text != ""
                        }


                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }

                        Item {
                            Layout.preferredHeight: 35
                            Layout.fillWidth: true
                            Layout.bottomMargin: 20

                            RowLayout {
                                anchors.fill: parent
                                spacing: 20

                                Item {
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }

                                UIButton {
                                    id: acceptButton
                                    Layout.fillHeight: true
                                    Layout.preferredWidth: 200
                                    radius: 15
                                    text: "确定"
                                    textColor: "#FFFFFF"
                                    fontWeight: Font.Bold
                                    fontSize: 14
                                    color: "#315EFB"

                                    onClicked: {
                                        root.close()
                                        if(root.accept && typeof root.accept === 'function')
                                            root.accept(textInput.text)
                                    }
                                }

                                Item {
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }
                            }

                        }

                        Item {
                            id: itemId
                            Layout.preferredHeight: root.height * 0.4
                            Layout.fillWidth: true
                            visible: false
                        }
                    }
                }
            }
        }
    }

    Component {
        id: areaContent

        Rectangle {
            width: root.width
            height: root.height * 0.75
            radius: 8
            clip: true
            color: "#FFFFFF"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (!textArea.containsMouse) {
                        textArea.focus = false
                        itemId.visible = false
                    }

                }
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 1

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40

                    Text {
                        anchors.centerIn: parent
                        width: parent.width - 50
                        text: root.panelTitle
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

                        onSignalClicked: {
                            root.close()
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: 16
                        spacing: 0

                        Rectangle {
                            id: rectangle
                            color: "#EFF1F6"
                            border.color: "#F5F5F5"
                            border.width: 1
                            Layout.fillWidth: true
                            Layout.preferredHeight: 110
                            Layout.rightMargin: 12
                            Layout.leftMargin: 12
                            radius: 2

                            RowLayout{
                                anchors.fill: parent
                                spacing: 0

                                // 输入框
                                TextArea {
                                    id: textArea
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.margins: 4
                                    clip: true
                                    wrapMode: TextEdit.Wrap
                                    placeholderText: "请输入信息"
                                    selectionColor: "#0078D7"
                                    selectedTextColor: "white"
                                    horizontalAlignment: TextInput.AlignLeft
                                    background: Rectangle {
                                        border.width: 1
                                        opacity: 0
                                    }
                                    padding:0
                                    color:"#0F172A"
                                    font.pixelSize: 12
                                    font.family: UIConfig.fontFamily
                                    font.weight: Font.Bold
                                    selectByMouse: true

                                    onTextChanged: {
                                    }

                                    //焦点变化
                                    onActiveFocusChanged: {
                                        if(activeFocus)
                                            itemId.visible = true
                                        else
                                            itemId.visible = false
                                    }

                                    Keys.priority: Keys.AfterItem

                                    // 按下回车键
                                    Keys.onEnterPressed: {
                                        root.close()
                                        if(root.accept && typeof root.accept === 'function')
                                            root.accept(textInput.text)
                                    }

                                    Keys.onReturnPressed: {
                                        root.close()
                                        if(root.accept && typeof root.accept === 'function')
                                            root.accept(textInput.text)
                                    }
                                }
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }

                        Item {
                            Layout.preferredHeight: 35
                            Layout.fillWidth: true
                            Layout.bottomMargin: 20
                            Layout.topMargin: 12

                            RowLayout {
                                anchors.fill: parent
                                spacing: 20

                                Item {
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }

                                UIButton {
                                    id: acceptButton
                                    Layout.fillHeight: true
                                    Layout.preferredWidth: 200
                                    radius: 15
                                    text: "确定"
                                    textColor: "#FFFFFF"
                                    fontWeight: Font.Bold
                                    fontSize: 14
                                    color: "#315EFB"

                                    onClicked: {
                                        root.close()
                                        if(root.accept && typeof root.accept === 'function')
                                            root.accept(textInput.text)
                                    }
                                }

                                Item {
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }
                            }

                        }

                        Item {
                            id: itemId
                            Layout.preferredHeight: root.height * 0.4
                            Layout.fillWidth: true
                            visible: false
                        }
                    }
                }
            }
        }
    }
}
