#pragma once

namespace Base
{
    /** 抽象服务接口 */
    class IService
    {
    public:
        /** 服务状态 */
        enum State
        {
            /** 运行中 */
            RUNNING = 0,

            /** 已停止 */
            STOPPED
        };

        IService();
        virtual ~IService();

        /** 获取服务状态 */
        virtual State GetState() = 0;

    protected:
        /** 服务启动前回调 */
        virtual bool OnStart() = 0;

        /** 服务停止后回调 */
        virtual void OnStop() = 0;
    };
}
