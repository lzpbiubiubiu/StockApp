//
// Copyright(c) 2016 Alexander Dalshov.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#ifndef SPDLOG_H
#include "spdlog/spdlog.h"
#endif

#if defined(_WIN32)

#include "spdlog/details/null_mutex.h"
#include "spdlog/sinks/base_sink.h"

#include <winbase.h>

#include <mutex>
#include <string>

namespace spdlog {
namespace sinks {

/*
 * MSVC sink (logging using OutputDebugStringA)
 */
template<typename Mutex>
class msvc_sink : public base_sink<Mutex>
{
public:
    explicit msvc_sink() {}

protected:
    void sink_it_(const details::log_msg &msg) override
    {

        fmt::memory_buffer formatted;
        sink::formatter_->format(msg, formatted);

#ifdef ENABLE_MSVC_UTF8
        OutputDebugStringA(convert_byte<CP_UTF8,CP_ACP>(fmt::to_string(formatted)).c_str());
#else
        OutputDebugStringA(fmt::to_string(formatted).c_str());
#endif
    }

    void flush_() override {}
};

using msvc_sink_mt = msvc_sink<std::mutex>;
using msvc_sink_st = msvc_sink<details::null_mutex>;

using windebug_sink_mt = msvc_sink_mt;
using windebug_sink_st = msvc_sink_st;

} // namespace sinks
} // namespace spdlog

#endif
