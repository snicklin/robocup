#include "DictionaryApproximator.h"
#include <sstream>
#include <fstream>
#include <vector>

void DictionaryApproximator::initialiseApproximator(int numberOfInputs, int numberOfOutputs, int numberOfHiddens) {
    numInputs = numberOfInputs;
    numOutputs = numberOfOutputs;
    tileMultiplier = numberOfHiddens;
}
    
void DictionaryApproximator::doLearningEpisode(vector< vector<float> > const& observations, vector< vector<float> > const& values, float stepSize, int iterations) {
    string tmp;
    for (int i = 0; i < observations.size(); i++) {
       //for each observation
        for (int j = 0; j < numOutputs; j++) {
          //for each possible action
            tmp = getValue(observations[i],j);
            map[tmp] = values[i][j];//Assign the value function to be the input values.
        }
    }
}
    
vector<float> DictionaryApproximator::getValues(vector<float> const& observations) {
    vector<float> result;
    
    for (int i = 0; i < numOutputs; i++) {
        result.push_back(getValue(observations,i));
    }
}
    
void DictionaryApproximator::saveApproximator(string agentName) {
    ifstream save_file;
    string file_name = agentName+"_approximator";//Added by Jake
    save_file.open(file_name,ios_base::in);
    string tempstr;
    
    save_file << approximator.size();
    
    for (map<string,float>::iterator iter = approximator.begin(); iter != approximator.end(); iter++) {
        save_file << "\n" << iter.first << " " << iter.second;
    }
    
}
    
void DictionaryApproximator::loadApproximator(string agentName) {
    ifstream save_file;
    string file_name = agentName+"_approximator";//Added by Jake
    save_file.open(file_name,ios_base::in);
    string tempstr;
    float tempval;
    int numvals;
    
    //get the number of values
    save_file >> numvals;
    
    for (int i = 0; i < numvals; i++) {
        save_file >> tempstr;
        save_file >> tempval;
        approximator[tempval] = tempstr;
    }
}


string DictionaryApproximator::getRepresentation(vector<float> const& observations,int action) {
    stringstream result;
    
    for (int i = 0; i < observations.size(); i++) {
        result << (int)(observations[i]*tileMultiplier) << " ";
    }
    result << action;
    return result.str();
}

float DictionaryApproximator::getValue(vector<float> const& observations,int action) {
    
    string val = getRepresentation(observations,action);
    float result = approximator[val];
    return result;
}

float DictionaryApproximator::setValue(vector<float> const& observations,int action,float value) {
    string val = getRepresentation(observations,action);
    approximator[val] = value;
}