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
 * BackupTapeTask.h
 *
 *  Created on: Aug 29, 2012
 *      Author: More Zeng
 */

#pragma once

/* Standard library headers */
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sys/types.h>

/* Boost headers */
#include <boost/thread/mutex.hpp>
#include <boost/thread/group.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/timer/timer.hpp>   // non‑deprecated timer header

/* Project headers */
#include "MetaManager.h"
#include "FileMetaParser.h"
#include "../ltfs_management/CatalogDbManager.h"

using namespace ltfs_management;

namespace bdt
{
    struct TapeBackupItem
    {
        BackupItem 	item;
        bool		running;
        bool        isMultiple;
        std::string      manifest;
        int         totalMultiple;
        int         numberMultiple;
    };

    struct PendingTapes
    {
        std::vector<std::string> bkTapes;
        off_t		maxSize;
    };

    struct MultipleFile
    {
        boost::posix_time::ptime time;
        int total;
        std::vector<long long> numbers;
        std::vector<bool> exists;

        MultipleFile()
        : total(0)
        {
            time = boost::posix_time::second_clock::local_time();
        }

        void
        Resize(int total_)
        {
            if ( total == total_ ) {
                return;
            }

            total = total_;
            numbers.resize(total);
            exists.resize(total);
            for (int i=0; i<total; ++i) {
                numbers[i] = -1;
                exists[i] = false;
            }
        }

        bool
        IsFull()
        {
            for (int i=0; i<total; ++i) {
                if (numbers[i] < 0) {
                    return false;
                }
            }
            return true;
        }
    };

    class BackupTapeTask : public BackendTask
    {
    public:
        BackupTapeTask();

        virtual
        ~BackupTapeTask();

        virtual void
        Start();

    private:
        void StartBackupSub(const std::vector<std::string>& bkTapes, off_t maxSize);
        void HandleBackupSub(const std::vector<std::string>& bkTapes, off_t maxSize);
        bool Backup(const std::vector<BackupItem> &items, const std::vector<std::string>& tapes);
        void SetBackupTapes(const std::vector<std::string>& bkTapes, bool bRunning);
        int GetRunningNum();
        bool IsTapesRunning(const std::vector<std::string>& tapes);
        bool GetFileItemToBackup(std::vector<BackupItem>& fileItems, off_t& maxSize, off_t maxFileSize = 100*1024*1024, unsigned long maxFileNum = 1000);
        void InsertTapeBackupItem(const BackupItem & item);
        void DeleteTapeBackupItem(const BackupItem & item);

        std::auto_ptr<boost::thread_group> threads_;
        TapeManagerInterface * tape_;
        CacheManager * cache_;
        ScheduleInterface * schedule_;
        MetaManager * meta_;
        std::map<std::string, bool>	tapes_;
        boost::mutex 		tapesMutex_;
        std::map<unsigned long long, TapeBackupItem>	files_;
        std::map<std::string, MultipleFile> multipleFiles_;
        boost::mutex 		filesMutex_;
        std::string				uuid_;
        std::auto_ptr<CatalogDbManager> catalogDb_;
        FileMetaParser      parser_;
    };

}
