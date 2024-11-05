import QtQuick 2.12
import "./"
import Other 1.0

// 键盘按键
Rectangle {
    id: root

    property string imageSource: ""
    property string imageHoverSource: ""
    property string imagePressSource: ""
    property string text: ""
    property color textColor: "#000000"
    property color borderColor: "#E5E5E5"
    property string fontFamily: UIConfig.fontFamily
    property color normalColor: "#FFFFFF"
    property color hoverColor: "#FFFFFF"
    property color pressColor: "#EBEBEB"
    property int positionY: -1
    property int buttonFontSize: 22
    property bool buttonBold: false

    signal signalClicked()

    function buttonStateChange() {
        rect.color = imageButton.hovered ? (imageButton.pressed ? root.pressColor : root.hoverColor) : root.normalColor
    }

    color: "transparent"
    border.width: 1
    border.color: Qt.rgba(0, 0, 0, 0.16)
    radius: 4

    Rectangle {
       id: rect
       y: root.positionY
       width: parent.width
       height: parent.height
       color: root.normalColor
       radius: parent.radius
       border.width: parent.border.width
       border.color: root.borderColor

       UIImageButton {
           id: imageButton
           focusPolicy: Qt.NoFocus
           imageSource: root.imageSource
           imageHoverSource: root.imageHoverSource
           imagePressSource: root.imagePressSource
           text: root.text
           radius: 4
           borderWidth: 1
           borderColor: root.borderColor
           textColor: root.textColor
           fontFamily: root.fontFamily
           fontWeight: root.buttonBold ? Font.Bold : Font.Medium
           fontSize: root.buttonFontSize
           width: root.width
           height: root.height

           onHoveredChanged: buttonStateChange()
           onPressedChanged: buttonStateChange()
           onSignalClicked: root.signalClicked()
       }
    }
}

