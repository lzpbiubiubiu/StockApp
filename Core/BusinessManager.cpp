#include <QFile>
#include <QRandomGenerator>
#include "Base/Common/ServiceManager.h"
#include "Base/Util/MathUtil.h"
#include "Base/Log/Log.h"
#include "Base/Http/HttpModule.h"
#include "Protocol/StockAppRequest.h"
#include "Protocol/Terminal/TerminalGetConfigwaresResponse.h"
#include "Protocol/Hytrade/HytradeInfoRequest.h"
#include "Protocol/Hytrade/HytradeInfoResponse.h"
#include "Protocol/Hytrade/ModifyStockOrderRequest.h"
#include "Protocol/Terminal/TerminalUserLoginRequest.h"
#include "Protocol/Terminal/TerminalUserLoginResponse.h"
#include "Protocol/Storage/ModifyConfigWareRequest.h"
#include "Protocol/Query/QueryOrderReportRequest.h"
#include "Protocol/Query/QueryOrderReportResponse.h"
#include "Protocol/Query/QueryOrderStockListResponse.h"
#include "Core/UrlManager.h"
#include "Core/SequenceGenerator.h"
#include "Core/DatabaseManager.h"
#include "Core/GlobalData.h"
#include "BusinessManager.h"

namespace Core
{
    BusinessManager::BusinessManager(QObject* parent)
        : ServiceBase(parent)
    {

    }

    BusinessManager::~BusinessManager()
    {}

    void BusinessManager::FetchConfigWares()
    {
        QMetaObject::invokeMethod(this, "onFetchConfigWares");
    }

    void BusinessManager::Login(const QString& account, const QString& password)
    {
        QMetaObject::invokeMethod(this, "onLogin", Q_ARG(QString, account), Q_ARG(QString, password));
    }

    void BusinessManager::onFetchConfigWares()
    {
        auto urlParam = Base::GetService<Core::UrlManager>()->GetUrl(Core::UrlManager::GET_CONFIG_WARES);
        auto request = QSharedPointer<Net::StockAppRequest>::create();
        auto response = QSharedPointer<Net::TerminalGetConfigwaresResponse>::create();

        request->SetUrl(urlParam.url);
        request->SetTimeout(urlParam.timeout);
        Net::HttpModule::Get()->ProcessRequest(request, response, true);
        if(response->IsOk())
        {
            m_configWares.clear();
            auto& httpData = response->GetResult();
            for(const auto& item : httpData.wares)
            {
                auto wareItemPtr = Core::WareItemPtr::create();
                wareItemPtr->name = item.name;
                wareItemPtr->uuid = Base::MathUtil::GetUUID();
                wareItemPtr->count = 1;
                wareItemPtr->retailPrice = item.retailPrice;
                wareItemPtr->wholesalePrice = item.wholesalePrice;
                wareItemPtr->stock = item.stock;
                wareItemPtr->code = item.code;
                wareItemPtr->extension = item.extension;
                m_configWares << wareItemPtr;
            }

            LOG_INFO(QStringLiteral("---------获取配置商品列表成功-----------"));
            emit signalLoadConfigWaresSuccess();

        }
        else
        {
            LOG_INFO(QStringLiteral("---------获取配置商品列表失败 %1-----------").arg(response->GetErrorMessage()));
            emit signalLoadConfigWaresError(response->GetErrorMessage());
        }
    }

    void BusinessManager::onLogin(const QString& account, const QString& password)
    {
        auto urlParam = Base::GetService<Core::UrlManager>()->GetUrl(Core::UrlManager::USER_LOGIN);
        auto response = QSharedPointer<Net::TerminalUserLoginResponse>::create();
        auto request = QSharedPointer<Net::TerminalUserLoginRequest>::create();
        request->SetAccount(account);
        request->SetPassword(password);
        request->SetUrl(urlParam.url);
        request->SetTimeout(urlParam.timeout);
        Net::HttpModule::Get()->ProcessRequest(request, response, true);
        if(response->IsOk())
        {
            auto& httpData = response->GetResult();
            auto& user = Core::GlobalData::Get()->GetUser();
            user.account = httpData.userInfo.account;
            user.name = httpData.userInfo.name;
            user.pwd = httpData.userInfo.password;
            user.tel = httpData.userInfo.tel;
            user.status = (Core::User::Status)httpData.userInfo.status;
            emit signalLoginSuccess();
        }
        else
        {
            LOG_INFO(QStringLiteral("---------用户登录失败：%1-----------").arg(response->GetErrorMessage()));
            emit signalLoginError(response->GetErrorMessage());
        }

    }

