import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import "./"

// TAB视图控件
Item {

    id: root

    /** 是否显示Tab */
    property bool tabVisible: true
    
    /** Tab高度 */
    property int tabHeight: 48

    property int tabBarWidth: root.width
    property alias tabCheckboxWidth: tabbar.checkboxWidth
    property alias tabCheckboxHeight: tabbar.checkboxHeight
    property alias tabShowBackgroud: tabbar.showBackgroud

    /** Tab内间距 */
    property alias tabLeftPadding: tabbar.leftPadding
    property alias tabTopPadding: tabbar.topPadding
    property alias tabRightPadding: tabbar.rightPadding
    property alias tabBottomPadding: tabbar.bottomPadding

    /** 当前激活Tab */
    property alias activeTab: tabbar.currentIndex

    /** Tab页元素 */
    default property alias items: stack.children

    /** 当前页索引 */
    readonly property alias currentIndex: stack.currentIndex

    property alias tabbar: tabbar

    ColumnLayout {
        id: column
        anchors.fill: parent
        spacing: 0

        UITabBar {
            id: tabbar
            visible: tabVisible
            Layout.preferredWidth: root.tabBarWidth
            Layout.preferredHeight: root.tabHeight
            Layout.alignment: Qt.AlignCenter
            currentIndex: activeTab
        }

        StackLayout {
            id: stack
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabbar.currentIndex
        }
    }

    Component {
        id: tabcomp
        UIButton {
            text: title
            fontWeight: Font.Bold
            fontSize: 12
        }
    }

    Component.onCompleted: {
        for(var i = 0; i < items.length; ++i) {
            if(items[i].active) {
                tabbar.addTab(i, items[i].title)
            }
        }
    }

    function changeTabState(index, check) {
       tabbar.changeTabState(index, check)
    }
}
