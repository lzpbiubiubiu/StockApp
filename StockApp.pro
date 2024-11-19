QT += quick core gui gui-private network xml svg sql concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
CONFIG += debug
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Application.cpp \
        Base/Common/IService.cpp \
        Base/Common/ServiceBase.cpp \
        Base/Common/ServiceManager.cpp \
        Base/Database/Base/DBBase.cpp \
        Base/Database/Base/EntityBase.cpp \
        Base/Database/Orm/QdOrmBuilder.cpp \
        Base/Database/Orm/QdOrmClass.cpp \
        Base/Database/Orm/QdOrmDml.cpp \
        Base/Database/Orm/QdOrmFactory.cpp \
        Base/Database/Orm/QdOrmMember.cpp \
        Base/Database/Orm/QdOrmWhere.cpp \
        Base/Http/HttpModule.cpp \
        Base/Http/HttpRequest.cpp \
        Base/Http/HttpResponse.cpp \
        Base/Http/HttpTask.cpp \
        Base/Http/HttpTaskExecutor.cpp \
        Base/Log/AsyncLogger.cpp \
        Base/Log/Log.cpp \
        Base/Util/JsonUtil.cpp \
        Base/Util/MathUtil.cpp \
        Base/Util/PathUtil.cpp \
        Base/Util/PriceUtil.cpp \
        Core/BusinessManager.cpp \
        Core/ConvertUtil.cpp \
        Core/DatabaseManager.cpp \
        Core/GarbageManager.cpp \
        Core/GlobalData.cpp \
        Core/KeyEmitter.cpp \
        Core/KeyEventFilter.cpp \
        Core/SequenceGenerator.cpp \
        Core/UrlManager.cpp \
        Database/BusinessDB.cpp \
        Database/StockDB.cpp \
        Dml/OrderDml.cpp \
        Entity/KvEntity.cpp \
        Entity/OrderEntity.cpp \
        Entity/SequenceEntity.cpp \
        Entity/VersionEntity.cpp \
        Entity/WareItemEntity.cpp \
        Protocol/Hytrade/HytradeInfoRequest.cpp \
        Protocol/Hytrade/HytradeInfoResponse.cpp \
        Protocol/Hytrade/ModifyStockOrderRequest.cpp \
        Protocol/Query/QueryOrderReportRequest.cpp \
        Protocol/Query/QueryOrderReportResponse.cpp \
        Protocol/Query/QueryOrderStockListResponse.cpp \
        Protocol/StockAppResponse.cpp \
        Protocol/Storage/ModifyConfigWareRequest.cpp \
        Protocol/Terminal/StockAppHeartbeatRequest.cpp \
        Protocol/StockAppRequest.cpp \
        Protocol/Terminal/TerminalGetConfigwaresResponse.cpp \
        Protocol/Terminal/TerminalUserLoginRequest.cpp \
        Protocol/Terminal/TerminalUserLoginResponse.cpp \
        Ui/Component/OrderStockListPanel.cpp \
        Ui/Component/SaleWarePanel.cpp \
        Ui/Component/StackWarePanel.cpp \
        Ui/Page/Controller/LoginPageController.cpp \
        Ui/Page/Controller/MainPageController.cpp \
        Ui/Page/Controller/ManagePageController.cpp \
        Ui/Page/Controller/PageController.cpp \
        Ui/Page/Controller/ReportPageController.cpp \
        Ui/Page/Controller/SalePageController.cpp \
        Ui/Page/LoginPage.cpp \
        Ui/Page/MainPage.cpp \
        Ui/Page/ManagePage.cpp \
        Ui/Page/Page.cpp \
        Ui/Page/ReportPage.cpp \
        Ui/Page/SalePage.cpp \
        main.cpp \
        Ui/MainWindow.cpp

RESOURCES += qml.qrc

