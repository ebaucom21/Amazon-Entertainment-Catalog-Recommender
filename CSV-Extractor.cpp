#include <vector>
#include <string>

class csvExtraction {
    public:
        // Function to extract data from a CSV file and store it in a vector of vectors of strings
        vector<vector<vector<string>>> extractData() {
            return extractDataHelper("amazon_prime_titles.csv");
        }
    private:
        vector<vector<vector<string>>> extractDataHelper(const string& filename) {
            // Read File
            // Only take the data I need
        }

}