#include <File.h>
#include <Application.h>
#include <NotifyManager.h>
extern unique_ptr<Core::Application> pApplication;
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
