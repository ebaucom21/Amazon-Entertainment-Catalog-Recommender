/*

Use this to test the functionality of the KD-Tree class with the CSV data.

*/
#include <iostream>
#include <vector>
#include <string>
#include "Balanced-KD.cpp"
#include "CSV-Extractor.cpp"
using namespace std;

int main() {
    
    vector<vector<vector<string>>> data;
    data = csvExtraction().extractData();
    if (data.empty()) {
        cout << "No data extracted from the CSV file.\n";
        return 1;
    }
    else {
        cout << "Data extracted successfully.\n";
    }
    // Create an instance of the balancedKD class with the extracted data
    cout << "Creating KD tree...\n";
    balancedKD kdTree(data);
    cout << "KD tree created successfully.\n";
    vector<vector<string>> target = {
        {"Movie"}, // Type
        {"None"}, // Actor/Directors
        {"None"}, // Rating
        {"1-1.5hrs"}, // Duration
        {"Drama"}  // Genre
    };
    // Find the nearest neighbors for the target vector
    cout << "Finding nearest neighbors...\n";
    vector<string> neighbors = kdTree.findNearestNeighbors(target, 5);
    for (const auto& neighbor : neighbors) {
        cout << "Neighbor: " << neighbor << "\n";
    }
    return 0;
}