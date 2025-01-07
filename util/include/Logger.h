#ifndef LOGGER_H
#define LOGGER_H

#include<mutex>
#include<string>
#include<stdarg.h>
#include<iostream>
#include <thread>
#include <inttypes.h>
#include <dirent.h>
#include <fstream>
#include <memory>

#define LOG_CR      1
#define LOG_ER      2
#define LOG_IN      4
#define LOG_VB      8
#define LOG_DB      16
#define LOG_CON     32
#define LOG_USER    64
#define LOG_ALL     127

using namespace std;

#ifdef _WIN32

#include <Windows.h>
#define gettid GetCurrentThreadId
#define _FILE_ string(__FILE__).substr(string(__FILE__).find_last_of("\\") + 1 )

#elif defined __unix__
#include <sys/syscall.h>
#include <sys/stat.h>
#include <unistd.h>
#define gettid() syscall(SYS_gettid)
#define _FILE_ string(__FILE__).substr(string(__FILE__).find_last_of("//") + 1 )
#endif // _WIN32

#define LOG_CRITICAL(Logger,logdata) { if(Logger.getLoglevel() & LOG_CR){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"CR",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_ERROR(Logger,logdata) { if(Logger.getLoglevel() & LOG_ER){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"ER",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_INFO(Logger,logdata) { if(Logger.getLoglevel() & LOG_IN){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"IN",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_VERBOSE(Logger,logdata) { if(Logger.getLoglevel() & LOG_VB){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"VB",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_DEBUG(Logger,logdata) { if(Logger.getLoglevel() & LOG_DB){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"DB",this,gettid(),Logger.writeLogData logdata);}}

#define LOG_CRITICALNP(Logger,logdata) { if(Logger.getLoglevel() & LOG_CR){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"CR",nullptr,gettid(),Logger.writeLogData logdata);}}
#define LOG_ERRORNP(Logger,logdata) { if(Logger.getLoglevel() & LOG_ER){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"ER",nullptr,gettid(),Logger.writeLogData logdata);}}
#define LOG_INFONP(Logger,logdata) { if(Logger.getLoglevel() & LOG_IN){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"IN",nullptr,gettid(),Logger.writeLogData logdata);}}
#define LOG_VERBOSENP(Logger,logdata) { if(Logger.getLoglevel() & LOG_VB){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"VB",nullptr,gettid(),Logger.writeLogData logdata);}}
#define LOG_DEBUGNP(Logger,logdata) { if(Logger.getLoglevel() & LOG_DB){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"DB",nullptr,gettid(),Logger.writeLogData logdata);}}

#define LOG_CRITICAL_IF(condition,Logger,logdata){ if((Logger.getLoglevel() & LOG_CR) && condition){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"CR",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_ERROR_IF(condition,Logger,logdata) { if((Logger.getLoglevel() & LOG_ER) && condition){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"ER",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_INFO_IF(condition,Logger,logdata) { if((Logger.getLoglevel() & LOG_IN) && condition){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"IN",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_VERBOSE_IF(condition,Logger,logdata) { if((Logger.getLoglevel() & LOG_VB) &&condition){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"VB",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_DEBUG_IF(condition,Logger,logdata) { if((Logger.getLoglevel() & LOG_DB)&&condition){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"DB",this,gettid(),Logger.writeLogData logdata);}}


#define LOG_CRITICALNP_IF(condition,Logger,logdata){ if((Logger.getLoglevel() & LOG_CR) && condition){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"CR",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_ERRORNP_IF(condition,Logger,logdata) { if((Logger.getLoglevel() & LOG_ER) && condition){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"ER",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_INFONP_IF(condition,Logger,logdata) { if((Logger.getLoglevel() & LOG_IN) && condition){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"IN",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_VERBOSENP_IF(condition,Logger,logdata) { if((Logger.getLoglevel() & LOG_VB) &&condition){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"VB",this,gettid(),Logger.writeLogData logdata);}}
#define LOG_DEBUGNP_IF(condition,Logger,logdata) { if((Logger.getLoglevel() & LOG_DB)&&condition){lock_guard<mutex> lck(Logger.getMutex());Logger.writeLogHeader(_FILE_,__LINE__,"DB",this,gettid(),Logger.writeLogData logdata);}}


#define MAX_LOG_SIZE 1024*1024

class Logger
{
    public:
        Logger(string Path,string fileName);
        Logger(string Path,string fileName,unsigned int logLevel);
        Logger(string Path,string fileName,unsigned int logLevel,unsigned int logFileSize);
        Logger(string Path,string fileName,unsigned int logLevel,unsigned int logFileSize,unsigned int logFilebackupCount);
        Logger();
        virtual ~Logger();

        inline void setLoglevel(unsigned int);
        inline unsigned int getLoglevel();
        inline void setLogFileSize(unsigned int);
        inline void setLogBackUpCount(unsigned int);
        inline mutex& getMutex();
        inline bool setLogFile(string Path,string filename);

        void writeLogHeader(string,unsigned int,string,void*,uint64_t,shared_ptr<char>);
        shared_ptr<char> writeLogData(const char *p_log,...);
        bool createDirectory(const string);

    protected:

    private:

        ofstream m_log;
        mutex m_logmutex;
        unsigned int m_logLevel;
        unsigned int m_logFileSize;
        unsigned int m_logBackupCount;

        string m_logFile;

        void backUpAndLoadNewFile();
        void spiltLogFile();
};

void Logger::setLoglevel(unsigned int logLevel)
{
    this->m_logLevel = logLevel;
}
unsigned int Logger::getLoglevel()
{
    return this->m_logLevel;
}

mutex& Logger::getMutex()
{
    return this->m_logmutex;
}

void Logger::setLogFileSize(unsigned int fileSize)
{
    this->m_logFileSize = fileSize * 1024 * 1024;
}

void Logger::setLogBackUpCount(unsigned int backUpCount)
{
    this->m_logBackupCount = backUpCount;
}
bool Logger::setLogFile(string Path,string Filename)
{
    #ifdef _WIN32
    this->m_logFile = Path + "\\"  + Filename;
    #elif defined __unix__
    this->m_logFile = Path + "/"  + Filename;
    #endif
    this->createDirectory(Path);
    return true;
}
#endif // LOGGER_H
