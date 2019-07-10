#include <util/include/File.h>
#include <core/include/Application.h>
#include <core/include/NotifyManager.h>
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
