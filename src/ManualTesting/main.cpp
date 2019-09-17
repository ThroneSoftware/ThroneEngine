#include <iostream>
#include <optional>
#include <string>
#include <windows.h>

#include <Core/Core.h>
#include <Core2/Core2.h>

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
    std::cout << ExePath() << std::endl;
	std::cout << "Throne!" << std::endl;
    print(10);
    print2(11);


	std::optional<int> opt;
}