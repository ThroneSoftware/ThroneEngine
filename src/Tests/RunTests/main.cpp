#include <cstdlib>
#include <iostream>

void executeTest(std::string_view test_name)
{
	std::string command = "call \"bin/Debug/";
	command += test_name;
	command += "\"";
	std::system(command.c_str());
}

int main()
{
	executeTest("TestTestFramework.exe");
    executeTest("TestStandard.exe");
}