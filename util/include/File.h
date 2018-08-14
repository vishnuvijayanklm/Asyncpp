#ifndef FILE_H
#define FILE_H
#include <fstream>
#include <iostream>
using namespace std;
namespace BHI
{
    namespace Util
    {
        class File:public fstream
        {
        private:
        protected:
        public:
            File();
            virtual ~File();
        };
    }
}

#endif // FILE_H
