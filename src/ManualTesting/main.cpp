//#include "Test.h"

#include <Core/Core.h>

#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <glm/vec3.hpp>
#include <gsl/pointers>
#include <windows.h>

#include <iostream>
#include <optional>
#include <string>
#include <vector>

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

    auto str = fmt::format("{} {} {}", "test", "1", "3");

    std::cout << str << "\n";

	std::optional<int> opt;
}