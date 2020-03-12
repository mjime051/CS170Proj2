#include "Data.h"

//constructor, takes in a file name as a parameter and uses ifstream and sstream to populate our 2d vector
Data::Data(std::string input) {
	std::ifstream testFile;
	std::string filePath = "C:\\Users\\matt_\\Downloads\\" + input;
	testFile.open(filePath);
	std::string temp;
	std::vector<float> initVect;
	//while we can keep on getting line then we can continue to read from the file
	while (std::getline(testFile,temp)) {
		std::stringstream ss(temp);
		float num = 0;
		//bool first = true;
		//populate the vector with everything in the line
		while (ss >> num)
		{
			initVect.push_back(num);
		}
		//push our vector into our private 2d vector variable
		this->data.push_back(initVect);
		//reset the vector so we can use it again and it has nothing in it for the next iteration
		initVect.resize(0);
	}
	//this is to normalize the data using log normalization
	/*for (int i = 0; i < this->data.size(); i++) {
		for (int j = 1; j < this->data[i].size(); j++)
		{
			this->data[i][j] = std::log(this->data[i][j] + 1);
		}
	}*/
	testFile.close();
}

//helper function I used to make sure my 2d vector was populated correctly
void Data::printData() {
	for (int i = 0; i < this->data.size(); i++) {
		for (int j = 0; j < this->data[i].size(); j++)
		{
			std::cout << this->data[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

//helper function that prints out the feature subset we pass in and the accuracy we pass in
void Data::print(std::vector<int> SetofIndex, float Percentage) {
	std::cout << "Using feature set: ";
	for (unsigned i = 0; i < SetofIndex.size(); i++)
	{
		std::cout << SetofIndex[i] << ' ';
	}
	std::cout << "accuracy of " << Percentage << std::endl;
}

//Just a helper function to make sure values are where they should be
float Data::dataAt(int i, int j) {
	//std::cout << "index x = " << i << " y = " << j << std::endl;
	return this->data[i][j];
}

int Data::nearestNeighbor(std::vector<int> currFeatureSubset, std::vector<std::vector<float>>  trainingData, std::vector<float> testInstance)
{
	//index of where our current min is
	int currMinIndex = 0;
	
	//set to a high number so it can be changed later
	float currMin = FLT_MAX;
	//Going through all features in our feature subset
	for (unsigned currInstIndex = 0; currInstIndex < trainingData.size(); currInstIndex++)
	{
		//reset for each instance since we are finding the distance between our "new" instance and all other instances
		float currInstanceDist = 0;
		//for each instance we need to get the difference between our "new" instance and all other instances
		for (unsigned currFeatureIndex = 0; currFeatureIndex < currFeatureSubset.size(); currFeatureIndex++)
		{
			//now that we have the difference we can square it and add it to the current distance to find distance
			//between two points after 
			float delta = trainingData[currInstIndex][currFeatureSubset[currFeatureIndex]] - testInstance[currFeatureSubset[currFeatureIndex]];
			currInstanceDist += (delta * delta);
		}
		//now take the square root and we have the distance between two instances
		currInstanceDist = std::sqrt(currInstanceDist);
		//if that distance is less than our current min distance then change our current min distance for later 
		//and keep track of the index of the instance that is closest. This is where if we had to incorporate k-NN
		//instead of 1-NN things would change
		if (currMin > currInstanceDist)
		{
			currMinIndex = currInstIndex;
			currMin = currInstanceDist;
		}
	}
	//returns either 1 or 2 which is the class label of our nearest neighbor
	return trainingData[currMinIndex][0];
}
//We need to find the accuracy of our feture subset.
//Go through all instances and compare them with all other instances to see the accuracy of the feature subset
//using nearestNeighbor
float Data::leaveOneOut(std::vector<int> indexOfFeatures)
{
	float correct = 0.0;
	//Going through all instances
	for (unsigned i = 0; i < this->data.size(); i++)
	{
		//current instance we are at will be our test instance
		std::vector<float> testInstance = this->data[i];
		//copy the 2d vector into a temp 2d vector
		std::vector<std::vector<float>> Temp_2 = this->data;
		//take out the instance that we are currently at otherwise that would be considered the nearest neighbor
		Temp_2.erase(Temp_2.begin() + i);
		//call nearest neighbor to see what the label says it is
		float nnPredictedClass = nearestNeighbor(indexOfFeatures, Temp_2, testInstance);
		//compare to what the label actually is, if its the same then it was a correct guess so increment the count
		if (nnPredictedClass == testInstance[0])
		{
			correct = correct + 1.0;
		}
	}
	//returns accuracy(the number of correct/the total number of comparisons, which is equal to the number of instances(
	return correct / this->data.size();
}

void Data::BestSubSetForward() {
	std::vector<int> currFeatureSubset;
	float currFeatureSubsetAccuracy = 0;

	std::vector<int> nLengthBestSetOfIndex;
	float nLengthBestPercentage = 0;

	std::vector<int> BestSetOfIndex;
	float BestSetPercentage = 0;

	std::set<int> remainingFeatures;

	

	for (unsigned i = 1; i < this->data[0].size(); i++)
	{
		remainingFeatures.insert(i);
	}
	//We will go through all the features, start at one to ignore the label
	for (unsigned i = 1; i <= remainingFeatures.size(); i++)
	{
		//reset percentage for this feature subset
		nLengthBestPercentage = 0;
		//resize to fit the features we are currently going through, aka add one to size
		currFeatureSubset.resize(i);
		std::set<int>::iterator it = remainingFeatures.begin();
		for (unsigned j = 1; j < remainingFeatures.size(); j++)
		{
			//this check will make sure you are not repeating any features in feature subsets
			//if we are doing feature subset 5 3 x and we iterate through and get to 5 3 5 this check will skip 
			//that iteration because 5 is already in that feature subset
			//std::vector<int>::iterator it = std::find(nLengthBestSetOfIndex.begin(), nLengthBestSetOfIndex.end(), j);
			//if (it == nLengthBestSetOfIndex.end())
			//{
				//Now let us update our currFeatureSubset to include another feature
			
				currFeatureSubset[currFeatureSubset.size() - 1] = *it;
				//find the percentage
				currFeatureSubsetAccuracy = leaveOneOut(currFeatureSubset);

				//print out the feature subset and accuracy as per guidelines
				print(currFeatureSubset, currFeatureSubsetAccuracy);
				//if our current percent is more than the percent of our prev best then set our next iteration that 
				//we will expand on to be the curr
				if (currFeatureSubsetAccuracy > nLengthBestPercentage)
				{
					nLengthBestSetOfIndex = currFeatureSubset;
					nLengthBestPercentage = currFeatureSubsetAccuracy;
				}
				//if our current percent is more than our overall best accuracy that we have found so far then 
				//set overall best set to curr
				if (currFeatureSubsetAccuracy > BestSetPercentage)
				{
					BestSetPercentage = currFeatureSubsetAccuracy;
					BestSetOfIndex = currFeatureSubset;
				}
			//}
				it++;
		}
		
		remainingFeatures.erase(nLengthBestSetOfIndex.back());
		currFeatureSubset = nLengthBestSetOfIndex;
	}

	std::cout << "Best accuracy was ";
	print(BestSetOfIndex, BestSetPercentage);
}

void Data::BestSubSetBackward()
{
	std::vector<int> currFeatureSubset;
	float currFeatureSubsetAccuracy = 0;
	std::vector<int> nLengthBestSetOfIndex;
	float nLengthBestPercentage = 0;
	std::vector<int> BestSetOfIndex;
	float BestSetPercentage = 0;
	std::vector<int> SET;

	//fill our vector with all features
	for (unsigned i = 1; i < this->data[0].size(); i++)
	{
		currFeatureSubset.push_back(i);
	}

	//check accuracy of entire feature Set
	currFeatureSubsetAccuracy = leaveOneOut(currFeatureSubset);
	//set it as best to compare with all future feature subsets
	BestSetPercentage = currFeatureSubsetAccuracy;
	BestSetOfIndex = currFeatureSubset;
	print(currFeatureSubset, currFeatureSubsetAccuracy);

	SET = currFeatureSubset;
	currFeatureSubset.pop_back();

	bool FIRST = true;
	while (!currFeatureSubset.empty())
	{
		//reset the percent of current iteration of feature subset
		nLengthBestPercentage = 0;
		//for the first run we technically already set the currfeatureaccuracy so no need to do again
		if (FIRST)
		{
			FIRST = false;
		}
		else
		{
			currFeatureSubsetAccuracy = leaveOneOut(currFeatureSubset);
			print(currFeatureSubset, currFeatureSubsetAccuracy);
			if (currFeatureSubsetAccuracy > nLengthBestPercentage)
			{
				nLengthBestSetOfIndex = currFeatureSubset;
				nLengthBestPercentage = currFeatureSubsetAccuracy;
			}

			if (currFeatureSubsetAccuracy > BestSetPercentage)
			{
				BestSetPercentage = currFeatureSubsetAccuracy;
				BestSetOfIndex = currFeatureSubset;
			}
		}

		for (unsigned j = SET.size(); j > 1; j--)
		{
			if (j == SET.size())
			{
				currFeatureSubset[SET.size() - j] = SET[j - 1];
			}
			else
			{
				currFeatureSubset[SET.size() - j] = SET[SET.size() - j - 1];
			}

			currFeatureSubsetAccuracy = leaveOneOut(currFeatureSubset);
			print(currFeatureSubset, currFeatureSubsetAccuracy);
			if (currFeatureSubsetAccuracy > nLengthBestPercentage)
			{
				nLengthBestSetOfIndex = currFeatureSubset;
				nLengthBestPercentage = currFeatureSubsetAccuracy;
			}

			if (currFeatureSubsetAccuracy > BestSetPercentage)
			{
				BestSetPercentage = currFeatureSubsetAccuracy;
				BestSetOfIndex = currFeatureSubset;
			}

		}
		currFeatureSubset = nLengthBestSetOfIndex;
		SET = currFeatureSubset;
		currFeatureSubset.pop_back();

	}

	std::cout << "Best accuracy was ";
	print(BestSetOfIndex, BestSetPercentage);
}
