#include "Data.h"

int main() {
	//std::string input;
	int option = 0;
	std::cout << "Welcome to Matthew Jimenez' Feature Selection Algorithm" << std::endl;
	//std::cout << "Type in the name of the test file" << std::endl;
	//std::getline(std::cin, input);

	std::cout << "Type the number of the test that you will run: small(1) or large(2)" << std::endl;
	std::cin >> option;
	std::string test1 = "cs_170_small80.txt";
	std::string test2 = "cs_170_large80.txt";
	if (option == 1)
	{
		Data* data = new Data(test1);
		data->printData();
	}
	else
	{
		Data* data = new Data(test2);
		data->printData();
	}
	return 0;
}