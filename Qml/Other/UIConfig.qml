pragma Singleton
import QtQml 2.12
import QtQuick 2.12
import QtQuick.Window 2.12

QtObject
{
    id: config
    objectName: "config"

    // 全局字体
    property string fontFamily: "Microsoft YaHei UI" //__winxp ? "SimSun" : "Microsoft YaHei UI"
}
