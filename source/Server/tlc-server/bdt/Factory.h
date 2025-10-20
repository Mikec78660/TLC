/* Copyright (c) 2012 BDT Media Automation GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Factory.h
 *
 *  Created on: Apr 11, 2012
 *      Author: More Zeng
 */

#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/mutex.hpp>

#ifdef MORE_TEST

namespace bdt
{

    void
    CreateLogger(const fs::path & folderLogger);

    std::ofstream &
    LoggerDebug();

    std::ofstream &
    LoggerInfo();

    std::ofstream &
    LoggerWarn();

    std::ofstream &
    LoggerError();

    static boost::mutex mutexLogger;

#define LogIdent                                            \
        ( std::string(__PRETTY_FUNCTION__)                   \
            + ":" + boost::lexical_cast<std::string>(__LINE__) \
            + ":" + boost::lexical_cast<std::string>(errno) )

#define LogTime                                             \
        boost::posix_time::to_simple_string(                \
            boost::posix_time::microsec_clock::local_time() )

#else

#include "../log/loggerManager.h"

using namespace ltfs_logger;

namespace bdt
{

    std::string
    GetName();

#endif


namespace bdt
{

    class CacheManager;
    class ReadManager;
    class MetaManager;
    class TapeManagerInterface;
    class ScheduleInterface;
    class ReadManager;
    class Throttle;
    class Configure;

    class Factory
    {
    public:
        Factory();

        virtual
        ~Factory();

        static void
        StartBackendTasks();

        static void
        StopBackendTasks();

        static void
        SetService(const std::string & service)
        {
            service_ = service;
        }

        static std::string
        GetService()
        {
            return service_;
        }

        static void
        SetName(const std::string & name)
        {
            name_ = name;
        }

        static std::string
        GetName()
        {
            return name_;
        }

        static void
        SetUuid(const std::string & uuid)
        {
            uuid_ = uuid;
        }

        static std::string
        GetUuid()
        {
            return uuid_;
        }

        static void
        SetMetaFolder(const fs::path & folder)
        {
            folderMeta_ = folder;
        }

        static fs::path
        GetMetaFolder()
        {
            return folderMeta_;
        }

        static void
        SetCacheFolder(const fs::path & folder)
        {
            folderCache_ = folder;
        }

        static fs::path
        GetCacheFolder()
        {
            return folderCache_;
        }

        static void
        SetTapeFolder(const fs::path & folder)
        {
            folderTape_ = folder;
        }

        static fs::path
        GetTapeFolder()
        {
            return folderTape_;
        }

        static void
        CreateThrottle(int interval, long long valve);

        static void
        ReleaseThrottle();

        static Throttle *
        GetThrottle()
        {
            return throttle_.get();
        }

        static void
        CreateConfigure();

        static void
        ReleaseConfigure();

        static Configure *
        GetConfigure()
        {
            return configure_.get();
        }

        static void
        CreateMetaManager();

        static void
        ReleaseMetaManager();

        static MetaManager *
        GetMetaManager()
        {
            return meta_.get();
        }

        static void
        CreateTapeManager();

        static void
        ReleaseTapeManager();

        static void
        ResetTapeManager(TapeManagerInterface * tape)
        {
            tape_.reset(tape);
        }

        static TapeManagerInterface *
        GetTapeManager()
        {
            return tape_.get();
        }

        static void
        CreateCacheManager();

        static void
        ReleaseCacheManager();

        static CacheManager *
        GetCacheManager()
        {
            return cache_.get();
        }

        static void
        CreateReadManager();

        static void
        ReleaseReadManager();

        static ReadManager *
        GetReadManager()
        {
            return read_.get();
        }

        static void
        CreateSchedule();

        static void
        ReleaseSchedule();

        static void
        ResetSchedule(ScheduleInterface * schedule)
        {
            schedule_.reset(schedule);
        }

        static ScheduleInterface *
        GetSchedule()
        {
            return schedule_.get();
        }

        static void
        CreateTapeLibraryManager();

        static void
        ReleaseTapeLibraryManager();

        static void
        ResetTapeLibraryManager(tape::TapeLibraryManager * changer)
        {
            changer_.reset(changer);
        }

        static tape::TapeLibraryManager *
        GetTapeLibraryManager()
        {
            return changer_.get();
        }

        static int
        SocketServerHandle(const std::string & service);

        static int
        SocketClientHandle(const std::string & service);

        static bool
        GetRelativePathFromCachePath(const fs::path & pathCache, fs::path & path);

        static bool
        GetRelativePathFromMetaPath(const fs::path & pathMeta, fs::path & path);

        static bool
        GetMetaPathFromCachePath(const fs::path & pathCache, fs::path & path);

        static bool
        GetCachePathFromMetaPath(const fs::path & pathMeta, fs::path & path);

    private:
        static std::string service_;
        static std::string name_;
        static std::string uuid_;
        static fs::path folderMeta_;
        static fs::path folderCache_;
        static fs::path folderTape_;
        static std::unique_ptr<Configure> configure_;
        static std::unique_ptr<Throttle> throttle_;
        static std::unique_ptr<MetaManager> meta_;
        static std::unique_ptr<TapeManagerInterface> tape_;
        static std::unique_ptr<CacheManager> cache_;
        static std::unique_ptr<ReadManager> read_;
        static std::unique_ptr<ScheduleInterface> schedule_;
        static std::unique_ptr<tape::TapeLibraryManager> changer_;
        static std::vector<BackendTask *> tasks_;
        static std::unique_ptr<boost::thread_group> taskGroup_;
        static std::ofstream logDebug_;
        static std::ofstream logInfo_;
        static std::ofstream logWarn_;
        static std::ofstream logError_;
        static boost::mutex logMutex_;
        static const std::string pathSocket_;
    };

inline std::string
GetName()
{
    return Factory::GetName();
}

}