    void BusinessManager::SubmitSaleOrder(qint64 wholeOrderPromoAmt, qint64 deliveryFeesAmt, const QString& remarkText)
    {
        QMetaObject::invokeMethod(this, "onSubmitSaleOrder", Q_ARG(qint64, wholeOrderPromoAmt), Q_ARG(qint64, deliveryFeesAmt), Q_ARG(QString, remarkText));
    }

    void BusinessManager::UpdateAppCache(const QString& ip, int port)
    {
        QMetaObject::invokeMethod(this, "onUpdateAppCache", Q_ARG(QString, ip), Q_ARG(int, port));
    }

    void BusinessManager::ConfigWareStorage(Core::WareItemPtr item, StorageOperate operate)
    {
        QMetaObject::invokeMethod(this, "onConfigWareStorage", Q_ARG(Core::WareItemPtr, item), Q_ARG(StorageOperate, operate));
    }

    bool BusinessManager::OnStart()
    {
        if(!QFile::exists(APP_CACHE_FILE))
            WriteAppCache(APP_CACHE_FILE);
        else
            ReadAppCache(APP_CACHE_FILE);

        // 恢复上次登录用户的账号信息
        RestoreLoginUser();

        // 恢复异常订单
        RestoreOrder();
        return true;
    }

    void BusinessManager::OnStop()
    {

    }

