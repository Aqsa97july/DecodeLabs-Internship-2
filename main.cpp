#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <map>
#include <random>


using namespace std; 

struct Flower {
    double features[4];
    string species;
};

int main() {
    vector<Flower> data;
    ifstream file("iris.csv"); 

    if (!file.is_open()) {
        cout << "Error: Could not find iris.csv in this folder!" << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string val;
        Flower f;
        
        for (int i = 0; i < 4; i++) {
            getline(ss, val, ',');
            f.features[i] = stod(val);
        }
        getline(ss, f.species, ',');
        data.push_back(f);
    }
    file.close();

random_device rd;
mt19937 g(rd());
shuffle(data.begin(), data.end(), g);    int trainSize = data.size() * 0.8;

    int correctGuesses = 0;
    int k = 3; 

    for (int i = trainSize; i < data.size(); i++) {
        Flower testPoint = data[i];
        vector<pair<double, string>> distances;

        for (int j = 0; j < trainSize; j++) {
            double dist = 0;
            for (int feat = 0; feat < 4; feat++) {
                dist += pow(testPoint.features[feat] - data[j].features[feat], 2);
            }
            distances.push_back({sqrt(dist), data[j].species});
        }

        sort(distances.begin(), distances.end());

        // Vote!
        map<string, int> votes;
        for (int n = 0; n < k; n++) {
            votes[distances[n].second]++;
        }

        string prediction;
        int maxVotes = -1;
        for (auto const& [name, count] : votes) {
            if (count > maxVotes) {
                maxVotes = count;
                prediction = name;
            }
        }

        if (prediction == testPoint.species) correctGuesses++;
    }

    cout << "Finished! Accuracy: " << (double)correctGuesses / (data.size() - trainSize) * 100 << "%" << endl;

    return 0;
}

