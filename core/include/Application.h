#ifndef APPLICATION_H
#define APPLICATION_H
#include <Subsystem.h>
#include <Stl.h>
#include <signal.h>
#include <Option.h>
#include <NotifyManager.h>
#include <Exception.h>
namespace Core
{

	class Application : public ISubsystem,Util::Runnable
	{
		private:
			typedef void(*fn)(int);
			StlMap<int,fn> m_signalRegistry;
			StlMap<string,Util::Option> m_OptionsInfo;
			StlMap<string,shared_ptr<ISubsystem>> m_SubsystemInfo;
			StlVector<string> m_CommandOptions;

			int version(string,string);
			void versionS();
			bool processOptions();

			void addOptions();

			int getval();
		public:

			Application(string name);
			virtual ~Application();
			void init();
			void run();
			void shutdown();

			void addOption(Util::Option&);
			void registerSignal(int,fn);
			void onApplicationStart(int argc,char *argv[]);
			bool registerSubsystem(ISubsystem*);
			ISubsystem* getSubsystem(string);
	};
}
#endif // APPLICATION_H
