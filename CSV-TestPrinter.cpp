/*

This file is used if you would like to test how the csv-extractor
will output data to your file/what is being returned by the extractor

*/
#include <iostream>
#include <vector>
#include <string>
#include "CSV-Extractor.cpp"

int main() {

    // Test the CSV-Extractor code snippet
    // Create an instance of the csvExtraction class
    csvExtraction extractor;
    // Call the extractData method to extract data from the CSV file
    vector<vector<vector<string>>> data = extractor.extractData();

    if (data.empty()) {
        std::cout << "No data extracted from the CSV file.\n";
        return 1;
    }

    /*
    // Print one item from the extracted data
    std::cout << "Title: " << data[0][0][0] << "\n";
    std::cout << "Type: " << data[0][1][0] << "\n";
    std::cout << "Director: " << data[0][2][0] << "\n";
    std::cout << "Actors: ";
    bool first = true;
    for (const auto& actor : data[0][2]) {
        if (first) {
            first = false;
            continue;
        }
        std::cout << actor << " ";
    }
    std::cout << "\n";
    std::cout << "Rating: " << data[0][3][0] << "\n";
    std::cout << "Duration: " << data[0][4][0] << "\n";
    std::cout << "Genre: ";
    for (const auto& genre : data[0][5]) {
        std::cout << genre << " ";
    }
    std::cout << "\n\n";
*/

    // Print the extracted data
    for (const auto& row : data) {
        std::cout << "Title: " << row[0][0] << "\n";
        std::cout << "Type: " << row[1][0] << "\n";
        std::cout << "Director: " << row[2][0] << "\n";
        std::cout << "Actors: ";
        bool first = true;
        for (const auto& actor : row[2]) {
            if (first) {
                first = false;
                continue;
            }
            std::cout << actor << " ";
        }
        std::cout << "\n";
        std::cout << "Rating: " << row[3][0] << "\n";
        std::cout << "Duration: " << row[4][0] << "\n";
        std::cout << "Genre: ";
        for (const auto& genre : row[5]) {
            std::cout << genre << " ";
        }
        std::cout << "\n\n";
    }
    
    return 0;
}