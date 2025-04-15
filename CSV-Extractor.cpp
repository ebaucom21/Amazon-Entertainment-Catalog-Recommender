#include <vector>
#include <string>

using namespace std;

class csvExtraction {
    public:
        // Function to extract data from a CSV file and store it in a vector of vectors of strings
        // Returns a vector of vectors of -
        /* 0 - Title
        1 - Type
        2 - vector of Actors/Directors
        3 - Rating
        4 - Duration
        5 - Genre
        */
        vector<vector<vector<string>>> extractData() {
            return extractDataHelper("amazon_prime_titles.csv");
        }
    private:
        vector<vector<vector<string>>> extractDataHelper(const string& filename) {
            // Read File
            // Only take the data I need
        }

};