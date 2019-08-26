#include <util/include/File.h>
#include <core/include/NotifyManager.h>

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
