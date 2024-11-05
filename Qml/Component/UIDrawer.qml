import QtQuick 2.12
import QtQuick.Window 2.12

import "./"

// 抽屉控件
Rectangle {
    id: root

    /** 面板打开时出现位置 */
    readonly property int leftEdge: 0
    readonly property int rightEdge: 1
    readonly property int topEdge: 2
    readonly property int bottomEdge: 3

    /** 内容控件 */
    property Component contentItem

    /** 抽屉关闭时是否摧毁控件 */
    property bool destoryOnClose: true

    /** 抽屉位置 0完全关闭 1完全打开 0-1 抽屉动画中 */
    property real position: 0

    /** 抽屉展示位置 */
    property int edge: bottomEdge

    /** 点击蒙层关闭抽屉 */
    property bool closeOnClickOutside: true

    /** 用户自定义数据 */
    property var userData

    property Item closeFocusItem: null

    /** maskCount是否计数 */
    property bool isCount: true

    /** 抽屉打开信号（动画前） */
    signal signalOpen()

    /** 抽屉完全打开信号（动画后） */
    signal signalOpened()

    /** 抽屉关闭信号（动画前） */
    signal signalClose()

    /** 抽屉完全关闭信号(动画后) */
    signal signalClosed()

    /** 点击蒙层信号 */
    signal signalClickOutside()

    /** 页面关闭回调 */
    property var closeFunc: null

    /** 打开抽屉 */
    function open() {
        if(position < 1 && animation.to === 0) {
            loader.enabled = true
            if(root.isCount){
                mainWindow.addPageMaskRefCount()
            }
            animation.stop()
            console.log("UIDrawer Window.activeFocusItem ", Window.activeFocusItem, closeFocusItem)
            if(closeFocusItem === null){
                closeFocusItem = Window.activeFocusItem
            }
            root.forceActiveFocus()
            root.visible = true
            mouseArea.visible = true
            animation.from = 0.01
            animation.to = 1
            animation.start()
            root.signalOpen()
        }
    }

    /** 关闭抽屉 */
    function close() {
        if(position > 0 && animation.to === 1) {
            loader.enabled = false
            if(root.isCount) {
                mainWindow.releasePageMaskRefCount()
            }
            animation.stop()
            if(closeFocusItem){
                console.log("UIDrawer closeFocusItem", closeFocusItem)
                closeFocusItem.forceActiveFocus()
            }
            animation.from = 0.99
            animation.to = 0
            animation.start()
            root.signalClose()
        }
    }

    function openPage() {
        if(position < 1 && animation.to === 0) {
            loader.enabled = true
            animation.stop()
            console.log("UIDrawer Window.activeFocusItem", Window.activeFocusItem)
            closeFocusItem = Window.activeFocusItem
            root.forceActiveFocus()
            root.visible = true
            mouseArea.visible = true
            animation.from = 0.01
            animation.to = 1
            animation.start()
            root.signalOpen()
        }
    }

    /** 关闭抽屉 */
    function closePage() {
        if(position > 0 && animation.to === 1) {
            loader.enabled = false
            animation.stop()
            if(closeFocusItem){
                console.log("UIDrawer closeFocusItem", closeFocusItem)
                closeFocusItem.forceActiveFocus()
            }
            animation.from = 0.99
            animation.to = 0
            animation.start()
            root.signalClose()
        }
    }

    color: Qt.rgba(0,0,0,0)
    visible: false

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        visible: false

        onClicked: {
            root.signalClickOutside()
            if(closeOnClickOutside && !contentMouseArea.containsMouse) {
                root.close()
            }
        }

        onPressed: {}
        onReleased: {}
        onWheel: {}
        hoverEnabled: true
    }

    Loader {
        id: loader
        sourceComponent: contentItem
        visible: position !== 0

        MouseArea {
            id: contentMouseArea
            anchors.fill: loader.item
        }

        onItemChanged: {
            if(edge === leftEdge) {
                loader.item.x = (position - 1) * loader.item.width
            } else if(edge === rightEdge) {
                loader.item.x = root.width - (position * loader.item.width)
            } else if(edge === topEdge) {
                loader.item.y = (position - 1) * loader.item.height
            } else if(edge === bottomEdge) {
                loader.item.y = root.height - (position * loader.item.height)
            }
            color = Qt.rgba(0, 0, 0, position * 0.3)
        }
    }

    onPositionChanged: {
        if(edge === leftEdge) {
            loader.item.x = (position - 1) * loader.item.width
        } else if(edge === rightEdge) {
            loader.item.x = root.width - (position * loader.item.width)
        } else if(edge === topEdge) {
            loader.item.y = (position - 1) * loader.item.height
        } else if(edge === bottomEdge) {
            loader.item.y = root.height - (position * loader.item.height)
        }
        color = Qt.rgba(0, 0, 0, position * 0.3)
    }

    onSignalClosed: {
        if(root.closeFunc && typeof root.closeFunc === 'function')
            root.closeFunc()
    }

    NumberAnimation {
        id: animation
        target: root
        property: "position"
        easing.type: Easing.InOutQuad
        duration: 300

        onStopped: {
            if(position === 0) {
                mouseArea.visible = false
                root.visible = false
                root.signalClosed()
                if(destoryOnClose) {
                    console.log("===============> drawer destroyed")
                    root.destroy()
                }
            } else if(position === 1) {
                root.signalOpened()
            }
        }
    }
}
