#include "Data.h"

Data::Data(std::string input) {
	std::ifstream testFile;
	std::string filePath = "C:\\Users\\matt_\\Downloads\\" + input;
	testFile.open(filePath);
	std::string temp;
	std::vector<float> initVect;
	while (std::getline(testFile,temp)) {
		std::stringstream ss(temp);
		float num = 0;
		//bool first = true;
		while (ss >> num)
		{
			initVect.push_back(num);
		}
		this->data.push_back(initVect);
		initVect.resize(0);
	}
	for (int i = 0; i < this->data.size(); i++) {
		for (int j = 1; j < this->data[i].size(); j++)
		{
			this->data[i][j] = std::log(this->data[i][j] + 1);
		}
	}
	testFile.close();
}

void Data::printData() {
	for (int i = 0; i < this->data.size(); i++) {
		for (int j = 0; j < this->data[i].size(); j++)
		{
			std::cout << this->data[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

float Data::dataAt(int i, int j) {
	//std::cout << "index x = " << i << " y = " << j << std::endl;
	return this->data[i][j];
}

int Data::nearestNeighbor(std::vector<int> featureSubSet, std::vector<float> newInstance) {
	int currMinIndex = 0;
	if (!newInstance.empty())
	{
		//set to a high number so it can be changed when compared to
		float Min = 10000000.0;

		//cycles through all 
		for (unsigned currInstance = 0; currInstance < this->data.size(); currInstance++)
		{
			float Temp = 0;
			//cycle through all inexoffeatures we are using for eucledian distance
			for (unsigned currFeatureIndex = 0; currFeatureIndex < featureSubSet.size(); currFeatureIndex++)
			{   
				float delta = this->data[currInstance][featureSubSet[currFeatureIndex]] - newInstance[featureSubSet[currFeatureIndex]];
				Temp += std::pow(delta,2);

			}
			Temp = sqrt(Temp);
			//finds min distance
			if (Min > Temp)
			{
				currMinIndex = currInstance;
				Min = Temp;
			}

		}
		//returns either 1 or 2 which is the class label
		return this->data[currMinIndex][0];
}