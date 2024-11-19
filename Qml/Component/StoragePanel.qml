import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import Component 1.0
import Other 1.0

/**
 * 商品入库面板
 */

UIDrawer {
    id: root

    /** 显示输入信息面板 */
    property bool __showInputPanel: false

    /** 当前tabView的序号 */
    property int __tabViewCurrentIndex: 0

    width: parent.width
    height: parent.height
    color: "transparent"

    edge: bottomEdge
    closeOnClickOutside: true

    //内容区域
    contentItem: tabViewContent

    Component {
        id: tabViewContent

        Rectangle {
            width: root.width
            height: root.height * 0.95
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
                        text: "商品入库"
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

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    UITabView {
                        anchors.fill: parent
                        id: tabView
                        tabBarWidth: 204
                        tabHeight: 32
                        tabCheckboxWidth: 100
                        tabCheckboxHeight: 28
                        tabShowBackgroud: true
                        onCurrentIndexChanged: {
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
                                    activeFocusOnPress: "name" === modelData.inputDataKey ? true : false
                                    onSignalOnClicked: {
                                        listView.currentIndex = index
                                        textInput.forceActiveFocus()
                                    }
                                    onSignalOnFocusChanged: {
                                        if("name" === modelData.inputDataKey){
                                            if(activeFocus){
                                                numPad.visible = false
                                                rec.visible = true
                                                itemId.visible = true
                                            }
                                            else{
                                                itemId.visible = false
                                            }

                                        }
                                        else{
                                            if(activeFocus){
                                                numPad.visible = true
                                                rec.visible = false
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

                // 数字键盘
                UINumPad {
                    radius: 8
                    id: numPad
                    color: "white"
                    visible: false
                    Layout.preferredHeight: root.height * 0.35
                    Layout.fillWidth: true
                    Layout.bottomMargin: 8
                    enterKeyText: "确定"
                    cancelBtnText: "清除"
                    onSignalCancelButtonClicked: {
                        numPad.visible = false
                        rec.visible = true
                        rec.forceActiveFocus()
                    }
                }

                Item {
                    id: itemId
                    visible: false
                    Layout.preferredHeight: root.height * 0.4
                    Layout.fillWidth: true
                }
            }
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
