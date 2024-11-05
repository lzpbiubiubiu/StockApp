#include "OrderEntity.h"

ORM_REGISTER_CPP_STOCK_DATABASE(OrderEntity)

namespace Orm
{
    template<>
    void register_class(QdOrmClassDao<OrderEntity>& t)
    {
        ORM_REGISTER_QD_TABLE(OrderEntity, ORDER_TABLE_VERSION_V1)
        OrmMetaMemberPtr member = nullptr;

        // ORDER_TABLE_VERSION_V1
        member = ORM_REGISTER_QD_ID(OrderEntity, Uuid)
        member = ORM_REGISTER_QD_DATA(OrderEntity, ReceiptNo, ORDER_TABLE_VERSION_V1)
        member->setIsIndex(true);
        member = ORM_REGISTER_QD_DATA(OrderEntity, OrderAmount, ORDER_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(OrderEntity, PromotionAmount, ORDER_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(OrderEntity, WholeOrderPromoAmount, ORDER_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(OrderEntity, Type, ORDER_TABLE_VERSION_V1)
        member->setIsIndex(true);
        member = ORM_REGISTER_QD_DATA(OrderEntity, Status, ORDER_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(OrderEntity, Extension, ORDER_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(OrderEntity, Time, ORDER_TABLE_VERSION_V1)
    }
}
