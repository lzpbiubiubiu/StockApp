import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import "./"
import Other 1.0

// 数字小键盘控件
Rectangle {

    property int padMargin: 8
    property int padSpacing: 8
    property int keyWidth: (width - padSpacing * 2 - padMargin * 2) / 3
    property int keyHeight: (height - padSpacing * 4 - padMargin * 2) / 5

    /** 显示回车 */
    property bool showEnter: true

    /** 显示取消 */
    property bool showCancel: true

    /** 显示小数点 */
    property bool showDecimal: true

    /** 回车按钮文案 */
    property string enterKeyText: "确定"

    /** 取消按钮文案 */
    property string cancelBtnText: "取消"

    /** 取消按钮点击 */
    signal signalCancelButtonClicked()

    id: root
    color: "transparent"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: root.padMargin
        spacing: root.padSpacing

        Rectangle {
            color: "transparent"
            Layout.preferredWidth: keyWidth * 3 + 16
            Layout.preferredHeight: keyHeight * 4 + 32

            GridLayout {
                anchors.fill: parent
                rows: 4
                columns: 3
                rowSpacing: root.padSpacing
                columnSpacing: root.padSpacing


                Repeater {
                    model:[

                        {name: "1", portrait: "", keys: [Qt.Key_1], modifier: Qt.NoModifier, buttonFontSize: 16},
                        {name: "2", portrait: "", keys: [Qt.Key_2], modifier: Qt.NoModifier, buttonFontSize: 16},
                        {name: "3", portrait: "", keys: [Qt.Key_3], modifier: Qt.NoModifier, buttonFontSize: 16},

                        {name: "4", portrait: "", keys: [Qt.Key_4], modifier: Qt.NoModifier, buttonFontSize: 16},
                        {name: "5", portrait: "", keys: [Qt.Key_5], modifier: Qt.NoModifier, buttonFontSize: 16},
                        {name: "6", portrait: "", keys: [Qt.Key_6], modifier: Qt.NoModifier, buttonFontSize: 16},

                        {name: "7", portrait: "", keys: [Qt.Key_7], modifier: Qt.NoModifier, buttonFontSize: 16},
                        {name: "8", portrait: "", keys: [Qt.Key_8], modifier: Qt.NoModifier, buttonFontSize: 16},
                        {name: "9", portrait: "", keys: [Qt.Key_9], modifier: Qt.NoModifier, buttonFontSize: 16},

                        {name: ".", portrait: "", keys: [Qt.Key_Period], modifier: Qt.NoModifier, buttonFontSize: 16},
                        {name: "0", portrait: "", keys: [Qt.Key_0], modifier: Qt.NoModifier, buttonFontSize: 16},
                        {name: "",  portrait: "qrc:/Resources/Images/kb_backspace.svg", keys: [Qt.Key_Backspace], modifier: Qt.NoModifier, buttonFontSize: 16},
                    ]

                    UIKeyButton {
                        Layout.preferredWidth: keyWidth
                        Layout.preferredHeight: keyHeight
                        imageSource: modelData.portrait
                        imageHoverSource: modelData.portrait
                        imagePressSource: modelData.portrait
                        pressColor: "#EBEBED"
                        text: modelData.name
                        buttonFontSize: modelData.buttonFontSize
                        buttonBold: true
                        onSignalClicked: {
                            for(var i=0;i<modelData.keys.length;i++){
                                emitter.emitKey(modelData.keys[i], modelData.modifier)
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            color: "transparent"
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: showCancel || showEnter

            RowLayout {
                anchors.fill: parent
                spacing: root.padSpacing

                UIKeyButton {
                    Layout.preferredWidth: parent.width / 2
                    Layout.preferredHeight: keyHeight
                    borderColor: "#0F62FE"
                    textColor: "#FFFFFF"
                    normalColor: "#0F62FE"
                    hoverColor: "#0F62FE"
                    pressColor: "#0D52D5"
                    text: cancelBtnText
                    buttonBold: true
                    buttonFontSize: 16
                    visible: showCancel
                    onSignalClicked: {
                        root.signalCancelButtonClicked()
                        //emitter.emitKey(Qt.Key_Escape, Qt.NoModifier)
                    }
                }

                UIKeyButton {
                    Layout.fillWidth: true
                    Layout.preferredHeight: keyHeight
                    borderColor: "#0F62FE"
                    textColor: "#FFFFFF"
                    normalColor: "#0F62FE"
                    hoverColor: "#0F62FE"
                    pressColor: "#0D52D5"
                    buttonBold: true
                    buttonFontSize: 16
                    visible: showEnter
                    text: enterKeyText

                    onSignalClicked: emitter.emitKey(Qt.Key_Enter, Qt.NoModifier)
                }
            }
        }
    }
}
