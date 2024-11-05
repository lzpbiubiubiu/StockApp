#include "WareItemEntity.h"

ORM_REGISTER_CPP_STOCK_DATABASE(WareItemEntity)

namespace Orm
{
    template<>
    void register_class(QdOrmClassDao<WareItemEntity>& t)
    {
        ORM_REGISTER_QD_TABLE(WareItemEntity, WARE_ITEM_TABLE_VERSION_V1)
        OrmMetaMemberPtr member = nullptr;

        // WARE_ITEM_TABLE_VERSION_V1
        member = ORM_REGISTER_QD_ID(WareItemEntity, Uuid)
        member = ORM_REGISTER_QD_DATA(WareItemEntity, OrderUuid, WARE_ITEM_TABLE_VERSION_V1)
        member->setIsIndex(true);
        member = ORM_REGISTER_QD_DATA(WareItemEntity, Code, WARE_ITEM_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(WareItemEntity, Name, WARE_ITEM_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(WareItemEntity, Count, WARE_ITEM_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(WareItemEntity, Stock, WARE_ITEM_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(WareItemEntity, Amount, WARE_ITEM_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(WareItemEntity, Price, WARE_ITEM_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(WareItemEntity, DiscountPrice, WARE_ITEM_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(WareItemEntity, PromotionAmount, WARE_ITEM_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(WareItemEntity, Extension, WARE_ITEM_TABLE_VERSION_V1)
    }
}
