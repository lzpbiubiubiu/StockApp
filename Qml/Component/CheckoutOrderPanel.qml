import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Component 1.0
import Other 1.0

/**
 * 订单确认面板
 *
 * 用户参数:
 * userData.orderAmount 订单总金额
 * userData.promotionAmount 订单促销金额
 * 返回输入参数:
 * __inputData.wholeOrderPromoAmt 整单优惠金额
 * __inputData.deliveryFeesAmt 自主配送费
 * __inputData.remarkText 订单备注信息
 */
UIDrawer {
    id: root

    // 功能类型
    enum FuncType {

        /** 主功能 */
        TYPE_MAIN = 0,

        /** 整单优惠输入 */
        TYPE_WHOLE_ORDER_PROMO,

        /** 商家自主配送费 */
        TYPE_DELIVERY_FEES,

        /** 订单备注 */
        TYPE_ORDER_REMARK
    }

    /** 用户参数 */
    property var userData: null

    /** 确认回调 */
    property var accept: null

    /** 输入数据 */
    property var __inputData: null

    /** 当前功能类型 */
    property int __funcType: CheckoutOrderPanel.FuncType.TYPE_MAIN

    /** 当前输入框的内容 */
    property string __inputText: ""

    edge: bottomEdge
    closeOnClickOutside: CheckoutOrderPanel.FuncType.TYPE_MAIN === root.__funcType

    Component.onCompleted: {
        root.__inputData = {}
        root.wholeOrderPromoAmt = 0
        root.deliveryFeesAmt = 0
        root.remarkText = ""
    }

    //内容区域
    contentItem: {
        if(CheckoutOrderPanel.FuncType.TYPE_WHOLE_ORDER_PROMO === root.__funcType
           || CheckoutOrderPanel.FuncType.TYPE_DELIVERY_FEES === root.__funcType)
        {
            amountInputContent
        }
        else if(CheckoutOrderPanel.FuncType.TYPE_ORDER_REMARK === root.__funcType){
            remarkInputContent
        }
        else{
            content
        }
    }

    Component {
        id: content

        Rectangle {
            width: root.width
            height: root.height * 0.75
            radius: 8
            clip: true
            color: "#FFFFFF"

            ColumnLayout {
                anchors.fill: parent
                spacing: 1

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40

                    Text {
                        anchors.centerIn: parent
                        width: parent.width - 50
                        text: "确认订单"
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

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: 16
                        spacing: 1

                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 20
                            Layout.leftMargin: 8
                            Layout.rightMargin: 8

                            Text {
                                height: parent.height
                                anchors.left: parent.left
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Medium
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignBottom
                                textFormat: Text.RichText
                                text: "整单优惠"
                            }

                            Text {
                                height: parent.height
                                anchors.right: icon1.left
                                anchors.rightMargin: 4
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Medium
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignBottom
                                textFormat: Text.RichText
                                text: {
                                    var itemData = root.__inputData.wholeOrderPromoAmt
                                    if(typeof itemData === 'undefined' || 0 === itemData) {
                                        "￥0.00"
                                    }
                                    else {
                                        "减 ￥" + (itemData / 100).toFixed(2)
                                    }
                                }
                            }

                            Text {
                                id: icon1
                                height: parent.height
                                anchors.right: parent.right
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Medium
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignBottom
                                textFormat: Text.RichText
                                color: "#DCDCDC"
                                text: ">"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    root.__funcType = CheckoutOrderPanel.FuncType.TYPE_WHOLE_ORDER_PROMO
                                    var itemData = root.__inputData.wholeOrderPromoAmt
                                    if(typeof itemData !== 'undefined' && 0 !== itemData) {
                                       root.__inputText = (itemData / 100).toFixed(2)
                                    }
                                    else{
                                        root.__inputText = ""
                                    }
                                }
                            }
                        }


                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 20
                            Layout.leftMargin: 8
                            Layout.rightMargin: 8

                            Text {
                                height: parent.height
                                anchors.left: parent.left
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Medium
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignBottom
                                textFormat: Text.RichText
                                text: "自主配送费"
                            }

                            Text {
                                height: parent.height
                                anchors.right: icon2.left
                                anchors.rightMargin: 4
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Medium
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignBottom
                                textFormat: Text.RichText
                                text: {
                                    var itemData = root.__inputData.deliveryFeesAmt
                                    if(typeof itemData === 'undefined' || 0 === itemData) {
                                        "￥0.00"
                                    }
                                    else {
                                        "￥" + (itemData / 100).toFixed(2)
                                    }
                                }
                            }

                            Text {
                                id: icon2
                                height: parent.height
                                anchors.right: parent.right
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Medium
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignBottom
                                textFormat: Text.RichText
                                color: "#DCDCDC"
                                text: ">"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    root.__funcType = CheckoutOrderPanel.FuncType.TYPE_DELIVERY_FEES
                                    var itemData = root.__inputData.deliveryFeesAmt
                                    if(typeof itemData !== 'undefined' && 0 !== itemData) {
                                       root.__inputText = (itemData / 100).toFixed(2)
                                    }
                                    else{
                                        root.__inputText = ""
                                    }
                                }
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 20
                            Layout.leftMargin: 8
                            Layout.rightMargin: 8

                            Text {
                                height: parent.height
                                anchors.left: parent.left
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Medium
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignBottom
                                textFormat: Text.RichText
                                text: "订单备注"
                            }

                            Text {
                                height: parent.height
                                anchors.right: icon3.left
                                anchors.rightMargin: 4
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Medium
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignBottom
                                textFormat: Text.RichText
                                color: {
                                    var itemData = root.__inputData.remarkText
                                    if(typeof itemData === 'undefined' || "" === itemData) {
                                        "#DCDCDC"
                                    }
                                    else {
                                        "#000000"
                                    }
                                }

                                text: {
                                    var itemData = root.__inputData.remarkText
                                    if(typeof itemData === 'undefined' || "" === itemData) {
                                        "无备注"
                                    }
                                    else {
                                        itemData
                                    }
                                }
                            }

                            Text {
                                id: icon3
                                height: parent.height
                                anchors.right: parent.right
                                font.pixelSize: 12
                                font.family: UIConfig.fontFamily
                                font.weight: Font.Medium
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignBottom
                                textFormat: Text.RichText
                                color: "#DCDCDC"
                                text: ">"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    root.__funcType = CheckoutOrderPanel.FuncType.TYPE_ORDER_REMARK
                                    var itemData = root.__inputData.remarkText
                                    if(typeof itemData !== 'undefined') {
                                       root.__inputText = itemData
                                    }
                                    else{
                                        root.__inputText = ""
                                    }
                                }
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }
                    }
                }

                Rectangle {
                     Layout.fillWidth: true
                     Layout.preferredHeight: 50
                     color: "#FFFFFF"

                     Item {
                        height: parent.height
                        width: totalPrice.implicitWidth
                        anchors.right: submitButton.left
                        anchors.rightMargin: 8

                        Text {
                            id: totalPrice
                            height: parent.height / 2
                            font.pixelSize: 10
                            font.family: UIConfig.fontFamily
                            font.weight: Font.Medium
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignBottom
                            textFormat: Text.RichText
                            text: "合计: <span style='font-size:10px; color:red; font-weight: normal'>%1</span><span style='font-size:12px; color:red; font-weight: bold'>%2</span>".arg("￥").arg((userData.orderAmount / 100).toFixed(2))
                        }

                        Text {
                            id: promotionPrice
                            anchors.top: totalPrice.bottom
                            height: parent.height / 2
                            font.pixelSize: 8
                            font.family: UIConfig.fontFamily
                            font.weight: Font.Medium
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignTop
                            textFormat: Text.RichText
                            color: "#808080"
                            text: "优惠减:￥" + (userData.promotionAmount / 100).toFixed(2)
                        }

                     }

                     UIButton {
                         id: submitButton
                         width: 80
                         height: 30
                         anchors.verticalCenter:parent.verticalCenter
                         anchors.right: parent.right
                         anchors.rightMargin: 8
                         radius: 15
                         text: "提交订单"
                         textColor: "#FFFFFF"
                         fontWeight: Font.Bold
                         fontSize: 12
                         color: "#315EFB"

                         onClicked: {
                             root.close()
                             if(root.accept && typeof root.accept === 'function')
                                 root.accept(root.__inputData)
                         }
                     }
                }
            }
        }
    }

    Component {
        id: amountInputContent

        Rectangle {
            id: amountInputRec
            width: root.width
            height: root.height * 0.75
            radius: 8
            clip: true
            color: "#FFFFFF"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (!amountInput.containsMouse) {
                        amountInput.focus = false
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

                    UIImageButton {
                        id: backButton

                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        imageSource: "qrc:/Resources/Images/back_arraow.svg"
                        imageHoverSource: "qrc:/Resources/Images/back_arraow.svg"
                        imagePressSource: "qrc:/Resources/Images/back_arraow.svg"

                        onSignalClicked: {
                            root.__funcType = CheckoutOrderPanel.FuncType.TYPE_MAIN
                            root.__inputText = ""
                        }
                    }

                    Text {
                        anchors.centerIn: parent
                        width: parent.width - 50
                        text: CheckoutOrderPanel.FuncType.TYPE_DELIVERY_FEES === root.__funcType ? "自主配送费" : "整单优惠金额"
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

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: 16
                        spacing: 0

                        // 输入金额
                        UITextInput {
                            id: amountInput
                            Layout.fillWidth: true
                            Layout.preferredHeight: 30
                            Layout.rightMargin: 12
                            Layout.leftMargin: 12
                            color: "#EFF1F6"
                            font.pixelSize: 12
                            font.family: UIConfig.fontFamily
                            font.weight: Font.Bold
                            text: CheckoutOrderPanel.FuncType.TYPE_DELIVERY_FEES === root.__funcType || CheckoutOrderPanel.FuncType.TYPE_WHOLE_ORDER_PROMO === root.__funcType ? root.__inputText : ""
                            placeholderText: CheckoutOrderPanel.FuncType.TYPE_DELIVERY_FEES === root.__funcType ? "请输入自主配送费用" : "请输入需要优惠的金额"
                            validator: RegExpValidator{regExp: /(0|[1-9]\d{0,7})(\.)\d{0,2}$/}

                            //焦点变化
                            onActiveFocusChanged: {
                                if(activeFocus)
                                    itemId.visible = true
                                else
                                    itemId.visible = false
                            }

                            onSignalOnEnterPressed: {
                                amountInputRec.__inputAmount()
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
                                        amountInputRec.__inputAmount()
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


            /** 输入金额函数 */
            function __inputAmount()
            {
                var input = parseFloat(amountInput.text)
                if("" === amountInput.text || isNaN(input))
                {
                    errorText.text = "请输入有效金额"
                    return
                }
                if(CheckoutOrderPanel.FuncType.TYPE_WHOLE_ORDER_PROMO === root.__funcType)
                {
                    var amount = input * 100
                    if(amount > root.userData.orderAmount)
                    {
                        errorText.text = "请输入合理的优惠金额"
                        return
                    }
                    root.__inputData["wholeOrderPromoAmt"] = input * 100
                    root.userData.orderAmount -= input * 100
                    root.userData.promotionAmount += input * 100
                }
                else if(CheckoutOrderPanel.FuncType.TYPE_DELIVERY_FEES === root.__funcType)
                {
                    root.__inputData["deliveryFeesAmt"] = input * 100
                }

                root.__funcType = CheckoutOrderPanel.FuncType.TYPE_MAIN
            }
        }
    }

    Component {
        id: remarkInputContent

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

                    UIImageButton {
                        id: backButton

                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        imageSource: "qrc:/Resources/Images/back_arraow.svg"
                        imageHoverSource: "qrc:/Resources/Images/back_arraow.svg"
                        imagePressSource: "qrc:/Resources/Images/back_arraow.svg"

                        onSignalClicked: {
                            root.__funcType = CheckoutOrderPanel.FuncType.TYPE_MAIN
                            root.__inputText = ""
                        }
                    }

                    Text {
                        anchors.centerIn: parent
                        width: parent.width - 50
                        text: "订单备注"
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
                                    text: CheckoutOrderPanel.FuncType.TYPE_ORDER_REMARK === root.__funcType ? root.__inputText : ""
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
                                    }

                                    Keys.onReturnPressed: {
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
                                        root.__inputData["remarkText"] = textArea.text
                                        root.__funcType = CheckoutOrderPanel.FuncType.TYPE_MAIN
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
