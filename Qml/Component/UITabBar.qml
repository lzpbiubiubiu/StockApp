import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import Other 1.0
import "./"

// TAB容器
Rectangle {

    property ListModel __tabs: ListModel { }
    readonly property int count: __tabs.count
    property int currentIndex: 0
    property alias leftPadding: container.leftPadding
    property alias topPadding: container.topPadding
    property alias rightPadding: container.rightPadding
    property alias bottomPadding: container.bottomPadding
    property alias container: container
    property bool showBackgroud: false
    property int checkboxWidth: 0
    property int checkboxHeight: 0
    property int tabWidth: 0

    // 切换tab前callback
    property var beforeSwitchTabCallback

    id: root
    color: "#FFFFFF"

    Row {
        id: container
        anchors.fill: parent
        spacing: 0
    }

    ButtonGroup {
        id: tabGroup;
    }

    Component {
        id: tabcomp

        Rectangle {
            property alias title: checkbox.text
            property alias state: checkbox.checked
            property int index: 0

            width: (tabWidth === 0) ? (container.width - root.leftPadding - root.rightPadding) / __tabs.count : tabWidth
            height: parent.height
            color: showBackgroud ? "#EDEFF3" : "#FFFFFF"

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                CheckBox {
                    id: checkbox
                    Layout.preferredWidth: (checkboxWidth === 0) ? parent.width : checkboxWidth
                    Layout.preferredHeight:(checkboxHeight === 0) ? parent.height - indicator.height : checkboxHeight
                    Layout.alignment: Qt.AlignCenter

                    focusPolicy: Qt.NoFocus
                    ButtonGroup.group: tabGroup
                    checked: index === root.currentIndex

                    indicator: Item {}

                    background: Rectangle {
                        color: checkbox.checked ? "#FFFFFF" : "#EDEFF3"
                        anchors.fill: parent
                    }

                    contentItem: Text {
                        text: checkbox.text
                        elide: Text.ElideRight
                        font.pixelSize: 12
                        font.family: UIConfig.fontFamily
                        font.weight: Font.Bold
                        color: checkbox.checked ? "#315EFB" : "#0F172A"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onCheckedChanged: {
                        console.log("tabWidth", tabWidth, index, checked)
                        if(checked && root.currentIndex != index)
                            root.currentIndex = index
                    }

                    MouseArea {
                        anchors.fill: parent

                        onClicked: if(root.beforeSwitchTabCallback && typeof(root.beforeSwitchTabCallback) === 'function') {
                                       root.beforeSwitchTabCallback(index, title)
                                   } else {
                                        console.log("tabindex", root.currentIndex, index, checkbox.checked)
                                       if(root.currentIndex != index || !checkbox.checked) {
                                            checkbox.checked = !checkbox.checked
                                       }
                                   }
                    }
                }

                Rectangle {
                    id: indicator
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: 0
                    color: checkbox.checked ? "#0F62FE" : "#FFFFFF"
                }
            }
        }
    }

    onCurrentIndexChanged: {
        console.log("==============> currentIndex:", root.currentIndex)
    }

    function addTab(index, title) {
        var tab = tabcomp.createObject(container, {"title": title, "index": index})
        __tabs.append({"tab": tab})
    }
    function changeTabTitle(oriTitle, title) {
        for(var i = 0; i < __tabs.count; ++i) {
            if(__tabs.get(i).tab.title === oriTitle) {
                __tabs.get(i).tab.title = title
            }
        }
    }

    function changeTabState(index, check) {
        console.log("changeTabState tabindex", index, check)
        if(index >= 0 && index < __tabs.count) {
			__tabs.get(index).tab.state= check
		}
    }
}
