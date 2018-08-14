#include <File.h>
#include <Application.h>
#include <NotifyManager.h>
extern unique_ptr<BHI::Core::Application> pApplication;
namespace BHI
{
    namespace Util
    {
        File::File()
        {
            //ctor
        }

        File::~File()
        {
            this->close();
        }
    }
}
