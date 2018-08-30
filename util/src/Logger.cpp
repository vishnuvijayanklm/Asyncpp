#include "Logger.h"
#include <string.h>
Logger::Logger()
{
	this->m_logLevel = 127;
	this->m_logFile.clear();
	this->setLogFileSize(10);
	this->setLogBackUpCount(2);
}

Logger::~Logger()
{

}

Logger::Logger(string Path,string fileName):Logger()
{
	this->setLogFile(Path,fileName);
}
Logger::Logger(string Path,string fileName,unsigned int logLevel):Logger(Path,fileName)
{
	this->setLoglevel(logLevel);
}

Logger::Logger(string Path,string fileName,unsigned int logLevel,unsigned int logFileSize):Logger(Path,fileName,logLevel)
{
	this->setLogFileSize(logFileSize);
}
Logger::Logger(string Path,string fileName,unsigned int logLevel,unsigned int logFileSize,unsigned int logFilebackupCount):Logger(Path,fileName,logLevel,logFileSize)
{
	this->setLogBackUpCount(logFilebackupCount);
}

void Logger::writeLogHeader(string fileName,unsigned int lineNo,string logType,void *p_Addr,uint64_t tid,char* msg)
{
	cout<<"==========="<<fileName.c_str()<<"================="<<lineNo<<"==============================="<<tid<<"==============================="<<msg<<endl;
	lock_guard<mutex> lock(this->m_logmutex);
	cout<<"After writeLogHeader log "<<tid<<endl;
	if(!this->m_log.is_open())
	{
		this->backUpAndLoadNewFile();
		if(!this->m_log.is_open())
		{
			cout<<msg<<endl;
			return;
		}
	}

	char temp[1024];
	temp[0] = '\0';
	time_t currentepochTime = time(0);
	tm *date = gmtime(&currentepochTime);
	timespec curTime;
	clock_gettime(CLOCK_REALTIME,&curTime);

	snprintf(temp,1024,"%04d-%02d-%02dT%02d:%02d:%02d.%06ld",date->tm_year+1900,date->tm_mon+1,date->tm_mday,date->tm_hour,date->tm_min,date->tm_sec,curTime.tv_nsec/100);
	this->m_log << fileName.c_str() << "|" << lineNo << "|" << temp <<"|[" << logType << "][" << p_Addr << "][" << tid << "]|"<<msg<<endl;
	this->m_log.flush();

	if(this->m_logLevel & LOG_CON)
	{
		cout<<msg<<endl;
	}
	cout<<"Spilt log>>>>"<<tid<<endl;
	this->spiltLogFile();
	cout<<"Spilt log<<<<"<<tid<<endl;
}

bool Logger::createDirectory(const string directoryName)
{
	DIR *dr = NULL;
	if((dr = opendir(directoryName.c_str())) == NULL)
	{
#ifdef _WIN32
		if(mkdir(directoryName.c_str()) == -1)
#elif defined __unix__
			if(mkdir(directoryName.c_str(),07) == -1)
#endif // defined
			{
				closedir(dr);
				return false;
			}
	}
	closedir(dr);
	return true;
}

char* Logger::writeLogData(const char *p_log, ...)
{
	cout<<"Before writeLogData lock "<<gettid()<<endl;
	lock_guard<mutex> lock(this->m_logmutex);
	cout<<"After writeLogData lock "<<gettid()<<endl;
	va_list args;
	va_start(args,p_log);
	int size = vsnprintf(this->mbuffer,MAX_LOG_SIZE-1,p_log,args);

	if(size)
	{

	}

	va_end(args);
	cout<<"Returning "<<endl;
	return this->mbuffer;
}

void Logger::spiltLogFile()
{
	if(!this->m_log.is_open())
	{
		return;
	}

	unsigned int size = this->m_log.tellp();

	if(size < this->m_logFileSize)
	{
		return;
	}

	this->m_log.close();

	this->backUpAndLoadNewFile();
}

void Logger::backUpAndLoadNewFile()
{
	char buffer[2048];
	unsigned int totalbackupfile = this->m_logBackupCount;
	if(this->m_logFile.empty())
	{
		return;
	}

	while(totalbackupfile > 1)
	{
#ifdef _WIN32
		MoveFile(std::string(this->m_logFile+"_backup_"+std::to_string(totalbackupfile - 1)).c_str(),std::string(this->m_logFile + "_backup_"+std::to_string(totalbackupfile)).c_str());
#elif defined __unix__
		snprintf(buffer,2048,"mv %s_backup_%d %s_backup_%d 2>/dev/null",this->m_logFile.c_str(),totalbackupfile-1,this->m_logFile.c_str(),totalbackupfile);
		system(buffer);
#endif
		totalbackupfile--;
	}

#ifdef _WIN32
	MoveFile(this->m_logFile.c_str(),std::string(this->m_logFile + "_backup_1").c_str());
#elif defined __unix__
	snprintf(buffer,1024,"mv %s %s_backup_1 2>/dev/null",this->m_logFile.c_str(),this->m_logFile.c_str());
	system(buffer);
#endif

	if(this->m_log.is_open())
	{
		this->m_log.close();
	}

	this->m_log.open(this->m_logFile.c_str());
}
