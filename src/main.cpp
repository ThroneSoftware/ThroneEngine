#include <iostream>
#include <optional>
#include <string>
#include <windows.h>

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

	std::optional<int> opt;
}