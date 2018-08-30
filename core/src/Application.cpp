#include "Application.h"
#include <unistd.h>
#include <iostream>

namespace Core
{

    Application::Application(string name):ISubsystem(name)
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
        Getter<StlMap<string,Util::Option>>(this->m_OptionsInfo);
        Util::Option  option;
        while(this->m_OptionsInfo.getNextElement(option))
        {
            option.process(this->m_CommandOptions);
        }
        this->registerSubsystem(new NotifyManager("Notifier"));
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

    ISubsystem* Application::getSubsystem(string name)
    {
        shared_ptr<ISubsystem> pSubsystem;
        if(this->m_SubsystemInfo.find(name,pSubsystem))
        {
            return pSubsystem.get();
        }
        throw SUBSYSTEM_NOT_CONFIGURED(name);
    }
    void Application::shutdown()
    {
	cout<<"Before Shutdown print"<<endl;
        /*LOG_VERBOSE((LOGGER),("Application::Shutdown>>>"));
        this->m_signalRegistry.clear();
        this->m_OptionsInfo.clear();
        this->m_CommandOptions.clear();
        shared_ptr<ISubsystem> pSubsystem;
        while(this->m_SubsystemInfo.removeFirstElement(pSubsystem))
        {
            if(pSubsystem.get())
            {
            	pSubsystem->shutdown();
            }
        }
        LOG_VERBOSE((LOGGER),("Application::Shutdown<<<"));*/
    }

    bool Application::registerSubsystem(ISubsystem *pSubsystem)
    {
        if(pSubsystem)
        {
            if(!this->m_SubsystemInfo.find(pSubsystem->name()))
            {
                pSubsystem->init();
                this->m_SubsystemInfo.insert(pSubsystem->name(),unique_ptr<ISubsystem>(pSubsystem));
                return true;
            }
            throw DUPLICATE_SUBSYSTEM(pSubsystem->name());
        }
        throw INVALID_SUBSYSTEM();
        return false;
    }

    void Application::run()
    {
        while(1)
        {
            NotifyManager *pNotifyManager = (NotifyManager*)(this->getSubsystem("Notifier"));
            //while(1)
            {
                if(!pNotifyManager)
                {
                    LOG_ERROR((LOGGER),("NotifyManager not found"));
                }
                else
                {
                    LOG_INFO(LOGGER,("Dispatching>>>"));
                    //future<int> ret;
                    pNotifyManager->dispatch(bind(&Application::versionS,this));
                    pNotifyManager->dispatch(bind(&Application::version,this,"",""));
                    //LOG_INFO(LOGGER,("Response %d",ret.get()));
                }
                //std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
