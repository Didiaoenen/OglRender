
#include <regex>
#include <string>

#include <iostream>

int main()
{
	std::string str("#name 0 1");
	std::regex e("(\\w+)[ ](\\w+)");
	std::smatch sm;
	std::regex_search(str, sm, e);

	std::cout << sm.prefix() << std::endl;
	std::cout << sm.suffix() << std::endl;

	return 0;
}