//#include "Test.h"

#include <pch.h>

#include <Core/Core.h>
#include <Standard/CompressedPair.h>
#include <Standard/Pointers.h>

#include <boost/algorithm/string.hpp>
#include <windows.h>

//
using namespace std;
string ExePath()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);  //
}

int main()
{
	// test 9
	std::cout << ExePath() << std::endl;
	std::cout << "Throne!" << std::endl;
	print(10);	//
	print(11);
	print(12);	//

	glm::vec3 vec3;
	vec3.x = 10;

	std::vector<std::string> vec;
	boost::join(vec, "/");

	gsl::not_null<int*> test = new int();
	delete test.get();

	auto str = fmt::format("{} {} {}", "test", "1", "3");

	std::cout << str << "\n";

	std::optional<int> opt;
	std::optional<int> opt2;  //

	trs::CompressedPair<int, double> pair1;
	pair1.first();
	pair1.second();

	class A
	{
	public:
		A(int i, double d, char c)
		{
			i = i + static_cast<int>(d) + c;
		}
	};

	trs::CompressedPair<A, A> pair2(std::piecewise_construct_t(), std::forward_as_tuple(3, 4.0, 'c'), std::forward_as_tuple(5, 2.0, 'b'));

	A a = A(5, 10.0, 'd');

	trs::CompressedPair<A, A> pair3(std::piecewise_construct_t(), std::forward_as_tuple(a), std::forward_as_tuple(5, 2.0, 'b'));

	pair2.first();

	//std::forward<int>(10);

	//std::pair<int, double> pair2;

	{
		std::shared_ptr<int> ptr;
		//std::make_shared<int>();
	}

	{
		auto notifier = [](int*) {

		};

		auto ptr = trs::makePtrOwnerWithNotifier<int, decltype(notifier)>(std::move(notifier), 10);

		__nop();

		{
			trs::SharedPtr sPtr(ptr);
		}

		__nop();
	}

	{
		class B
		{
		public:
			B(int a)
			  : m_a(a)
			{
			}

			int m_a;
		};

		auto ptrOwner = trs::makePtrOwner<B>(10);
        ptrOwner->m_a;
	}

    {
        auto s1 = std::make_shared<int>(10);
        auto s2 = s1;
        s1 = std::move(s2);
    }
}