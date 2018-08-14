#ifndef OPTION_H
#define OPTION_H
#include<string>
#include <iostream>
#include <Stl.h>
#include <functional>
#include <Exception.h>
#include <defines.h>
using namespace std;
namespace Util
{
    class Option
    {
    private:
        string m_fullName;
        string m_shortName;
        string m_Seperator;

        bool m_isRequired;
        bool m_isRepeatable;

        function<bool(string,string)> m_OptionCallback;
    protected:

    public:
        Option();
        Option(string,string);
        virtual ~Option();
        Option& isRepeatable(bool);
        Option& isRequired(bool);
        Option& setCallBack(function<bool(string,string)>);
        Option& setSeperator(string);
        bool process(StlVector<string>&);

        const string& shortName() const;
        const string& fullName() const;
    };
}
#endif // OPTION_H
