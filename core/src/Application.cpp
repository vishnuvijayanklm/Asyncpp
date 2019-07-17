#include <core/include/Application.h>
#include <core/include/NotifyManager.h>
#include <core/include/SharedLibraryManager.h>
#include <util/include/defines.h>
#include <unistd.h>
#include <iostream>
namespace Core
{

    Application::Application():ISubsystem(APPLICATION)
    {
    }

    Application::~Application()
    {
        LOG_VERBOSE((LOGGER),("Application::~Application>>>"));
        this->shutdown();
        LOG_VERBOSE((LOGGER),("Application::~Application<<<"));
    }

    void Application::onApplicationStart(int argc,char *argv[])
    {
        for(int i=0;i<argc;i++)
        {
            string option(argv[i]);
            std::transform(option.begin(),option.end(),option.begin(),::tolower);
            this->m_CommandOptions.insert(option);
        }
        for(unsigned int i=0;i<this->m_CommandOptions.size();i++)
        {
            LOG_INFO((LOGGER),("Argv %s",this->m_CommandOptions[i].c_str()));
        }

        // throw UNKNOWN_OPTION();
	this->m_OptionsInfo.startGet();
        Util::Option  option;
        while(this->m_OptionsInfo.getNextElement(option))
        {
            option.process(this->m_CommandOptions);
        }
	this->m_OptionsInfo.stopGet();
        this->registerSubsystem(new ThreadPool());
        this->registerSubsystem(new NotifyManager());
        this->registerSubsystem(new SharedLibraryManager());
    }
    void Application::init()
    {
        this->addOptions();
        /*this->registerSubsystem(new NotifyManager("Notifier1"));
            this->registerSubsystem(new NotifyManager("Notifier2"));
            this->registerSubsystem(new NotifyManager("Notifier3"));
            this->registerSubsystem(new NotifyManager("Notifier4"));*/
        // this->registerSubsystem(new Notifier("Notifier"));
    }

    ISubsystem* Application::getSubsystem(SubsystemId id)
    {
        shared_ptr<ISubsystem> pSubsystem;
        if(likely(this->m_SubsystemInfo.find(id,pSubsystem)))
        {
            return pSubsystem.get();
        }
        throw SUBSYSTEM_NOT_CONFIGURED(to_string(id));
    }
    void Application::shutdown()
    {
	LOG_VERBOSE((LOGGER),("Application::Shutdown>>>"));
        this->m_signalRegistry.clear();
        this->m_OptionsInfo.clear();
        this->m_CommandOptions.clear();
        shared_ptr<ISubsystem> pSubsystem;
        while(this->m_SubsystemInfo.removeFirstElement(pSubsystem))
        {
            if(likely(pSubsystem.get() != nullptr))
            {
            	pSubsystem->shutdown();
            }
        }
        LOG_VERBOSE((LOGGER),("Application::Shutdown<<<"));
    }

    bool Application::registerSubsystem(ISubsystem *pSubsystem)
    {
        if(likely(pSubsystem != nullptr))
        {
            if(likely(!this->m_SubsystemInfo.find(pSubsystem->id())))
            {
                pSubsystem->init();
                this->m_SubsystemInfo.insert(pSubsystem->id(),unique_ptr<ISubsystem>(pSubsystem));
                return true;
            }
            throw DUPLICATE_SUBSYSTEM(to_string(pSubsystem->id()));
        }
        throw INVALID_SUBSYSTEM();
        return false;
    }

    void Application::run()
    {
	int i = 1000;
        while(i>0)
        {
            NotifyManager *pNotifyManager = (NotifyManager*)(this->getSubsystem(NOTIFY_MANAGER));
            //while(1)
            {
                if(unlikely(pNotifyManager == nullptr))
                {
                    LOG_ERROR((LOGGER),("NotifyManager not found"));
		}
		else
		{
			EventResponse<int> resp;
			pNotifyManager->dispatch(bind(&Application::version,this,"",""),resp,1);
			//pNotifyManager->dispatch(bind(&Application::versionS,this),1);
			LOG_INFO(LOGGER,("Response %d",resp.get_value()));
		}
            }
        }
    }

    void Application::registerSignal(int sig,fn fnptr)
    {
        this->m_signalRegistry.insert(sig,fnptr);
        signal(sig,fnptr);
    }

    void Application::addOptions()
    {
        this->addOption(Util::Option("-v","--version")
                        .isRepeatable(false)
                        .isRequired(false)
                        .setCallBack(bind(&Application::version,this,placeholders::_1,placeholders::_2)));
    }

    void Application::addOption(Util::Option &options)
    {
        this->m_OptionsInfo.insert(options.shortName(),options);
        this->m_OptionsInfo.insert(options.fullName(),options);
    }

    int Application::version(string,string)
    {
        int randNum = rand() % 1000;
        LOG_INFO(LOGGER,("Version 1.2.5 %d",randNum));
        return randNum;
    }

    void Application::versionS()
    {
        LOG_INFO(LOGGER,("VersionS 1.2.5 %d %d",version("",""),version("","")));
    }
}
