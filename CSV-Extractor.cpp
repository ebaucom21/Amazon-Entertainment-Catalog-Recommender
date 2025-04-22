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
            return extractDataHelper("../amazon_prime_titles.csv");
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
                if (line[0] != 's') continue; // Skip the line if it doesn't start with a show_id
                vector<vector<string>> row(6); // 6 columns
                string cell;
                size_t pos = 0;
                int lastPos = 0;
                // discard show_id
                getCell(line, lastPos); 
                // Get type
                row[1] = getCell(line, lastPos);
                // Get title
                row[0] = getCell(line, lastPos);
                // Get actors/directors
                // Get director first
                row[2] = getCell(line, lastPos);
                // Get the actors inside of the quotes that are seperated by commas
                vector<string> temp = getCell(line, lastPos);
                temp.insert(temp.begin(), row[2][0]); // Add the director to the front of the vector
                row[2] = temp;
                
                // Skip country
                getCell(line, lastPos);
                // Skip date_added
                getCell(line, lastPos);
                // Skip release_year
                getCell(line, lastPos);
                // Get rating
                row[3] = getCell(line, lastPos);
                // Get duration
                row[4] = getCell(line, lastPos);
                // Get genres the same as actors
                row[5] = getCell(line, lastPos);
                // Skip description
                getCell(line, lastPos);

                result.push_back(row); // Add the row to the result
            }

            return result;
        }
        vector<string> getCell(const string& line, int& lastPos) {
            vector<string> cells;
            if (line.find('\"', lastPos + 1) == lastPos + 1) {
                int pos = line.find('\"', lastPos + 1);
                lastPos = pos;
                pos = line.find('\"', lastPos + 1);
                string cell = line.substr(lastPos + 1, pos - lastPos - 1);
                int lastPos2 = 0;
                int pos2 = 0;
                pos2 = cell.find(',', lastPos2 + 1);
                string curInd = cell.substr(lastPos2, pos2 - lastPos2);
                cells.push_back(curInd);
                lastPos2 = pos2 + 1;
                while (cell.find(',', lastPos2+1) != string::npos) {
                    pos2 = cell.find(',', lastPos2 + 1);
                    // Remove the space at the start of these
                    string curInd = cell.substr(lastPos2+1, pos2 - lastPos2-1);
                    cells.push_back(curInd);
                    lastPos2 = pos2 + 1;
                }
                if (lastPos2 < cell.size()) {
                    curInd = cell.substr(lastPos2+1, cell.size() - lastPos2-1);
                    cells.push_back(curInd);
                    lastPos = pos + 1;
                }
            } else {
                // If the cell is not in quotes, find the next comma
                size_t pos = line.find(',', lastPos + 1);
                string cell = line.substr(lastPos + 1, pos - lastPos - 1);
                cells.push_back(cell);
                lastPos = pos;
            }

            // Replacements that need to be checked
            for (auto& cell : cells) {
                if (cell == "Action") cell = "Action and adventure";
                else if (cell == "Suspense") cell = "Mystery and thrillers";
                else if (cell == "Science Fiction") cell = "Science fiction";
                else if (cell == "UNRATED") cell = "18+";
                else if (cell == "R") cell = "16+";
                else if (cell == "PG-13") cell = "13+";
                else if (cell == "PG") cell = "7+";
                else if (cell == "NR") cell = "18+";
                else if (cell == "NOT-RATE") cell = "18+";
                else if (cell == "NC-17") cell = "16+";
                else if (cell == "G") cell = "All";
                else if (cell == "ALL") cell = "All";
                else if (cell == "AGES_18_") cell = "18+";
                else if (cell == "TV Show") cell = "Shows";
                // TODO: Fix for time ranges
                else if (cell.find("min") != string::npos) {
                    cell = cell.substr(0, cell.find("min")-1);
                    int temp = 0;
                    try {
                        temp = stoi(cell);
                    } catch (const std::invalid_argument& e) {
                        // Handle the case where conversion fails
                        cell = "Unknown";
                        continue;
                    }
                    if (temp < 60) cell = "<1hr";
                    else if (temp <= 90) cell = "1-1.5hrs";
                    else if (temp <= 120) cell = "1.5-2hrs";
                    else cell = ">2hrs";
                }
            }

            return cells;
        }
};