    bool BusinessManager::WriteAppCache(const QString& fileName, const QString& ip, int port) const
    {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly))
        {
            LOG_ERROR(QString("%1 file open failed").arg(fileName));
            return false;
        }

        auto& app = Core::GlobalData::Get()->GetConfig().app;

        app.render = Core::Render::RENDER_HARDWARE;
        app.lang = "en_US";
        app.ip = ip;
        app.port = port;
        QJsonObject root;
        if(app.render == Core::Render::RENDER_SOFTWARE)
            root["render"] = "software";
        else if(app.render == Core::Render::RENDER_HARDWARE)
            root["render"] = "hardware";
        else
            Q_ASSERT_X(0, "WriteApp()", "render type is invalid");
        root["ip"] = app.ip;
        root["port"] = app.port;

        QJsonDocument doc;
        doc.setObject(root);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.flush();
        file.close();
        return true;
    }

    bool BusinessManager::ReadAppCache(const QString& fileName) const
    {
        auto& app = Core::GlobalData::Get()->GetConfig().app;

        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            LOG_ERROR(QString("%1 file open failed").arg(fileName));
            return false;
        }
        QByteArray data = file.readAll();
        file.close();

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if(parseError.error != QJsonParseError::NoError)
        {
            LOG_ERROR(QString("%1 json parse failed").arg(fileName));
            return false;
        }

        QJsonObject root = doc.object();

        if(root["render"].toString().compare("software", Qt::CaseInsensitive) == 0)
            app.render = Core::Render::RENDER_SOFTWARE;
        else if(root["render"].toString().compare("hardware", Qt::CaseInsensitive) == 0)
            app.render = Core::Render::RENDER_HARDWARE;
        else
            Q_ASSERT_X(0, "ReadApp()", "render type is invalid");

        app.lang = root["lang"].toString();
        app.ip = root["ip"].toString();
        app.port = root["port"].toVariant().toLongLong();

        return true;
    }

    const Core::WareItemPtrList& BusinessManager::GetConfigWares() const
    {
        return m_configWares;
    }

    Core::WareItemPtr BusinessManager::CreatorConfigWare(int index)
    {
        if(index < 0 || index >= m_configWares.size())
        {
            return nullptr;
        }
        Core::WareItemPtr wareItem = Core::WareItemPtr::create(*m_configWares[index]);
        wareItem->uuid = Base::MathUtil::GetUUID();
        return wareItem;
    }

    void BusinessManager::AddSaleWareItem(Core::WareItemPtr item, Core::Order::OrderType type)
    {
         QMetaObject::invokeMethod(this, "onAddSaleWareItem", Q_ARG(Core::WareItemPtr, item), Q_ARG(Core::Order::OrderType, type));
    }

    void BusinessManager::AddPresentationWareItem(Core::WareItemPtr item)
    {
         QMetaObject::invokeMethod(this, "onAddPresentationWareItem", Q_ARG(Core::WareItemPtr, item));
    }

    void BusinessManager::UpdateSaleWareItem(int index, ItemAction action, const QVariantList& values)
    {
        QMetaObject::invokeMethod(this, "onUpdateSaleWareItem", Q_ARG(int, index), Q_ARG(ItemAction, action),Q_ARG(QVariantList, values));
    }

    void BusinessManager::DeleteSaleWareItem(int index)
    {
        QMetaObject::invokeMethod(this, "onDeleteSaleWareItem", Q_ARG(int, index));
    }

    void BusinessManager::onAddSaleWareItem(Core::WareItemPtr item, Core::Order::OrderType type)
    {
        Q_ASSERT(item != nullptr);
        Q_ASSERT(item->count > 0);

        QString message;
        if(m_codeSaleLookup.contains(item->code))
        {
            auto updateItem = m_codeSaleLookup[item->code];
            if(DoUpdateSaleItemCount(updateItem, updateItem->count + item->count, message))
            {

                LOG_INFO(QStringLiteral("---------合行商品行-----------"));
                emit signalSaleMergeItemSuccess(updateItem);
                return;
            }
        }
        else
        {
            QString message;
            // 创建出库订单
            if(!m_saleOrder)
            {
                auto dbm = Base::GetService<Core::DatabaseManager>();
                m_orderDml.SetDatabase(dbm->GetTempDatabase());

                m_saleOrder = CreateOrder();
                m_saleOrder->type = type;

                if(!m_orderDml.AddOrder(m_saleOrder, message))
                {
                    emit signalAddSaleWaresError(QStringLiteral("数据库订单创建失败：") + message);
                    return;
                }
            }

            item->orderUuid = m_saleOrder->uuid;
            if(Core::Order::RETAIL == m_saleOrder->type)
                item->price = item->retailPrice;
            else if(Core::Order::WHOLESALE == m_saleOrder->type)
                item->price = item->wholesalePrice;
            else
                item->price = item->retailPrice;
            item->discountPrice = item->price;
            item->promotionAmount = (item->price - item->discountPrice);
            item->amount = item->count *  item->discountPrice;
            m_saleOrder->items.prepend(item);
            if(!m_orderDml.AddItem(item, message))
            {
                emit signalAddSaleWaresError(QStringLiteral("数据库出库商品失败：") + message);
                return;
            }

            m_codeSaleLookup[item->code] = item;
            m_saleOrder->orderAmount += item->amount;
            m_saleOrder->promotionAmount += item->promotionAmount;
            m_saleOrder->kind++;
            m_saleOrder->piece++;

            // 更新订单
            QString sqlError;
            if(!m_orderDml.UpdateOrder(m_saleOrder, sqlError))
            {
                QString message = QStringLiteral("sql error: %1").arg(sqlError).toUtf8();
                LOG_ERROR(message.toUtf8());
                emit signalAddSaleWaresError(QStringLiteral("数据库订单更新失败：") + message);
                return;
            }
        }

        emit signalAddSaleWaresSuccess(item);
    }

    void BusinessManager::onAddPresentationWareItem(Core::WareItemPtr item)
    {
        Q_ASSERT(item != nullptr);
        Q_ASSERT(item->count > 0);

        QString message;
        if(m_codePresentationLookup.contains(item->code))
        {
            auto updateItem = m_codePresentationLookup[item->code];
            if(DoUpdateSaleItemCount(updateItem, updateItem->count + item->count, message))
            {

                LOG_INFO(QStringLiteral("---------赠送合行商品行-----------"));
                emit signalSaleMergeItemSuccess(updateItem);
                return;
            }
        }
        else
        {
            QString message;
            m_saleOrder->items.prepend(item);
            if(!m_orderDml.AddItem(item, message))
            {
                emit signalPresentationWareError(QStringLiteral("数据库出库商品失败：") + message);
                return;
            }

            m_codePresentationLookup[item->code] = item;
            m_saleOrder->orderAmount += item->amount;
            m_saleOrder->promotionAmount += item->promotionAmount;
            m_saleOrder->piece += item->count;
            m_saleOrder->kind++;

            // 更新订单
            QString sqlError;
            if(!m_orderDml.UpdateOrder(m_saleOrder, sqlError))
            {
                QString message = QStringLiteral("sql error: %1").arg(sqlError).toUtf8();
                LOG_ERROR(message.toUtf8());
                emit signalPresentationWareError(QStringLiteral("数据库订单更新失败：") + message);
                return;
            }
        }

        emit signalAddSaleWaresSuccess(item);
    }

    void BusinessManager::onUpdateSaleWareItem(int index, ItemAction action, const QVariantList& values)
    {
        if(!m_saleOrder)
        {
            emit signalUpdateSaleItemError("订单未创建");
            return;
        }

        // 索引检查
        if(index < 0 || index >= m_saleOrder->items.size())
        {
            emit signalUpdateSaleItemError(QString("Failed to edit item, index out of range, index: %1").arg(index));
            return;
        }
        bool result = false;
        QString message;
        auto item = m_saleOrder->items[index];

        if(ItemAction::UPDATE_COUNT == action)
        {
            Q_ASSERT(values.size() >= 1);
            result = DoUpdateSaleItemCount(item, values[0].toInt(), message);
        }
        else if(ItemAction::ADD_COUNT == action)
        {
            Q_ASSERT(values.size() >= 1);
            result = DoUpdateSaleItemCount(item, item->count + values[0].toInt(), message);
        }
        else if(action == ItemAction::REDUCE_COUNT)
        {
            Q_ASSERT(values.size() >= 1);
            result = DoUpdateSaleItemCount(item, item->count - values[0].toInt(), message);
        }
        else if(action == ItemAction::UPDATE_DISCOUNT_PRICE)
        {
            Q_ASSERT(values.size() >= 1);
            result = DoUpdateSaleItemDiscountPriceData(item, values[0].toLongLong(), message);
        }
        else if(action == ItemAction::UPDATE_PRESENTATION)
        {
            Q_ASSERT(values.size() >= 1);

            // 当购物车已经存在赠送的商品
            if(m_codePresentationLookup.contains(item->code))
            {
                // 更新原来的商品赠送数量
                m_codeSaleLookup.remove(item->code);
                auto updateItem = m_codePresentationLookup[item->code];
                if(!DoUpdateSaleItemCount(updateItem, updateItem->count + item->count, message))
                {
                    emit signalPresentationWareError(QStringLiteral("商品赠送失败"));
                    LOG_ERROR(message.toUtf8());
                    return;
                }

                // 删除新的商品
                m_saleOrder->items.removeAt(index);
                QString sqlError;
                if(!m_orderDml.DeleteItem(item->uuid, sqlError))
                {
                    QString message = QStringLiteral("sql error: %1").arg(sqlError);
                    LOG_ERROR(message.toUtf8());
                    emit signalPresentationWareError(QStringLiteral("商品赠送失败"));
                    return;
                }
                m_saleOrder->kind--;

                if(!m_orderDml.UpdateOrder(m_saleOrder, sqlError))
                {
                    message = QStringLiteral("sql error: %1").arg(sqlError).toUtf8();
                    LOG_ERROR(message.toUtf8());
                    emit signalPresentationWareError(QStringLiteral("商品赠送失败"));
                    result = false;
                }
                emit signalPresentationWareSuccess(index, updateItem);
                return;
            }
            else
            {
                result = DoUpdateItemPresentationData(item, message);
            }

        }
        else
        {
            Q_ASSERT(0);
        }

        // 更新订单
        QString sqlError;
        if(!m_orderDml.UpdateOrder(m_saleOrder, sqlError))
        {
            message = QStringLiteral("sql error: %1").arg(sqlError).toUtf8();
            LOG_ERROR(message.toUtf8());
            result = false;
        }

        if(result)
        {
            emit signalUpdateSaleItemSuccess(index, item);
        }
        else
        {
            emit signalUpdateSaleItemError(message);
        }
    }

    void BusinessManager::onDeleteSaleWareItem(int index)
    {
        if(!m_saleOrder)
        {
            emit signalUpdateSaleItemError("订单未创建");
            return;
        }

        // 索引检查
        if(index < 0 || index >= m_saleOrder->items.size())
        {
            emit signalUpdateSaleItemError(QString("Failed to edit item, index out of range, index: %1").arg(index));
            return;
        }

        auto item = m_saleOrder->items[index];
        m_saleOrder->items.removeAt(index);
        QString sqlError;
        if(!m_orderDml.DeleteItem(item->uuid, sqlError))
        {
            QString message = QStringLiteral("sql error: %1").arg(sqlError);
            LOG_ERROR(message.toUtf8());
            return;
        }

        m_saleOrder->orderAmount -= item->amount;
        m_saleOrder->piece -= item->count;
        m_saleOrder->kind--;
        m_codeSaleLookup.remove(item->code);

        if(Core::WareItem::WareType::WARE_TYPE_GIFT == item->wareType)
        {
            m_codePresentationLookup.remove(item->code);
        }

        if(m_saleOrder->items.empty())
        {
            if(!m_orderDml.DeleteOrder(m_saleOrder->uuid, sqlError))
            {
                QString message = QStringLiteral("sql error: %1").arg(sqlError);
                LOG_ERROR(message.toUtf8());
            }
            m_saleOrder.clear();
        }
        else
        {
            // 更新订单
            QString sqlError;
            if(!m_orderDml.UpdateOrder(m_saleOrder, sqlError))
            {
                QString message = QStringLiteral("sql error: %1").arg(sqlError).toUtf8();
                LOG_ERROR(message.toUtf8());
                emit signalUpdateSaleItemError(message);
                return;
            }
        }
        emit signalDeletedSaleItem(index);
    }

    OrderPtr BusinessManager::CreateOrder() const
    {
        auto order = OrderPtr::create();
        order->uuid = Base::MathUtil::GetUUID();
        order->time = QDateTime::currentDateTime();
        auto sg = Base::GetService<Core::SequenceGenerator>();
        order->receiptNo = sg->GetReceiptNumber(Core::SequenceGenerator::SequenceName::SALE);
        return order;
    }

    bool BusinessManager::DoUpdateSaleItemCount(WareItemPtr item, int count, QString& message)
    {
        if(count <= 0)
        {
            message = QStringLiteral("商品数量不能为0");
            return false;
        }

        if(count > 999)
        {
            message = QStringLiteral("单个商品数量最多为999");
            return false;
        }
        // 更新订单统计信息：kind|piece
        double q1 = item->count;
        double q2 = count;
        if(q1 > 0 && q2 == 0)
        {
            m_saleOrder->kind--;
            m_codeSaleLookup.remove(item->code);
            m_saleOrder->piece -= q2;
        }
        else
        {
            // 品数不变
            // q1 == 0 && q2 == 0
            // q1 > 0 && q2 > 0
            m_saleOrder->piece += (q2 - q1);
        }
        item->count = count;
        qint64 amount1 = item->amount;
        qint64 promotionAmount1 = (q1 * item->promotionAmount);
        item->amount = item->discountPrice * item->count;
        m_saleOrder->orderAmount += (item->amount - amount1);
        qint64 promotionAmount2 = (item->count * item->promotionAmount);
        m_saleOrder->promotionAmount += promotionAmount2 - promotionAmount1;

        QString sqlError;
        if(!m_orderDml.UpdateItem(item, sqlError))
        {
            message = tr("sql error: %1").arg(sqlError);
            LOG_ERROR(message.toUtf8());
            return false;
        }
        return true;
    }

    bool BusinessManager::DoUpdateSaleItemDiscountPriceData(WareItemPtr item, qint64 discountPrice, QString& message)
    {
        if(discountPrice < 0)
        {
            message = QString("修改商品折扣价失败，参数无效：discountPrice=%1，需要正数值").arg(discountPrice);
            return false;
        }

        qint64 amount1 = item->amount;
        qint64 promotionAmount1 = (item->count * item->promotionAmount);

        item->discountPrice = discountPrice;
        item->amount = item->discountPrice * item->count;
        item->promotionAmount = item->price - item->discountPrice;

        m_saleOrder->orderAmount += item->amount - amount1;
        m_saleOrder->promotionAmount += (item->count * item->promotionAmount) - promotionAmount1;

        QString sqlError;
        if(!m_orderDml.UpdateItem(item, sqlError))
        {
            message = tr("sql error: %1").arg(sqlError);
            LOG_ERROR(message.toUtf8());
            return false;
        }
        return true;
    }

    bool BusinessManager::DoUpdateItemPresentationData(WareItemPtr item, QString& message)
    {
        qint64 amount1 = item->amount;
        qint64 promotionAmount1 = item->promotionAmount * item->count;

        // 商品行全部赠送
        item->wareType = Core::WareItem::WARE_TYPE_GIFT;
        item->discountPrice = 0;
        item->promotionAmount = 0;
        item->amount = 0;
        m_codeSaleLookup.remove(item->code);
        m_codePresentationLookup[item->code] = item;

        // 更新订单统计信息：orderAmount|discountAmount
        m_saleOrder->orderAmount += item->amount - amount1;
        m_saleOrder->promotionAmount -= promotionAmount1;

        QString sqlError;
        if(!m_orderDml.UpdateItem(item, sqlError))
        {
            message = tr("sql error: %1").arg(sqlError);
            LOG_ERROR(message.toUtf8());
            return false;
        }

        // 更新订单
        if(!m_orderDml.UpdateOrder(m_saleOrder, sqlError))
        {
            message = QStringLiteral("sql error: %1").arg(sqlError).toUtf8();
            LOG_ERROR(message.toUtf8());
            return false;
        }

        return true;

    }

    Core::OrderPtr BusinessManager::GetCurrentSaleOrder()
    {
        return m_saleOrder;
    }

    void BusinessManager::RestoreLoginUser()
    {
        auto lastUserAccount = Core::GlobalData::Get()->GetValue("userAccount");
        emit signalRestoreUserAccount(lastUserAccount);
    }

    void BusinessManager::RestoreOrder()
    {
        auto dbm = Base::GetService<Core::DatabaseManager>();
        m_orderDml.SetDatabase(dbm->GetTempDatabase());

        // 列出临时库所有订单
        QString sqlError;
        OrderPtrList orders;
        if(!m_orderDml.ListOrders(orders, sqlError))
        {
            QString message = tr("sql error: %1").arg(sqlError);
            return;
        }

        // 没有订单
        if(orders.isEmpty())
            return;

        // 查找异常退出时的订单（可能是数据库最后一单，也可能是位于中间的取单）
        OrderPtr lastSale;
        for(int i = orders.size() - 1; i >= 0; --i)
        {
            // 状态不匹配
            if(orders[i]->status != Order::OrderStatus::CREATE)
                continue;

            lastSale = orders[i];
            break;
        }

        // 未找到
        if(lastSale)
        {
            m_saleOrder = lastSale;
            // 恢复商品数据
            if(!m_orderDml.FindItemByOrderUuid(m_saleOrder->items, m_saleOrder->uuid, false, sqlError))
            {
                QString message = tr("sql error: %1").arg(sqlError);
                LOG_ERROR(message.toUtf8());
                return;
            }
            // 重建缓存
            m_codeSaleLookup.clear();
            m_codePresentationLookup.clear();
            for(const auto& i : m_saleOrder->items)
            {
                if(i->count > 0)
                {

                    if(Core::WareItem::WareType::WARE_TYPE_GIFT == i->wareType)
                    {
                        m_codePresentationLookup[i->code] = i;
                    }
                    else
                    {
                        m_codeSaleLookup[i->code] = i;
                    }
                    m_saleOrder->kind++;
                    m_saleOrder->piece += i->count;
                }
            }
        }

        emit signalRestoreOrderSuccess();
    }

    void BusinessManager::onSubmitSaleOrder(qint64 wholeOrderPromoAmt, qint64 deliveryFeesAmt, const QString& remarkText)
    {
        if(!m_saleOrder)
        {
            emit signalSubmitSaleOrderError(QStringLiteral("订单不存在"));
            return;
        }

        // 无商品返回
        if(m_saleOrder->piece <= 0)
        {
            emit signalSubmitSaleOrderError(QStringLiteral("订单不存在商品"));
            return;
        }
        m_saleOrder->wholeOrderPromoAmount = wholeOrderPromoAmt;
        m_saleOrder->orderAmount -= m_saleOrder->wholeOrderPromoAmount;

        auto response = QSharedPointer<Net::HytradeInfoResponse>::create();
        auto request = QSharedPointer<Net::HytradeInfoRequest>::create();

        // 过滤数量为0的商品并倒序
        QList<WareItemPtr> items;
        for(int i = m_saleOrder->items.size() - 1; i >= 0; --i)
        {
            items << m_saleOrder->items[i];
        }
        request->SetWareList(items);
        request->SetReceiptNo(m_saleOrder->receiptNo);
        request->SetOrderAmount(m_saleOrder->orderAmount);
        request->SetOrderType(m_saleOrder->type);
        request->SetTotalPromotionAmount(m_saleOrder->promotionAmount + m_saleOrder->wholeOrderPromoAmount);
        request->SetWarePromotionAmount(m_saleOrder->promotionAmount);
        request->SetDeliveryFeesAmount(deliveryFeesAmt);
        request->SetWholeOrderPromotionAmount(m_saleOrder->wholeOrderPromoAmount);
        request->SetOrderRemark(remarkText);
        auto urlParam = Base::GetService<Core::UrlManager>()->GetUrl(Core::UrlManager::HYTRADE_INFO);
        request->SetUrl(urlParam.url);
        request->SetTimeout(urlParam.timeout);
        QString message;

        Net::HttpModule::Get()->ProcessRequest(request, response, true);
        if(response->IsOk())
        {
            LOG_INFO(QStringLiteral("---------生成备货订单成功-----------"));

            if(!FinishOrder(message))
            {
                emit signalSubmitSaleOrderError(message);
            }
            else
            {
                emit signalSubmitSaleOrderSuccess();
            }
        }
        else
        {
            LOG_INFO(QStringLiteral("---------生成备货订单失败-----------"));
            QString errorData = response->GetData();
            QString errorCode = response->GetError();
            message = response->GetErrorMessage();
            if("1004" == errorCode)
            {
                message = errorData;
            }
            emit signalSubmitSaleOrderError(message);
        }

    }

    void BusinessManager::onUpdateAppCache(const QString& ip, int port)
    {
        WriteAppCache(APP_CACHE_FILE, ip, port);
        LOG_INFO(QString("Update App Cache Success"));
    }

    void BusinessManager::onConfigWareStorage(Core::WareItemPtr item, StorageOperate operate)
    {
        Q_ASSERT(item != nullptr);
        auto response = QSharedPointer<Net::StockAppResponse>::create();
        auto request = QSharedPointer<Net::ModifyConfigWareRequest>::create();

        request->SetCode(item->code);
        request->SetName(item->name);
        request->SetRetailPrice(item->retailPrice);
        request->SetWholeSalePrice(item->wholesalePrice);
        if(StorageOperate::WARE_UPDATE == operate)
        {
            request->SetOperate(2);
        }
        else
        {
            request->SetOperate(1);
            request->SetStock(item->stock);
        }
        auto urlParam = Base::GetService<Core::UrlManager>()->GetUrl(Core::UrlManager::MODIFY_CONFIG_WARE);
        request->SetUrl(urlParam.url);
        request->SetTimeout(urlParam.timeout);
        QString message;

        Net::HttpModule::Get()->ProcessRequest(request, response, true);
        if(response->IsOk())
        {
            LOG_INFO(QStringLiteral("---------商品入库成功-----------"));
            emit signalConfigWareStorageSuccess();
        }
        else
        {
            LOG_INFO(QStringLiteral("---------商品入库失败-----------"));
            message = response->GetErrorMessage();
            emit signalConfigWareStorageError(message);
        }
    }

    bool BusinessManager::FinishOrder(QString& message)
    {
        bool ret = true;
        if(!m_saleOrder)
        {
            return false;
        }


        // 更新订单状态
        m_saleOrder->status = Order::OrderStatus::FINISH;

        // 清空缓存
        auto order = m_saleOrder;
        m_saleOrder.clear();
        m_codeSaleLookup.clear();
        m_codePresentationLookup.clear();

        auto dbm = Base::GetService<Core::DatabaseManager>();

        QString sqlError;

        // 切换正式库
        m_orderDml.SetDatabase(dbm->GetBusinessDatabase(order->time.date()));

        // 克隆订单和商品到正式库，入库数据需倒序
        QList<WareItemPtr> saveItems;
        for(int i = order->items.size() - 1; i >= 0; --i)
        {
            saveItems << order->items[i];
        }


        if(!m_orderDml.AddOrder(order, sqlError) || !m_orderDml.AddItemList(saveItems, sqlError))
        {
            message = QStringLiteral("sql error: %1").arg(sqlError);
            LOG_ERROR(sqlError);
            ret = false;
        }

        // 切换到临时库
        m_orderDml.SetDatabase(dbm->GetTempDatabase());

        // 删除临时库订单和商品
        if(!m_orderDml.DeleteOrder(order->uuid, sqlError) || !m_orderDml.DeleteItemByOrderUuid(order->uuid, sqlError))
        {
            message += QStringLiteral("sql error: %1").arg(sqlError);
            LOG_ERROR(sqlError);
            ret = false;
        }

        // 滚动流水号
        auto sg = Base::GetService<Core::SequenceGenerator>();
        sg->Next(Core::SequenceGenerator::SALE);
        return ret;
    }

    void BusinessManager::QueryOrderReport(const QDateTime& date)
    {
        QMetaObject::invokeMethod(this, "onQueryOrderReport", Q_ARG(QDateTime, date));
    }

    void BusinessManager::QueryOrderStockList()
    {
        QMetaObject::invokeMethod(this, "onQueryOrderStockList");
    }

    void BusinessManager::ModifyStockOrder(Core::StockOrderDetailPtr stockOrder, StockOrderOperate operateType)
    {
        QMetaObject::invokeMethod(this, "onModifyStockOrder", Q_ARG(Core::StockOrderDetailPtr, stockOrder), Q_ARG(StockOrderOperate, operateType));
    }

    void BusinessManager::onQueryOrderReport(const QDateTime& date)
    {
        auto response = QSharedPointer<Net::QueryOrderReportResponse>::create();
        auto request = QSharedPointer<Net::QueryOrderReportRequest>::create();
        request->SetQueryDate(date.toMSecsSinceEpoch());
        auto urlParam = Base::GetService<Core::UrlManager>()->GetUrl(Core::UrlManager::QUERY_ORDER_REPORT);
        request->SetUrl(urlParam.url);
        request->SetTimeout(urlParam.timeout);
        QString message;

        Net::HttpModule::Get()->ProcessRequest(request, response, true);
        if(response->IsOk())
        {
            LOG_INFO(QStringLiteral("---------订单报表查询成功-----------"));
            Core::OrderReportPtrList list;
            for(const auto& item : response->GetResult().orderReports)
            {
                Core::OrderReportPtr orderReport = Core::OrderReportPtr::create();
                orderReport->date = item.date;
                orderReport->amount = item.totalAmount;
                list.append(orderReport);
            }
            emit signalQueryOrderReportSuccess(list);
        }
        else
        {
            LOG_INFO(QStringLiteral("---------订单报表查询失败-----------"));
            message = response->GetErrorMessage();
            emit signalQueryOrderReportError(message);
        }
    }

    void BusinessManager::onQueryOrderStockList()
    {
        auto request = QSharedPointer<Net::StockAppRequest>::create();
        auto response = QSharedPointer<Net::QueryOrderStockListResponse>::create();
        auto urlParam = Base::GetService<Core::UrlManager>()->GetUrl(Core::UrlManager::QUERY_ORDER_STOCK_LIST);
        request->SetUrl(urlParam.url);
        request->SetTimeout(urlParam.timeout);
        QString message;

        Net::HttpModule::Get()->ProcessRequest(request, response, true);
        if(response->IsOk())
        {
            LOG_INFO(QStringLiteral("---------备货清单查询成功-----------"));
            Core::StockOrderDetailPtrList stockOrders;
            for(const auto& item : response->GetResult().stockingOrders)
            {
                Core::StockOrderDetailPtr stockOrderDetail = Core::StockOrderDetailPtr::create();
                stockOrderDetail->orderNo = item.orderNo;
                stockOrderDetail->orderUuid = item.orderUuid;
                stockOrderDetail->time = item.time;
                stockOrderDetail->orderAmount = item.orderAmount;
                stockOrderDetail->totalPromotionAmount = item.totalPromotionAmount;
                stockOrderDetail->deliveryFreeAmount = item.deliveryFreeAmount;
                stockOrderDetail->extension = item.extension;
                for(const auto& wareItem : item.wares)
                {
                    Core::StockOrderDetail::StockOrderWare storckOrderWare;
                    storckOrderWare.amount = wareItem.amount;
                    storckOrderWare.code = wareItem.code;
                    storckOrderWare.name = wareItem.name;
                    storckOrderWare.count = wareItem.count;
                    storckOrderWare.discountPrice = wareItem.discountPrice;
                    storckOrderWare.gifts = wareItem.gifts;
                    storckOrderWare.price = wareItem.price;
                    storckOrderWare.promotionAmount = wareItem.promotionAmount;
                    storckOrderWare.extension = wareItem.extension;
                    stockOrderDetail->wares.append(storckOrderWare);
                }
                stockOrders.append(stockOrderDetail);
            }
            emit signalQueryOrderStockListSuccess(stockOrders);
        }
        else
        {
            LOG_INFO(QStringLiteral("---------备货清单查询失败-----------"));
            message = response->GetErrorMessage();
            emit signalQueryOrderStockListError(message);
        }
    }

    void BusinessManager::onModifyStockOrder(Core::StockOrderDetailPtr stockOrder, StockOrderOperate operateType)
    {
        Q_ASSERT(stockOrder != nullptr);
        auto response = QSharedPointer<Net::StockAppResponse>::create();
        auto request = QSharedPointer<Net::ModifyStockOrderRequest>::create();

        request->SetOrderNo(stockOrder->orderNo);
        request->SetOrderUuid(stockOrder->orderUuid);
        request->SetOperate(operateType);
        request->SetStockOrderDetail(stockOrder->ToPushJsonObj());
        auto urlParam = Base::GetService<Core::UrlManager>()->GetUrl(Core::UrlManager::MODIFY_STOCK_ORDER);
        request->SetUrl(urlParam.url);
        request->SetTimeout(urlParam.timeout);
        QString message;

        Net::HttpModule::Get()->ProcessRequest(request, response, true);
        if(response->IsOk())
        {
            LOG_INFO(QStringLiteral("---------订单出库成功-----------"));
            emit signalModifyStockOrderSuccess();
        }
        else
        {
            LOG_INFO(QStringLiteral("---------订单出库失败-----------"));
            message = response->GetErrorMessage();
            emit signalModifyStockOrderError(message, operateType);
        }
    }
}

