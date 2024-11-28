#include "TerminalGetConfigwaresResponse.h"

namespace Net
{
    TerminalGetConfigwaresResponse::TerminalGetConfigwaresResponse(QObject *parent)
        : StockAppResponse(parent)
    {
    }

    TerminalGetConfigwaresResponse::~TerminalGetConfigwaresResponse()
    {}

    void TerminalGetConfigwaresResponse::ParseJsonParam(const QJsonObject& val)
    {
        QJsonArray configWares = val["configWares"].toArray();
        for(const auto& item : configWares)
        {
            QJsonObject obj = item.toObject();
            ConfigWare ware;
            ASSIGN_TO_STRING(obj, "name", ware.name);
            ASSIGN_TO_STRING(obj, "code", ware.code);
            ASSIGN_TO_STRING(obj, "imageUrl", ware.imageUrl);
            ASSIGN_TO_STRING(obj, "imageMd5", ware.imageMd5);
            ASSIGN_TO_INT64(obj, "retailPrice", ware.retailPrice);
            ASSIGN_TO_INT64(obj, "wholesalePrice", ware.wholesalePrice);
            ASSIGN_TO_INT64(obj, "stock", ware.stock);
            ASSIGN_TO_OBJECT(obj, "extension", ware.extension);
            m_result.wares.append(ware);
        }
    }

    const TerminalGetConfigwaresResponse::Result& TerminalGetConfigwaresResponse::GetResult() const
    {
        return m_result;
    }

}
