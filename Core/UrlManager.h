#pragma once

/**
* @file         UrlManager.h
* @brief        url管理类
* @details      负责客户端请求url管理
* @copyright    lizhongpu
*/

#include <QHash>
#include <QString>

#include "Base/Common/ServiceBase.h"

namespace Core
{
    /** url管理类 */
    class UrlManager : public Base::ServiceBase
    {
        Q_OBJECT

    public:
        /** URL标识 */
        enum Key
        {
            /** 心跳 */
            HEARTBEAT = 0,

            /** 获取首页商品列表 */
            GET_CONFIG_WARES,

            /** 出库订单结算 */
            HYTRADE_INFO,

            /** 用户登录 */
            USER_LOGIN,

            /** 修改商品 */
            MODIFY_CONFIG_WARE,

            /** 查询订单报表 */
            QUERY_ORDER_REPORT
        };

        /** URL参数 */
        struct Param
        {
            /** 主机 */
            QString host;

            /** 路径 */
            QString path;

            /** 超时（毫秒）*/
            int timeout = 0;

            /** URL */
            QString url;

            /** 描述 */
            QString description;
        };
    
        /** HOST数据结构 */
        struct HostInfo
        {
            /** scheme协议类型 */
            QString scheme;

            /** user 协议用户 */
            QString user;

            /** password 密码 */
            QString password;

            /** host 主机 */
            QString host;

            /** port 端口 */
            int port = 0;
        };
    public:
        UrlManager(QObject* parent = nullptr);
        virtual ~UrlManager();

        /**
        * @brief    添加/注册URL
        * @param    key         对应的枚举Key
        * @param    host        host
        * @param    path        url路径
        * @param    timeout     超时时间（毫秒）
        * @param    description 描述
        */
        void AddUrl(Key key, const QString& host, const QString& path, int timeout, const QString& description = QString());

        /**
        * @brief    获取URL
        * @return   URL参数
        */
        Param GetUrl(Key key) const;

        /**
        * @brief    获取GW主机信息
        * @return   GW主机信息
        */
        HostInfo GetGWHostInfo() const;

    protected:
        virtual bool OnStart() override;

        virtual void OnStop() override;

    private:
        QHash<Key, Param> m_urlTabs;
    };
}// namespace Extra
