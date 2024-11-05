#pragma once

/**
* @file         JsonUtil.h
* @brief        Json封装方法
* @copyright    lizhongpu
*/

#include <QJsonObject>

namespace Base
{
    /**
     * @brief The JsonUtil class Json操作便捷类
     */
    class JsonUtil
    {
    public:
        /** QByteArray转QJsonObject */
        static QJsonObject ToJsonObject(const QByteArray& byteData);

        /** 字符串转QJsonObject */
        static QJsonObject ToJsonObject(const QString& str);

        /** QJsonObject转字符串 */
        static QString ToString(const QJsonObject& jsonObj, bool indent = false);

    protected:
        JsonUtil() {};
        ~JsonUtil() {};
    };
} // namespace Base
