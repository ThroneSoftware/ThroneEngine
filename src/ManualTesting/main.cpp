//#include "Test.h"

#include <Core/Core.h>

//#include <Test.h>
//#include <Vendors/Test/Test.h>
//#include <boost/test.h>
//#include <glm/test.h>
#include <windows.h>

#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <glm/vec3.hpp>

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

	std::optional<int> opt;
}