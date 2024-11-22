#include <QUrl>
#include "UrlManager.h"
#include "Core/GlobalData.h"
namespace Core
{

    UrlManager::UrlManager(QObject* parent/* = nullptr*/)
        : Base::ServiceBase(parent)
    {}

    UrlManager::~UrlManager()
    {}

    void UrlManager::AddUrl(Key key, const QString& host, const QString& path, int timeout, const QString& description/* = QString()*/)
    {
        if(m_urlTabs.contains(key))
            return;
        Param param;
        param.path = path;
        param.timeout = timeout;
        param.description = description;
        param.url = param.host + param.path;

        m_urlTabs[key] = param;
    }

    UrlManager::Param UrlManager::GetUrl(Key key) const
    {
        if(!m_urlTabs.contains(key))
            return Param();

        Param param = m_urlTabs[key];
        const auto& app = Core::GlobalData::Get()->GetConfig().app;
        param.host = QString("http://%1:%2").arg(app.ip).arg(app.port);
        param.url = param.host + param.path;
        return param;
    }

    UrlManager::HostInfo UrlManager::GetGWHostInfo() const
    {
        auto gwInfo = QUrl();

        HostInfo info;
        info.scheme = gwInfo.scheme();
        info.host = gwInfo.host();
        info.port = gwInfo.port();

        return info;
    }

    bool UrlManager::OnStart()
    {
        AddUrl(HEARTBEAT, "", "/stockServer/terminal/heartbeat", 5000, QStringLiteral("心跳"));

        AddUrl(USER_LOGIN, "", "/stockServer/terminal/userLogin", 10000, QStringLiteral("用户登录"));

        AddUrl(GET_CONFIG_WARES, "", "/stockServer/terminal/getConfigWares", 5000, QStringLiteral("获取首页配置商品列表"));
        AddUrl(HYTRADE_INFO, "", "/stockServer/sale/hytradeInfo", 5000, QStringLiteral("出库订单结算"));
        AddUrl(MODIFY_CONFIG_WARE, "", "/stockServer/storage/modifyConfigWare", 5000, QStringLiteral("商品入库"));
        AddUrl(QUERY_ORDER_REPORT, "", "/stockServer/sale/queryOrderReport", 5000, QStringLiteral("查询订单报表"));
        AddUrl(QUERY_ORDER_STOCK_LIST, "", "/stockServer/sale/queryOrderStockList", 5000, QStringLiteral("查询备货清单列表"));
        AddUrl(MODIFY_STOCK_ORDER, "", "/stockServer/sale/modifyStockOrder", 5000, QStringLiteral("备货订单修改"));

        return true;
    }

    void UrlManager::OnStop()
    {}
}// namespace Extra
