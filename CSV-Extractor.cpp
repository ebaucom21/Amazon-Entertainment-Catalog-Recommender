#include <vector>
#include <string>
#include <fstream>

using namespace std;

class csvExtraction {
    public:
        // Function to extract data from a CSV file and store it in a vector of vectors of strings
        // Returns a vector of vectors of -
        /* 
        0 - Title
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
            vector<vector<vector<string>>> result;
            ifstream file(filename);
            string line;
            string header;
            getline(file,header); // Read the header line
            if (header.empty()) {
                // Handle empty file case
                return result;
            }
            while (getline(file, line)) {
                vector<vector<string>> row(6); // 6 columns
                string cell;
                size_t pos = 0;
                int lastPos = 0;
                // discard show_id
                lastPos = line.find(',');
                // Get type
                pos = line.find(',', lastPos + 1);
                cell = line.substr(lastPos + 1, pos - lastPos - 1);
                row[1].push_back(cell); // Type
                lastPos = pos;
                // Get title
                pos = line.find(',', lastPos + 1);
                cell = line.substr(lastPos + 1, pos - lastPos - 1);
                row[0].push_back(cell); // Title
                lastPos = pos;
                // Get actors/directors
                // Get director first
                pos = line.find(',', lastPos + 1);
                cell = line.substr(lastPos + 1, pos - lastPos - 1);
                row[2].push_back(cell); // Director
                lastPos = pos;
                // Get actors
                pos = line.find('\"', lastPos + 2);
                cell = line.substr(lastPos + 2, pos - lastPos - 3);
                size_t lastPos2 = 0;
                size_t pos2 = 0;
                while (cell.find(',', pos2+1) != string::npos) {
                    pos2 = cell.find(',', pos2 + 1);
                    if (pos2 != string::npos) {
                        string curAct = cell.substr(lastPos2, pos2);
                        row[2].push_back(curAct); // Actor
                        lastPos2 = pos2 + 1;
                    }
                }
                string curAct = cell.substr(lastPos2, cell.size() - lastPos2);
                row[2].push_back(curAct); // Actor
                lastPos = pos + 2;
                // Skip country
                pos = line.find(',', lastPos + 1);
                lastPos = pos;
                // Skip date_added
                pos = line.find(',', lastPos + 1);
                lastPos = pos;
                // Skip release_year
                pos = line.find(',', lastPos + 1);
                lastPos = pos;
                // Get rating
                pos = line.find(',', lastPos + 1);
                cell = line.substr(lastPos + 1, pos - lastPos - 1);
                row[3].push_back(cell); // Rating
                lastPos = pos;
                // Get duration
                pos = line.find(',', lastPos + 1);
                cell = line.substr(lastPos + 1, pos - lastPos - 1);
                row[4].push_back(cell); // Duration
                // Get genres the same as actors
                pos = line.find('\"', lastPos + 2);
                cell = line.substr(lastPos + 2, pos - lastPos - 3);
                lastPos2 = 0;
                pos2 = 0;
                while (cell.find(',', pos2+1) != string::npos) {
                    pos2 = cell.find(',', pos2 + 1);
                    if (pos2 != string::npos) {
                        string curGen = cell.substr(lastPos2, pos2);
                        row[5].push_back(curGen); // Genre
                        lastPos2 = pos2 + 1;
                    }
                }
                string curGen = cell.substr(lastPos2, cell.size() - lastPos2);
                row[5].push_back(curGen); // Genre
                lastPos = pos + 2;
                // Skip description
                pos = line.find(',', lastPos + 1);
                lastPos = pos;
            }

            return result;
        }

};