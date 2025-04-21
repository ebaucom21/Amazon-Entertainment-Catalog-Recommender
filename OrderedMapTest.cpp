/*

Use this to test the functionality of the OrderedMap class with the CSV data.

*/
#include <iostream>
#include <vector>
#include <string>
#include "OrderedMap.cpp"
#include "CSV-Extractor.cpp"
using namespace std;

int main() {
    
    //Allows to get the CSV data
    vector<vector<vector<string>>> data;
    data = csvExtraction().extractData();
    if (data.empty()) {
        cout << "No data extracted from the CSV file.\n";
        return 1;
    }
    else {
        cout << "Data extracted successfully.\n";
    }

    
    // This is a randomized test target
    vector<string> target = {"Movie", "None", "None", "1-1.5hrs", "Drama"};

    // The ordered map is made here and we go trough the top 5 recommendations made
    cout << "Creating ordered map...\n";
    ordered_map myMap( data , target);
    cout<< "Ordered map created successfully.\n";
    cout<< "Finding top 5 recommendations...\n";

    vector<string> str= myMap.getTopKRecom(5);
    int count = 1;

    for(auto recoms: str){
        cout << count << ". " << recoms << endl;
        count++;
    }

    cout << "Top 5 recommendations found successfully.\n";

    return 0;
}