#pragma once

#include <QString>

namespace Base
{
    /** 数学工具类，提供常用算法封装 */
    class MathUtil
    {
    public:
        /** （小写）生成32位UUID字符串 */
        static QString GetUUID();

        /** （小写）生成16位随机字符串 */
        static QString GetNonce();

        /** （小写）计算MD5摘要字符串 */
        static QString GetMD5(const QString& content);

        /** （小写）计算文件MD5摘要字符串 */
        static QString GetFileMD5(const QString& fileName);

        /** （小写）计算SHA256摘要字符串 */
        static QString GetSHA256(const QString& content);

        /** 生成32位指定范围内的随机整数 */
        static int GetRandomInt(int min, int max);

        /** 判断两个浮点数是否相等 */
        static bool IsFloatEqual(double a, double b, double epsilon = 1e-6);

        /** 数字字符串加密 */
        static QString GetCipherNumber(const QString& plain);

        /** 数字字符串解密 */
        static QString GetPlainNumber(const QString& cipher);

        /** 计算图片文件Base64的值 */
        static QString GetImageFileBase64(const QString& imagePath);

        /** 判断字符串是否为纯数字 */
        static bool IsNumber(const QString& text);

        /** 判断字符串是否为纯英文 */
        static bool IsEnglish(const QString& text);
    };
};
