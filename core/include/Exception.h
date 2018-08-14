#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <exception>
#include <string>
using namespace std;
namespace Core
{
    class Exception:public exception
    {
    private:
    protected:
    public:

        Exception();
        virtual ~Exception();
        virtual const char* what() const throw() = 0;
        virtual const char* getString() const = 0;
        virtual int getCode() const = 0;
        virtual const char* getAdditionalInfo() const = 0;
    };
}

#define REGISTER_EXCEPTION(EXCEPTION,BASE,errorCode,errorMsg)\
class EXCEPTION : public /*BASE*/ Core::Exception  \
{                                                       \
private :                                           \
string m_additionalInfo;                    \
protected:                                          \
public:                                             \
    EXCEPTION()                                 \
                                                        {                                           \
                                                                                                    this->m_additionalInfo.clear();         \
                                                                                                }                                           \
EXCEPTION(string additionalInfo)            \
{                                           \
                                            this->m_additionalInfo = additionalInfo;\
                                                                 }                                           \
~EXCEPTION()                                \
{                                           \
                                        }                                           \
const char* what() const throw()            \
{                                           \
                                            return errorMsg;                        \
                                        }                                           \
const char* getString() const               \
{                                           \
                                            return errorMsg;                        \
                                        }                                           \
int getCode() const                         \
{                                           \
                                            return errorCode;                       \
                                        }                                           \
const char* getAdditionalInfo() const       \
{                                           \
                                            return this->m_additionalInfo.c_str();  \
                                        }                                           \
};

/*-------------Option Exception 10X------------------------*/
REGISTER_EXCEPTION(MANDATORY_OPTION_MISSING,OPTION_EXCEPTION,-100,"Mandatory option missing");
REGISTER_EXCEPTION(UNKNOWN_OPTION,OPTION_EXCEPTION,-101,"Unknown Option");

/*-------------Subsystem Exception 11X---------------------*/
REGISTER_EXCEPTION(SUBSYSTEM_NOT_CONFIGURED,SUBSYSTEM_EXCEPTION,-110,"Subsystem not configured");
REGISTER_EXCEPTION(DUPLICATE_SUBSYSTEM,SUBSYSTEM_EXCEPTION,-111,"Duplicate Subsytem");
REGISTER_EXCEPTION(INVALID_SUBSYSTEM,SUBSYSTEM_EXCEPTION,-112,"Invalid Subsystem / Subsystem pointer empty");


#endif // EXCEPTION_H

