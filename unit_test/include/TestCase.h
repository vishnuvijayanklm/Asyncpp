#ifndef TESTCASE_H
#define TESTCASE_H
#include <iostream>
#include <containers/include/Stl.h>

#define GREEN 		"\033[32m"
#define RED 		"\033[31m"
#define DEFAULT 	"\033[0m"

#define START_FN 	cout<<GREEN<<"[---------] "<<DEFAULT<<getName()<<"\n"<<GREEN<<"[ RUNNING ] "<<DEFAULT<<endl
#define END_FN  	cout<<GREEN<<"[---------]"<<DEFAULT<<" Success "<<getSuccessRate()<<" Failed "<<getFailureRate()<<endl<<endl;

#define _FILE_ string(__FILE__).substr(string(__FILE__).find_last_of("//") + 1 )

#define OPERATION(X,Y,OP)			\
{						\
bool isSuccess = true;				\
auto start = std::chrono::system_clock::now();	\
if(X OP Y)					\
{						\
	cout<<GREEN<<"[   OK    ] ";		\
	usleep(rand() % 1000 * 1000);		\
}						\
else						\
{						\
	cout<<RED<<"[ FAILED  ] ";		\
	usleep(rand() % 1000 * 1000);		\
	isSuccess = false;			\
}						\
auto end = std::chrono::system_clock::now();  	\
cout<<_FILE_<<":"<<__LINE__<<" EXPECTED "<<#X	\
<<" "<<#OP<<" "#Y<<" ACTUAL "<<#X<<" vs "<<#Y	\
<<DEFAULT<<" ("<<chrono::duration_cast<	\
std::chrono::milliseconds>(end - start).count()	\
<<" ms)"<<endl;					\
return isSuccess;				\
}



#define CHECK_EQ(X,Y) OPERATION(X,Y,==)
#define CHECK_NE(X,Y) OPERATION(X,Y,!=)

namespace UNIT_TEST
{
	class ITestCase
	{
		public:
			virtual void execute() = 0;
	};
	class TestCase
	{
			StlQueue<ITestCase*> mTestCases;	
			TestCase()
			{
			}
		public:
		
			~TestCase();
			static TestCase* getInstance()
			{
				static TestCase *pTestCase = new TestCase();
				return pTestCase;
			}
			
			void addTestCase(ITestCase *pTestCase)
			{
				this->mTestCases.push(pTestCase);
			}

			void runAllTestCases()
			{
				StlQueue<ITestCase*> temp(this->mTestCases);
				ITestCase *pTestCase = nullptr;
				while(temp.pop(pTestCase) && pTestCase != nullptr)
				{
					pTestCase->execute();
				}
			}
	};
}

#define TEST_CASE(X) 						\
class X##_Test:public UNIT_TEST::ITestCase			\
{ 								\
		size_t mTotal;					\
		size_t mFailed;					\
	public:							\
		X##_Test()					\
		{						\
			this->mTotal = 4;			\
			this->mFailed = 0;			\
			UNIT_TEST::TestCase::getInstance()	\
			->addTestCase(this);			\
		}						\
		~X##_Test()					\
		{						\
		} 						\
		const char* getName()				\
		{						\
			return #X;				\
		}						\
		bool X();					\
		void execute()					\
		{						\
			START_FN;				\
			if(this->X())				\
			{					\
				this->mFailed++;		\
			}					\
			END_FN;					\
		}						\
		size_t getFailureRate()				\
		{						\
			return this->mFailed;			\
		}						\
		size_t getSuccessRate()				\
		{						\
			return this->mTotal - this->mFailed;	\
		}						\
}X##_Obj;							\
bool X##_Test::X() 

#endif
