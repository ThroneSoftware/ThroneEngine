#include <iostream>
#include <optional>
#include <string>
#include <windows.h>

#include <Core/Core.h>

using namespace std;
string ExePath()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}

int main()
{
    // test
    std::cout << ExePath() << std::endl;
	std::cout << "Throne!" << std::endl;
    print(10);

	std::optional<int> opt;
}