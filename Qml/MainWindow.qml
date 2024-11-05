import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1
import Component 1.0
import Other 1.0

ApplicationWindow {

    /** 加载动画 */
    property Item __loading: null

    /** toast队列 */
    property variant __toasts: []

    /** 当前toast */
    property variant __currentToast: null

    /** 缓存toast组件 */
    property var __toastComponent : Qt.createComponent("qrc:/Qml/Component/UIToast.qml")

    id: root
    width: mainWindow.fullScreen ? Screen.width : 300
    height: mainWindow.fullScreen ? Screen.height + 1 : 600
    title: qsTr("Stock App")
    flags: mainWindow.fullScreen ? (Qt.FramelessWindowHint | Qt.WindowSystemMenuHint | Qt.WindowMinimizeButtonHint | Qt.Window) : Qt.Window
    onActiveFocusItemChanged: {
        if(root.activeFocusItem !== null && root.activeFocusItem instanceof TextField){
            console.log("Window ===================>", mainFrame.activeFocusItem, mainFrame.activeFocusItem instanceof TextField)
        }
        else{
            mainWindow.showNumPad = false
        }
        console.log("Window ===================>", mainFrame.activeFocusItem)
    }

    Rectangle {
       anchors.fill: parent
       color: "white"
       radius: 12

       ColumnLayout {
           anchors.fill: parent
           spacing: 0

           StackView {
               id: stackView

               Layout.fillWidth: true
               Layout.fillHeight: true
               focus: true
               popEnter: Transition {}
               popExit: Transition {}
               pushEnter: Transition {}
               pushExit: Transition {}
               replaceEnter: Transition {}
               replaceExit: Transition {}
               onCurrentItemChanged: {
                   if(currentItem != null)
                       currentItem.focus = true
               }
           }

           Loader
           {
               id: flipper
               Layout.fillWidth: parent
           }
       }
    }

    // 数字键盘
    UINumPad {
        color: "white"
        width: parent.width
        height: parent.height * 0.4
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        visible: mainWindow.showNumPad
        onSignalCancelButtonClicked: {
            mainWindow.showNumPad = false
        }

    }

    /** 加载页面 */
    function loadPage(page) {
        stackView.replace(page.item)
//        if(stackView.currentItem.footer !== null)
//            flipper.sourceComponent = stackView.currentItem.footer
    }

    /** 显示loading弹窗 */
    function showLoading(text, delayShow) {
        if(__loading !== null) {
            __loading.text = text
        } else {
            var component = Qt.createComponent("qrc:/Qml/Component/UILoading.qml")
            if (component.status === Component.Ready) {
                __loading = component.createObject(root, { "text": text, "delayShow": delayShow })
                console.log("UILoading Window.activeFocusItem", Window.activeFocusItem)
                __loading.closeFocusItem = Window.activeFocusItem
                __loading.forceActiveFocus()
            } else {
                console.error(component.errorString())
            }
        }
        mainWindow.loadingRefCount++
        console.log("==========> show() __loadingRefCount ", mainWindow.loadingRefCount)
    }

    /** 关闭loading弹窗 */
    function closeLoading() {
        if(--mainWindow.loadingRefCount < 0)
            mainWindow.loadingRefCount = 0

        if(__loading !== null && mainWindow.loadingRefCount === 0) {
            console.log("UILoading closeFocusItem", __loading.closeFocusItem)
            if(__loading.closeFocusItem)
                __loading.closeFocusItem.forceActiveFocus()
            __loading.visible = false
            __loading.destroy()
            __loading = null
        }
        console.log("==========> close() __loadingRefCount ", mainWindow.loadingRefCount)
    }

    /** 显示提示框 */
    function showToast(text, duration) {
        // 如果前一个toast仍在显示且内容不一致则复用
        console.log("showToast:", text)
        if(__currentToast !== null) {
            if(__currentToast.text !== text)
            {
                __currentToast.text = text
                __currentToast.duration = duration
                __currentToast.reset()
            }
            return
        }

        if (__toastComponent.status === Component.Ready) {
            __currentToast = __toastComponent.createObject(root, { "text": text, "duration": duration })
            __currentToast.Component.onDestruction.connect(function() {
                __currentToast = null
            })
            __currentToast.show()
        }
    }
}