INCLUDEPATH += "External\jsoncpp\include"

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Application.h \
    Base/Common/IService.h \
    Base/Common/ServiceBase.h \
    Base/Common/ServiceManager.h \
    Base/Common/Singleton.h \
    Base/Database/Base/DBBase.h \
    Base/Database/Base/DmlBase.h \
    Base/Database/Base/EntityBase.h \
    Base/Database/Base/Qd.h \
    Base/Database/Inl/QdOrmDaoDelete.inl \
    Base/Database/Inl/QdOrmDaoFetch.inl \
    Base/Database/Inl/QdOrmDaoFetchAll.inl \
    Base/Database/Inl/QdOrmDaoInsert.inl \
    Base/Database/Inl/QdOrmDaoSave.inl \
    Base/Database/Inl/QdOrmDaoTable.inl \
    Base/Database/Inl/QdOrmDaoUpdate.inl \
    Base/Database/Inl/QdOrmDaoUpdateAll.inl \
    Base/Database/Orm/QdOrmBuilder.h \
    Base/Database/Orm/QdOrmClass.h \
    Base/Database/Orm/QdOrmCondition.h \
    Base/Database/Orm/QdOrmDao.h \
    Base/Database/Orm/QdOrmDefine.h \
    Base/Database/Orm/QdOrmDml.h \
    Base/Database/Orm/QdOrmFactory.h \
    Base/Database/Orm/QdOrmMember.h \
    Base/Database/Orm/QdOrmWhere.h \
    Base/Http/HttpConsts.h \
    Base/Http/HttpModule.h \
    Base/Http/HttpRequest.h \
    Base/Http/HttpResponse.h \
    Base/Http/HttpTask.h \
    Base/Http/HttpTaskExecutor.h \
    Base/Log/AsyncLogger.h \
    Base/Log/Log.h \
    Base/Util/JsonUtil.h \
    Base/Util/MathUtil.h \
    Base/Util/PathUtil.h \
    Base/Util/PriceUtil.h \
    Core/BusinessManager.h \
    Core/Config.h \
    Core/ConvertUtil.h \
    Core/DatabaseManager.h \
    Core/GarbageManager.h \
    Core/GlobalData.h \
    Core/KeyEmitter.h \
    Core/KeyEventFilter.h \
    Core/SequenceGenerator.h \
    Core/StackStruct.h \
    Core/UrlManager.h \
    Database/BusinessDB.h \
    Database/StockDB.h \
    Dml/OrderDml.h \
    Entity/KvEntity.h \
    Entity/OrderEntity.h \
    Entity/SequenceEntity.h \
    Entity/VersionEntity.h \
    Entity/WareItemEntity.h \
    Protocol/Hytrade/HytradeInfoRequest.h \
    Protocol/Hytrade/HytradeInfoResponse.h \
    Protocol/Hytrade/ModifyStockOrderRequest.h \
    Protocol/Query/QueryOrderReportRequest.h \
    Protocol/Query/QueryOrderReportResponse.h \
    Protocol/Query/QueryOrderStockListResponse.h \
    Protocol/StockAppResponse.h \
    Protocol/Storage/ModifyConfigWareRequest.h \
    Protocol/Terminal/StockAppHeartbeatRequest.h \
    Protocol/StockAppRequest.h \
    Protocol/Terminal/TerminalGetConfigwaresResponse.h \
    Protocol/Terminal/TerminalUserLoginRequest.h \
    Protocol/Terminal/TerminalUserLoginResponse.h \
    Ui/Component/OrderStockListPanel.h \
    Ui/Component/SaleWarePanel.h \
    Ui/Component/StackWarePanel.h \
    Ui/MainWindow.h \
    Ui/Page/Controller/LoginPageController.h \
    Ui/Page/Controller/MainPageController.h \
    Ui/Page/Controller/ManagePageController.h \
    Ui/Page/Controller/PageController.h \
    Ui/Page/Controller/ReportPageController.h \
    Ui/Page/Controller/SalePageController.h \
    Ui/Page/LoginPage.h \
    Ui/Page/MainPage.h \
    Ui/Page/ManagePage.h \
    Ui/Page/Page.h \
    Ui/Page/ReportPage.h \
    Ui/Page/SalePage.h

android {
    QT += core-private
    #LIBS += C:/Qt/Qt5.12.12/5.12.12/android_arm64_v8a/plugins/imageformats/*.so
#    MY_IMAGEFORMATS_FOLDER = C:/Qt/Qt5.12.12/5.12.12/android_arm64_v8a/plugins/imageformats
#    EXTRA_FILES += $$shell_quote($$MY_IMAGEFORMATS_FOLDER/*) $$escape_expand(\\n\\t)
    #ANDROID_PACKAGE_SOURCE_DIR += $$OUT_PWD/android-build/assets
    #DEPLOYMENT += androiddeployqt
}
android: include(C:/Qt/Qt5.12.12/AndroidSDK/android_openssl-master/openssl.pri)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android_sources

DISTFILES += \
    android_sources/AndroidManifest.xml \
    android_sources/build.gradle \
    android_sources/gradle/wrapper/gradle-wrapper.jar \
    android_sources/gradle/wrapper/gradle-wrapper.properties \
    android_sources/gradlew \
    android_sources/gradlew.bat \
    android_sources/res/values/libs.xml
