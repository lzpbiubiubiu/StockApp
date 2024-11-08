#include "Controller/ManagePageController.h"
#include "ManagePage.h"

namespace UI
{
    IMPLEMENT_PAGE("managePage", ManagePage)

    ManagePage::ManagePage(QObject* parent/* = nullptr*/)
        : Page(parent)
    {
        SetContentItem("qrc:/Qml/Page/ManagePage.qml");
        SetController(new ManagePageController(this));
    }

    ManagePage::~ManagePage()
    {
    }
} // namespace UI
