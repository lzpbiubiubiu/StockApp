﻿#pragma once
#include <QString>

namespace Base
{
    /** 路径工具类，提供目录创建、目录路径查询等功能 */
    class PathUtil
    {
    public:
        /** 创建目录 */
        static QString CreateDir(const QString& dirPath);

        /** 删除目录 */
        static bool DeleteDir(const QString& dirPath);

        /** 拷贝目录 */
        static bool CopyDir(QString const& srcDir, QString const& destDir, bool force = true);

        /** 拷贝文件 */
        static bool CopyFileToPath(const QString& srcFile, const QString& destPath, bool force = true);

        /** 获取程序根目录 */
        static QString GetApplicationDir();

        /** 获取数据目录路径 C:/Users/用户名/Documents/Stock/ */
        static QString GetDataDir();

        /** 获取数据目录路径 C:/Users/用户名/Documents/Stock/Database/ */
        static QString GetDatabaseDir();

        /** 获取配置缓存目录路径 C:/Users/用户/Documents/Stock/ConfigCache/ */
        static QString GetConfigCacheDir();

        /** 获取崩溃转储目录路径 C:/Users/用户/Documents/Stock/CrashDump/ */
        static QString GetDumpDir();

        /** 获取日志目录路径 AppDir/Logs/ */
        static QString GetLogDir();

        /** 获取临时目录路径 C:/Users/用户/AppData/Local/Temp/ */
        static QString GetTempDir();

        /** 获取程序临时路径 C:/Users/用户/AppData/Local/Temp/Stock/ */
        static QString GetTempStockDir();

        /** 获取补丁下载路径 C:/Users/用户/AppData/Local/Temp/Patch/App/ */
        static QString GetAppPatchDir();

        /** 获取资源目录路径 C:/Users/用户/Documents/PosData/Assets/ */
        static QString GetAssetsDir();
    };
};
