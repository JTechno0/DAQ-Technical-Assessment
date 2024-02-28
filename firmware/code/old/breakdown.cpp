#include <iostream>
#include <bits/stdc++.h>
#include <sstream>
#include <fstream>

using namespace std;
//Endianess of system is little Endian
bool isBigEndian(){
    uint16_t word = 1; // 0x0001
    uint8_t *first_byte = (uint8_t*) &word; // points to the first byte of word
    return !(*first_byte); // true if the first byte is zero
}
int main(){
    string parseString;
    int limit = 0;

    // Read from the text file
    ifstream MyReadFile("../dump.log");
    while (getline (MyReadFile, parseString)) {
        int startbr = parseString.find('(');
        parseString = parseString.substr(startbr+1, -1);
        int endbr = parseString.find(')');
        string timestamp = parseString.substr(0, endbr);

        parseString = parseString.substr(endbr+1, -1);
        int startsp = parseString.find(' ');
        parseString = parseString.substr(startsp+1, -1);
        int endsp = parseString.find(' ');
        string interface = parseString.substr(0, endsp);
        parseString = parseString.substr(endsp+1, -1);

        int splitid = parseString.find('#');
        string id = parseString.substr(0, splitid);
        parseString = parseString.substr(splitid+1, -1);

        string data = parseString;

        std::string s = id;
        
        unsigned int x = std::stoul(s, nullptr, 16);

        if (x == 1797)
        {
            string FRSpeedHex1 = parseString.substr(0,2);
            string FLSpeedHex1 = parseString.substr(4,2);
            string RRSpeedHex1 = parseString.substr(8,2);
            string RLSpeedHex1 = parseString.substr(12,2);
            string FRSpeedHex2 = parseString.substr(2,2);
            string FLSpeedHex2 = parseString.substr(6,2);
            string RRSpeedHex2 = parseString.substr(10,2);
            string RLSpeedHex2 = parseString.substr(14,2);

            string FRSpeedHex = FRSpeedHex2 + FRSpeedHex1;
            string FLSpeedHex = FLSpeedHex2 + FLSpeedHex1;
            string RRSpeedHex = RRSpeedHex2 + RRSpeedHex1;
            string RLSpeedHex = RLSpeedHex2 + RLSpeedHex1;
            unsigned int FRSpeed = std::stoul(FRSpeedHex, nullptr, 16);
            unsigned int FLSpeed = std::stoul(FLSpeedHex, nullptr, 16);
            unsigned int RRSpeed = std::stoul(RRSpeedHex, nullptr, 16);
            unsigned int RLSpeed = std::stoul(RLSpeedHex, nullptr, 16);
            string outputone = "(" + timestamp + ")" + " : " + "WheelSpeedFR: " + std::to_string(FRSpeed*0.1); 
            string outputtwo = "(" + timestamp + ")" + " : " + "WheelSpeedFL: " + std::to_string(FLSpeed*0.1); 
            string outputthree = "(" + timestamp + ")" + " : " + "WheelSpeedRR: " + std::to_string(RRSpeed*0.1); 
            string outputfour = "(" + timestamp + ")" + " : " + "WheelSpeedRL: " + std::to_string(RLSpeed*0.1); 
            cout << outputone << "\n";
            cout << outputtwo << "\n";
            cout << outputthree << "\n";
            cout << outputfour << "\n" << "\n";
            limit = limit + 1;
        }
        
        /*cout << timestamp << ' '; 
        cout << interface << ' ';
        cout << id << ' ';
        cout << data << ' ';
        cout << x;
        cout << FRSpeedHex;
        cout << FLSpeedHex;
        cout << RRSpeedHex;
        cout << RLSpeedHex;*/
    }
    MyReadFile.close();
    //cout << (isBigEndian()?"Big-Endian":"Little-Endian") << endl;
    return 0;
}
