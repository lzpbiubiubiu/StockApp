#include "Controller/StoragePageController.h"
#include "StoragePage.h"

namespace UI
{
    IMPLEMENT_PAGE("storagePage", StoragePage)

    StoragePage::StoragePage(QObject* parent/* = nullptr*/)
        : Page(parent)
    {
        SetContentItem("qrc:/Qml/Page/StoragePage.qml");
        SetController(new StoragePageController(this));
    }

    StoragePage::~StoragePage()
    {
    }
} // namespace UI
