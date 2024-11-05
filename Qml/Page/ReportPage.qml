import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import Component 1.0
import Other 1.0

Page {
    id: root

    Rectangle {
        id: main
        anchors.fill: parent
        color: "transparent"

        ColumnLayout {
            anchors.fill: parent
            spacing: 2

            Rectangle {
                 Layout.fillWidth: true
                 Layout.fillHeight: true
                 color: "#EBEBEB"
                 visible: 0 === page.dataModel.length || page.querying

                 // 图标
                 AnimatedImage {
                     id: icon
                     width: 80
                     height: 80
                     source: "qrc:/Resources/Images/loading.gif"
                     smooth: true
                     anchors.centerIn: parent
                    visible: page.querying
                 }

                 Image {
                     id: errorImage
                     source: "qrc:/Resources/Images/ware_bk.svg"
                     visible: !icon.visible
                     anchors.centerIn: parent
                 }

                 //错误提示
                 Text {
                     id: errorText
                     anchors.top: errorImage.bottom
                     anchors.horizontalCenter: parent.horizontalCenter
                     font.family: UIConfig.fontFamily
                     font.pixelSize: 18
                     font.weight: Font.Medium
                     color: "#1E293B"
                     horizontalAlignment: Text.AlignHCenter
                     maximumLineCount: 5
                     wrapMode: Text.WordWrap
                     elide: Text.ElideRight
                     text: page.querying ? "查询中..." : "查询订单信息失败"
                 }
            }

            GraphPanel {
                id: reportContent
                Layout.topMargin: 16
                Layout.preferredHeight: 370
                Layout.fillWidth: true
                visible: 0 < page.dataModel.length && !page.querying
                dataPoints: page.dataModel
                datePoints: page.dateTitleModel
                totalAmount: page.totalAmount

                Connections {
                    target: page
                    onSignalDataModelChanged:{
                        reportContent.canvas.requestPaint()
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                visible: 0 < page.dataModel.length && !page.querying

                ColumnLayout {
                    anchors.fill: parent

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 16
                            spacing: 10

                            UIButton {
                                id: weekButton
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                                text: "近1周"
                                textColor: "#191919"
                                textHoverColor: "#0F62FE"
                                textPressColor: "#0F62FE"
                                color: "#FFFFFF"
                                hoverColor: "#EBF4FF"
                                pressColor: "#EBF4FF"
                                radius: 15
                                fontSize: 12
                                fontWeight: Font.Medium
                                onClicked: {
                                    selected = true
                                    monthButton.selected = false
                                    page.controller.byweekReport()
                                    reportContent.canvas.requestPaint()
                                }
                            }

                            UIButton {
                                id: monthButton
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                                text: "近1月"
                                selected: true
                                textColor: "#191919"
                                textHoverColor: "#0F62FE"
                                textPressColor: "#0F62FE"
                                color: "#FFFFFF"
                                hoverColor: "#EBF4FF"
                                pressColor: "#EBF4FF"
                                radius: 15
                                fontSize: 12
                                fontWeight: Font.Medium
                                onClicked: {
                                    selected = true
                                    weekButton.selected = false
                                    page.controller.monthlyReport()
                                    reportContent.canvas.requestPaint()
                                }
                            }

                            Item {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                            }
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
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
}
