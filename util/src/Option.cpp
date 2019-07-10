#include <util/include/Option.h>
#include <util/include/defines.h>
namespace Util
{
    Option::Option()
    {
        this->m_shortName.clear();
        this->m_fullName.clear();
        this->m_Seperator.clear();
        this->m_isRepeatable = false;
        this->m_isRequired = false;
        this->m_OptionCallback = nullptr;
    }

    Option::Option(string shortName,string fullName):Option()
    {
        this->m_shortName.resize(shortName.size());
        this->m_fullName.resize(fullName.size());
        std::transform(shortName.begin(),shortName.end(),this->m_shortName.begin(),::tolower);
        std::transform(fullName.begin(),fullName.end(),this->m_fullName.begin(),::tolower);
    }

    Option::~Option()
    {
        //dtor
    }

    Option& Option::isRepeatable(bool isRepeatable)
    {
        this->m_isRepeatable = isRepeatable;
        return *this;
    }

    Option& Option::isRequired(bool isRequired)
    {
        this->m_isRequired = isRequired;
        return *this;
    }

    Option& Option::setSeperator(string Seperator)
    {
        this->m_Seperator = Seperator;
        return *this;
    }

    Option& Option::setCallBack(function<bool(string,string)> callBack)
    {
        this->m_OptionCallback = callBack;
        return *this;
    }
    const string& Option::shortName() const
    {
        return this->m_shortName;
    }
    const string& Option::fullName() const
    {
        return this->m_fullName;
    }

    bool Option::process(StlVector<string>& Args)
    {
        LOG_INFO((LOGGER),("Option::process  [%s,%s] Required %d",this->m_shortName.c_str(),this->m_fullName.c_str(),this->m_isRequired));
        if(Args.isExist(this->m_fullName) || Args.isExist(this->m_shortName))
        {
            if(likely(this->m_OptionCallback != nullptr))
            {
                return this->m_OptionCallback(this->m_fullName,"Test");
            }
            return true;
        }
        else
        {
            if(unlikely(this->m_isRequired))
            {
                throw MANDATORY_OPTION_MISSING(this->m_fullName +"/" + this->m_shortName);
            }
        }
        return true;
    }
}
