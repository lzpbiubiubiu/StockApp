import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import Component 1.0
import Other 1.0

Page {
    id: root

    /** 显示输入信息面板 */
    property bool __showInputPanel: false

    /** 当前tabView的序号 */
    property int __tabViewCurrentIndex: 0
    Connections {
        target: mainWindow
        onPageIndexChanged: {
            if(2 === mainWindow.pageIndex){
                contentLoader.sourceComponent = null
                contentLoader.sourceComponent = tabViewContent
                root.__tabViewCurrentIndex = 0
            }
        }
    }
    Rectangle {
        id: main
        anchors.fill: parent
        color: "transparent"

        Loader {
            id: contentLoader
            anchors.fill: parent
            anchors.topMargin: 20
            sourceComponent: tabViewContent
        }
    }

    Component {
        id: tabViewContent

        UITabView {
            id: tabView
            tabBarWidth: 204
            tabHeight: 32
            tabCheckboxWidth: 100
            tabCheckboxHeight: 28
            tabShowBackgroud: true
            onCurrentIndexChanged: {
                mainWindow.showNumPad = false
                root.__tabViewCurrentIndex = currentIndex
            }
            UITab {
               title: "新增"
               sourceComponent: inputContent

               Component.onCompleted: {
                   item.inputData = {}
                   item.dataLsitModel = [{"inputDataKey": "code", "required": true, "tipText":"商品编码", "placeholderText":"请输入商品编码", "showKeybord": true, "regExp": "^[A-Za-z0-9]+$"},
                                         {"inputDataKey": "name", "required": true, "tipText":"商品名称", "placeholderText":"请输入商品名称", "showKeybord": true, "regExp": ".*"},
                                         {"inputDataKey": "stock", "required": true, "tipText":"商品库存", "placeholderText":"请输入商品库存", "showKeybord": false, "regExp": "^[0-9]+$"},
                                         {"inputDataKey": "retailPrice", "required": true, "tipText":"商品零售价", "placeholderText":"请输入价格", "showKeybord": false, "regExp": "^(0|[1-9]\\d{0,7})(\\.)\\d{0,2}$"},
                                         {"inputDataKey": "wholesalePrice", "required": true, "tipText":"商品批发价", "placeholderText":"请输入价格", "showKeybord": false, "regExp": "^(0|[1-9]\\d{0,7})(\\.)\\d{0,2}$"}]
               }
            }

            UITab {
                title: "更新"
                sourceComponent: inputContent
                Component.onCompleted: {
                    item.inputData = {}
                    item.dataLsitModel = [{"inputDataKey": "code", "required": true, "tipText":"商品编码", "placeholderText":"请输入商品编码", "showKeybord": true, "regExp": "^[A-Za-z0-9]+$"},
                                          {"inputDataKey": "name", "required": true, "tipText":"商品名称", "placeholderText":"请输入商品名称", "showKeybord": true, "regExp": ".*"},
                                          {"inputDataKey": "retailPrice", "required": true, "tipText":"商品零售价", "placeholderText":"请输入价格", "showKeybord": false, "regExp": "^(0|[1-9]\\d{0,7})(\\.)\\d{0,2}$"},
                                          {"inputDataKey": "wholesalePrice", "required": true, "tipText":"商品批发价", "placeholderText":"请输入价格", "showKeybord": false, "regExp": "^(0|[1-9]\\d{0,7})(\\.)\\d{0,2}$"}]
                }
            }
        }
    }
    Component {
        id: inputContent
        Rectangle {
            color: "transparent"

            /** ListView model 数据 */
            property var dataLsitModel: null

            /** 输入数据 */
            property var inputData: null

            ColumnLayout {
                anchors.fill: parent

                // 列表
                ListView {
                    id: listView
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30 * listView.count + 12 * (listView.count - 1)
                    Layout.topMargin: 12
                    Layout.bottomMargin: 8
                    model: dataLsitModel

                    cacheBuffer: 1000
                    //消除反弹效果
                    boundsBehavior:Flickable.StopAtBounds
                    maximumFlickVelocity: 2500
                    spacing: 12
                    clip: true
                    currentIndex: 0

                    delegate: FocusScope {
                        id: area
                        width: listView.width
                        height: 30
                        activeFocusOnTab: true

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 8
                            anchors.rightMargin: 8

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                Text {
                                    height: parent.height
                                    anchors.left: parent.left
                                    width: 80
                                    font.pixelSize: 12
                                    font.family: UIConfig.fontFamily
                                    font.weight: Font.Medium
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    text: (modelData.required && "" === textInput.text ? "%1 <font color='red'>*</font>".arg(modelData.tipText) : modelData.tipText) + ":"
                                }

                                UITextInput {
                                    id: textInput
                                    focus: true
                                    color: "#EFF1F6"
                                    height: parent.height
                                    width: parent.width - 80
                                    anchors.right: parent.right
                                    font.pixelSize: 12
                                    font.family: UIConfig.fontFamily
                                    font.weight: Font.Bold
                                    placeholderText: modelData.placeholderText
                                    validator: RegExpValidator { regExp: new RegExp(modelData.regExp)}
                                    textField.readOnly: "name" === modelData.inputDataKey ? true : false
                                    activeFocusOnPress: false
                                    onSignalOnClicked: {
                                        listView.currentIndex = index
                                        textInput.forceActiveFocus()
                                    }
                                    onSignalOnFocusChanged: {
                                        if(activeFocus){
                                            if("name" !== modelData.inputDataKey){
                                                mainWindow.showNumPad = true
                                            }
                                            else{
                                                if(!root.__showInputPanel){
                                                    mainWindow.showNumPad = false
                                                    rec.forceActiveFocus()
                                                    root.showInputTextPanel(textInput)
                                                    root.__showInputPanel = true
                                                }
                                            }
                                        }
                                    }
                                    onSignalTextChanged: {
                                        if("retailPrice" === modelData.inputDataKey
                                           || "wholesalePrice" === modelData.inputDataKey){

                                            console.log("modelData.inputDataKe============>", modelData.inputDataKey)
                                            inputData[modelData.inputDataKey] = parseFloat(text) * 100
                                        }
                                        else if("stock" === modelData.inputDataKey){
                                            inputData[modelData.inputDataKey] = parseInt(text)
                                        }
                                        else{
                                            inputData[modelData.inputDataKey] = text
                                        }
                                    }

                                    onSignalOnEnterPressed: {
                                        if("" === textInput.text)
                                        {
                                            return
                                        }
                                        if(!root.__checkInputData(listView, inputData)){
                                            return
                                        }
                                        page.controller.wareStorage(inputData, root.__tabViewCurrentIndex)
                                    }
                                }

                            }

                        }
                    }
                }

                Item {
                    id: itemId
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Rectangle {
                     id: rec
                     Layout.fillWidth: true
                     Layout.preferredHeight: 30
                     color: "#FFFFFF"
                     Layout.bottomMargin: 16

                     UIButton {
                         id: submitButton
                         width: 150
                         height: 30
                         anchors.centerIn: parent
                         radius: 15
                         text: "确定"
                         textColor: "#FFFFFF"
                         fontWeight: Font.Bold
                         fontSize: 12
                         color: "#315EFB"
                         focusPolicy: Qt.NoFocus

                         onClicked: {
                             if(!root.__checkInputData(listView, inputData)){
                                 listView.forceActiveFocus()
                                 return
                             }
                             page.controller.wareStorage(inputData, root.__tabViewCurrentIndex)
                         }
                     }
                }

                Connections {
                    target: mainWindow
                    onShowNumPadChanged: {
                        if(!mainWindow.showNumPad)
                        {
                            rec.forceActiveFocus()
                        }
                    }
                }

                Connections {
                    target: mainFrame
                    onActiveFocusItemChanged: {
                        if(mainFrame.activeFocusItem !== null && mainFrame.activeFocusItem instanceof TextField){
                        }
                    }
                }
            }
        }
    }

    // 状态栏
    footer: FunctionBar {
        id: functionBar
        implicitHeight: 51

    }

    /** 显示输入信息面板 */
    function showInputTextPanel(textInput) {
        if(textInput === 'undefined' || textInput instanceof TextField){
            return
        }
        var json = {
            "panelTitle": "输入商品名称",
            "panelText": textInput.text,
            "accept": function(text) {
                textInput.text = text
                root.__showInputPanel = false
            },
            "reject":function() {
                root.__showInputPanel = false
            }
        }
        var component = Qt.createComponent("qrc:/Qml/Component/InputTextPanel.qml")
        if(component.status === Component.Ready) {
            var object = component.createObject(parent, json)
            object.anchors.fill = parent
            object.open()
        } else {
            console.error(component.errorString())
        }
    }

    /** 检测输入的数据*/
    function __checkInputData(listViewItem, inputData){
        if(listViewItem === 'undefined')
            return false

        if(listViewItem !== null && !(listViewItem instanceof ListView))
            return false

        for (var i = 0; i <  listViewItem.count; i++) {
            var itemData = inputData[listViewItem.model[i].inputDataKey]
            if(typeof itemData === 'undefined' || "" === itemData){
                listViewItem.currentIndex = i
                return false
            }
        }
        return true
    }
}
