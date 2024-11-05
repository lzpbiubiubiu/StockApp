import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2
import Other 1.0

// 通用对话框控件
Dialog {
    id: root

    /** 是否响应Esc关闭控件 */
    property bool closeOnEscPressed: false

    /** 关闭时是否摧毁控件 */
    property bool destoryOnClose: true

    /** 是否显示蒙层 */
    property bool showMask: true

    // 背景色
    property color color: "white"

    // 圆角
    property int radius: 0

    // 对话框关闭时，恢复的焦点
    property Item restoreFocusItem: null
    
    /** 关闭窗口计时*/
    property int closeInterval: 5

    /** 窗口关闭信号 */
    signal signalClosed()

    /** 窗口打开信号 */
    signal signalOpened()

    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay
    closePolicy: Popup.NoAutoClose

    // 背景
    background: Rectangle {
        color: root.color
        radius: root.radius
        border.width: 0
    }

    // 遮罩
    Overlay.modal: Rectangle {
        anchors.fill: parent
        color: root.showMask ? Qt.rgba(0, 0, 0, 0.5) : Qt.rgba(0, 0, 0, 0)
    }

    // 进入动画
    enter: Transition {}

    // 退出动画
    exit: Transition {}

    // 标题
    header: Item{}

    // 页脚
    footer: Item {}

    // 屏蔽键盘事件
    Keys.onPressed: {
        event.accepted = true
    }

    // esc退出
    Keys.onEscapePressed:
    {
        if(closeOnEscPressed)
            root.close()
    }

    // 对话框打开，弹出层计数 +1
    onOpened: {
        mainWindow.addPageMaskRefCount()
        console.log("UIDialog Window.activeFocusItem", Window.activeFocusItem)
        root.restoreFocusItem = Window.activeFocusItem
        root.signalOpened()
    }

    // 对话框关闭，弹出层计数 -1
    onClosed: {
        console.log("UIDialog restoreFocusItem", restoreFocusItem)
        if(root.restoreFocusItem)
            root.restoreFocusItem.forceActiveFocus()
        mainWindow.releasePageMaskRefCount()
        root.signalClosed()
        if(destoryOnClose) {
            root.destroy()
        }
    }

    Component.onDestruction: {
        console.log("UIDialog.onDestruction()")
    }
}
