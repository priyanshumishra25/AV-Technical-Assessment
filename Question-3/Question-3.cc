#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Function to extract data from CAN message
void parseCANLog(const string &inputFile, const string &outputFile) {
    ifstream infile(inputFile);
    ofstream outfile(outputFile);

    if (!infile.is_open()) {
        cerr << "Error: Unable to open input file!" << endl;
        return;
    }
    if (!outfile.is_open()) {
        cerr << "Error: Unable to open output file!" << endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string timestamp, can_device, can_data;
        
        // Extract timestamp, CAN device, and CAN message
        ss >> timestamp >> can_device >> can_data;

        // Check if the CAN message contains '#'
        size_t hashPos = can_data.find('#');
        if (hashPos == string::npos) continue;

        // Extract CAN ID and Data
        string can_id_hex = can_data.substr(0, hashPos);
        string data_hex = can_data.substr(hashPos + 1);

        // Convert CAN ID from hex to decimal
        unsigned int can_id;
        stringstream hexStream;
        hexStream << hex << can_id_hex;
        hexStream >> can_id;

        // Only process messages with CAN ID 0x705 (decimal 1797)
        if (can_id == 1797 && data_hex.length() >= 16) {
            // Convert hex string to bytes
            unsigned char bytes[8];
            for (int i = 0; i < 8; ++i) {
                bytes[i] = stoi(data_hex.substr(i * 2, 2), nullptr, 16);
            }

            // Extract 16-bit WheelSpeedRR (Little-Endian)
            uint16_t rawValue = bytes[4] | (bytes[5] << 8);

            // Apply scaling factor (0.1)
            double wheelSpeedRR = rawValue * 0.1;

            // Write output
            outfile << "(" << timestamp.substr(1, timestamp.length() - 2) << "): " 
                    << fixed << setprecision(1) << wheelSpeedRR << endl;
        }
    }

    infile.close();
    outfile.close();
    cout << "Decoding complete! Check output.txt for results." << endl;
}

int main() {
    parseCANLog("../Question-3/candump.log", "../Question-3/output.txt");
    return 0;
}
