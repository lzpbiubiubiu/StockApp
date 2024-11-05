import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Component 1.0
import Other 1.0
/**
 * 销售报表曲线面板
 *
 * 用户参数:
 * dataPoints：[{date:"", amount: 0}]
 * datePoints：["2024-08-12"]
 */
Rectangle {
    id: root

    /** 图表画布 */
    property alias canvas: canvas

    /** 数据点model */
    property var dataPoints: []

    /** 日期label 数据点model */
    property var datePoints: []

    /** 当前报表总金额 */
    property var totalAmount: 0

    /** 当前选中的序号 */
    property int __currentIndex: 0

    /** 是否显示明细 */
    property bool __showDetail: false

    Rectangle {
        width: parent.width
        height: 40
        color: root.__showDetail ? "#87CEFA" : "transparent"
        opacity: root.__showDetail ? 0.7 : 1

        Text {
            id: iconPoint
            anchors.left: parent.left
            anchors.leftMargin: 16
            height: parent.height
            visible: !root.__showDetail
            font.pixelSize: 14
            font.family: UIConfig.fontFamily
            font.weight: Font.Bold
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            textFormat: Text.RichText
            color: "#1E90FF"
            text: "*"
        }

        Text {
            anchors.left: iconPoint.right
            anchors.leftMargin: 8
            height: parent.height
            visible: !root.__showDetail
            font.pixelSize: 10
            font.family: UIConfig.fontFamily
            font.weight: Font.Bold
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            textFormat: Text.RichText
            color: "#696969"
            text: "销售总额: <span style='font-size:10px; color:#228B22; font-weight: normal'>%1</span>".arg((totalAmount / 100).toFixed(2))
        }

        Text {
            anchors.left: parent.left
            anchors.leftMargin: 8
            height: parent.height
            font.pixelSize: 10
            font.family: UIConfig.fontFamily
            font.weight: Font.Bold
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            color: "#000000"
            text: {
                var itemData = dataPoints[root.__currentIndex].date
                if('undefined' === itemData){
                    ""
                }
                else{
                    itemData
                }
            }
            visible: root.__showDetail
        }

        Text {
            anchors.right: parent.right
            anchors.rightMargin: 8
            height: parent.height
            font.pixelSize: 10
            font.family: UIConfig.fontFamily
            font.weight: Font.Bold
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            textFormat: Text.RichText
            color: "#000000"
            text: {
                var itemData = dataPoints[root.__currentIndex].amount
                if('undefined' === itemData){
                    ""
                }
                else{
                    "销售额: <span style='font-size:10px; color:#228B22; font-weight: normal'>%1</span>".arg(itemData.toFixed(2))
                }
            }
            visible: root.__showDetail
        }
    }

    Canvas {
        id: canvas
        anchors.top: parent.top
        width: parent.width
        height: 370

        property real lineX: 0
        property bool isDrawing: false

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            ctx.setLineDash([])
            var xStart = 50
            var yStart = 50
            var xEnd = width - 16
            var yEnd = 350
            var xLength = xEnd - xStart
            var yLength = 300
            var margin = 8
            var x = 0
            var y = 0

            // 绘制纵向坐标轴金额刻度, y轴6个刻度尺标题
            ctx.font = "10px Arial"
            ctx.fillStyle = "#A9A9A9"
            ctx.strokeStyle = "#A9A9A9";
            var yTextMetricsWidth = 0
            var maxAmount = getMaxAmount()
            for (var i = 5; i >= 0; i--) {
                var yText = (maxAmount / 5 * i).toFixed(2)
                var yTextMetrics = ctx.measureText(yText)
                if((yTextMetrics.width + margin) > yTextMetricsWidth){
                   yTextMetricsWidth = yTextMetrics.width + 2 * margin
                }
                ctx.fillText(yText, yTextMetricsWidth - yTextMetrics.width - margin, (yEnd - i * (yLength / 5)) + 5)
            }

            // 计算数据点在坐标轴上的位置, 横纵坐标轴的刻度差值
            xLength = xEnd - yTextMetricsWidth
            var numPoints = dataPoints.length
            var xIncrement = xLength / (numPoints - 1)
            var yIncrement = yLength / getMaxAmount()

            // 绘制横向坐标轴日期刻度标识标题
            for (var i = 0; i < datePoints.length; i++) {
                y = yEnd
                var xText = datePoints[i]
                var xSpacing = (xLength) / (datePoints.length - 1)
                var xTextMetrics = ctx.measureText(xText)
                x = yTextMetricsWidth + i * xSpacing
                if(i > 0){
                    x -= i * xTextMetrics.width / (datePoints.length - 1)
                }
                ctx.fillText(xText, x, y + margin * 2)
            }

            // 绘制坐标轴
            ctx.strokeStyle = "#A9A9A9"
            ctx.setLineDash([])
            ctx.lineWidth = 1
            ctx.beginPath()
            // x轴
            ctx.moveTo(yTextMetricsWidth - 2, yEnd)
            ctx.lineTo(xEnd + 2, yEnd)
            ////y轴
            //ctx.moveTo(yTextMetricsWidth, xStart)
            //ctx.lineTo(yTextMetricsWidth, yEnd)
            ctx.stroke()

            // 绘制曲线
            ctx.strokeStyle = "#1E90FF"
            ctx.lineWidth = 2 // 曲线的线宽，圆弧的线宽
            ctx.beginPath()
            var dataPointReals = []
            var roundDia = 4 // 最后一个圆点的直径
            var roundSize = 8 // 最后一个圆点的尺寸大小，圆的直径+圆弧线的宽度*2
            for (var i = 0; i < numPoints; i++) {
                x = yTextMetricsWidth + i * xIncrement
                y = yEnd - dataPoints[i].amount * yIncrement
                var lastX = x
                var lastY = y

                if (0 === i) {
                    ctx.moveTo(lastX, lastY)
                }
                // 最后一个点
                else if((numPoints - 1) === i){
                    if(dataPointReals[numPoints - 2].yPoint === y){
                        lastX -= (roundSize / 2)
                    }
                    else{
                        if(y === yStart){
                            lastY += (roundSize / 2)
                        }
                        else if(y === yEnd){
                            lastY -= (roundSize / 2)
                        }
                        else{
                            lastX -= (roundDia / 2)
                            lastY += (roundDia / 2)
                        }
                    }

                    ctx.quadraticCurveTo(dataPointReals[i -1].xPoint, dataPointReals[i -1].yPoint, lastX, lastY)
                }
                else {

                    ctx.quadraticCurveTo(dataPointReals[i -1].xPoint, dataPointReals[i -1].yPoint, lastX, lastY)
                }
                var dataPoint = {}
                dataPoint.xPoint = x
                dataPoint.yPoint = y
                dataPointReals.push(dataPoint)
            }
            ctx.stroke()

            // 填充阴影部分
            /**
            ctx.lineTo(x, yEnd) // 连接到 x 轴
            ctx.lineTo(yTextMetricsWidth, yEnd); // 连接回起始点
            ctx.closePath();
            ctx.fillStyle = "rgba(30, 144, 255, 0.2)" // 设置阴影部分的颜色和透明度
            ctx.fill() // 填充阴影部分
            **/

            // 绘制空心圆形在最后一个点的位置，圆心的位置
            ctx.beginPath()
            ctx.arc(x, y, roundDia, 0, Math.PI * 2)
            ctx.stroke()

            // 绘制竖线
            if(isDrawing){
                ctx.lineWidth = 1
                ctx.beginPath()
                if(lineX < xStart){
                    lineX = yTextMetricsWidth
                }
                else if(lineX > xEnd){
                    lineX = xEnd
                }

                var xData = 0
                var yData = 0
                var lastDataPoint = false
                for(var i = 0; i < dataPointReals.length; i++) {
                    if(0 === i){
                        xData = dataPointReals[i].xPoint
                        yData = dataPointReals[i].yPoint
                        root.__currentIndex = i
                        continue
                    }
                    if(Math.abs(dataPointReals[i].xPoint - lineX) <  Math.abs(xData - lineX)){
                        xData = dataPointReals[i].xPoint
                        yData = dataPointReals[i].yPoint
                        if(i == dataPointReals.length - 1){
                            lastDataPoint = true
                        }
                        root.__currentIndex = i
                    }
                }

                if(lastDataPoint){
                    // 竖线
                    if(yData === yEnd){
                        ctx.moveTo(xData, yStart)
                        ctx.lineTo(xData, yData - (roundSize / 2))
                    }
                    else if( yData === yStart){
                        ctx.moveTo(xData, yData + (roundSize / 2))
                        ctx.lineTo(xData, yEnd)
                    }
                    else{
                        ctx.moveTo(xData, yStart)
                        ctx.lineTo(xData, yData - (roundSize / 2) )

                        ctx.moveTo(xData, yData + (roundSize / 2))
                        ctx.lineTo(xData, yEnd)

                    }
                    ctx.stroke()
                }
                else{
                    // 竖线
                    ctx.moveTo(xData, yStart)
                    ctx.lineTo(xData, yEnd)
                    ctx.stroke()

                    // 竖线和曲线相交的实心圆点
                    ctx.fillStyle = "#1E90FF";
                    ctx.beginPath();
                    ctx.arc(xData, yData, roundDia, 0, Math.PI * 2)
                    ctx.fill()
                    ctx.stroke()
                }
            }

            // 绘制背景虚线
            ctx.strokeStyle = "#A9A9A9";
            ctx.setLineDash([5, 5]) // 设置虚线样式，交替绘制5像素线和5像素空白
            ctx.lineWidth = 1 // 设置线条宽度
            for (var i = 5; i >= 0; i--) {
                if(i > 0){
                    ctx.beginPath()
                    ctx.moveTo(yTextMetricsWidth - 2, (yEnd - i * (yLength / 5)))
                    ctx.lineTo(xEnd + 2, (yEnd - i * (yLength / 5)))
                    ctx.stroke()
                }
            }
            ctx.setLineDash([])
        }

        MouseArea {
            id: swipeArea
            anchors.fill: parent
            onPressed: {
                canvas.lineX = mouse.x
                canvas.isDrawing = true
                canvas.requestPaint()
                root.__showDetail = true
            }

            onPositionChanged: {
                if (canvas.isDrawing) {
                    canvas.lineX = mouse.x
                    canvas.requestPaint()
                }
            }

            onReleased: {
                canvas.isDrawing = false
                canvas.requestPaint()
                root.__showDetail = false
            }
        }

        function getMaxAmount() {
            var maxAmount = 0
            for (var i = 0; i < dataPoints.length; i++) {
                var amount = dataPoints[i].amount
                if (amount  > maxAmount) {
                    maxAmount = Math.ceil(amount / 100) * 100
                }
            }
            return maxAmount
        }
    }
}
