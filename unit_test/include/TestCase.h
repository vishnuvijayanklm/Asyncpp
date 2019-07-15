#ifndef TESTCASE_H
#define TESTCASE_H
#include <iostream>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define DEFAULT "\033[0m"

#define CHECK_NE(X,Y) std::cout<<GREEN<<"\n[ RUNNING ] "<<DEFAULT<<X <<" != "<<Y<<std::endl;if(X != Y ) {std::cout<<GREEN<<"[   OK    ]"<<DEFAULT<<" 2 ms"<<std::endl;}else{std::cout<<RED<<"[ FAILED  ]"<<DEFAULT<<" 2 ms"<<std::endl;};
#define CHECK_EQ(X,Y) std::cout<<GREEN<<"\n[ RUNNING ] "<<DEFAULT<<X <<" == "<<Y<<std::endl; if(X == Y ) {std::cout<<GREEN<<"[   OK    ]"<<DEFAULT<<" 2 ms"<<std::endl;}else {std::cout<<RED<<"[ FAILED  ]"<<DEFAULT<<" 2 ms"<<std::endl;};


namespace UNIT_TEST
{
	class TestCase
	{
		
		public:
		TestCase();
		~TestCase();
	};
}
#endif
