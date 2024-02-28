#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <list>
using namespace std;

//all the strings that hold data
std::list<string> sendID;
std::list<string> sendName;
std::list<string> sendLength;
std::list<string> RSyntax;
std::list<string> RName;
std::list<string> RBitS;
std::list<string> RBitL;
std::list<string> REndian;
std::list<string> Rsigned;
std::list<string> Rscale;
std::list<string> Roffset;
std::list<string> Rmin;
std::list<string> Rmax;
std::list<string> Runit;
std::list<string> DATAs;
std::list<list<string>> Outputs;

//decodes SensorBus.dbc so we can apply its rules with new data
void Translate() {
    //opening file stream
    ifstream SensorFile("../SensorBus.dbc");
    string SensorText;
    string BOScrub;
    string OTScrub;
    bool hitBO = false;
    while (getline (SensorFile, SensorText)) {
        //finding if a line contains the rules to break down data
        if (SensorText.find("BO_") == 0)
        {
            //breaking down the line and saving the relevant data
            BOScrub = SensorText;
            sendID.push_back(BOScrub.substr(BOScrub.find(' ')+1,-1).substr(0,BOScrub.substr(BOScrub.find(' ')+1,-1).find(' ')));
            BOScrub = BOScrub.substr(BOScrub.find(' ')+1+BOScrub.substr(BOScrub.find(' ')+1,-1).find(' ')+1,-1);
            sendName.push_back(BOScrub.substr(0,BOScrub.find(":")));
            BOScrub = BOScrub.substr(BOScrub.find(":")+2,-1);
            sendLength.push_back(BOScrub.substr(0,BOScrub.find(' ')));
            hitBO = true;
        } else if (hitBO && SensorText.empty())
        {
            //if you hit an empty line then stop decoding the rules also remove and extra 00 that was placed
            sendID.pop_back();
            hitBO = false;
            
        } else if (hitBO) 
        {
            //starts to read every line after BO_, these describe how to break apart the hex string
            OTScrub = SensorText;
            RSyntax.push_back(OTScrub.substr(1, OTScrub.find('_')-1));
            OTScrub = OTScrub.substr(OTScrub.find('_')+1,-1);
            RName.push_back(OTScrub.substr(OTScrub.find(' ')+1,-1).substr(0,OTScrub.substr(OTScrub.find(' ')+1,-1).find(' ')));
            OTScrub = OTScrub.substr(OTScrub.find(' ')+1+OTScrub.substr(OTScrub.find(' ')+1,-1).find(' ')+1,-1);
            RBitS.push_back(OTScrub.substr(OTScrub.find(":")+2, -1).substr(0,OTScrub.find("|")-2));
            OTScrub = OTScrub.substr(OTScrub.find('|')+1,-1);
            RBitL.push_back(OTScrub.substr(0,OTScrub.find("@")));
            OTScrub = OTScrub.substr(OTScrub.find("@")+1,-1);
            
            REndian.push_back(OTScrub.substr(0,1));
            OTScrub = OTScrub.substr(1,-1);
            Rsigned.push_back(OTScrub.substr(0,1));
            OTScrub = OTScrub.substr(1,-1);
            Rscale.push_back(OTScrub.substr(OTScrub.find('(')+1,OTScrub.substr(OTScrub.find('(')+1,-1).find(",")));
            OTScrub = OTScrub.substr(OTScrub.find(",")+1,-1);
            Roffset.push_back(OTScrub.substr(0,OTScrub.find(")")));
            OTScrub = OTScrub.substr(OTScrub.find(")")+2,-1);
            Rmin.push_back(OTScrub.substr(OTScrub.find("[")+1,OTScrub.substr(OTScrub.find("[")+1,-1).find("|")));
            OTScrub = OTScrub.substr(OTScrub.find("|")+1,-1);
            Rmax.push_back(OTScrub.substr(0,OTScrub.find("]")));
            OTScrub = OTScrub.substr(OTScrub.find("]")+1,-1);
            Runit.push_back(OTScrub.substr(OTScrub.find('"')+1,OTScrub.substr(OTScrub.find('"')+1,-1).find('"')));
            sendID.push_back("00");
        }
        
    }
    SensorFile.close();
}

//a moves a object from the front to the back of a list
std::list<string> cycle(std::list<string> a)
{
    string FV = a.front();
    a.pop_front();
    a.push_back(FV);
    return a;

}
//calls cycle for every list
void callCycle()
{
    sendID = cycle(sendID);
    sendName = cycle(sendName);
    sendLength = cycle(sendLength);
    RSyntax = cycle(RSyntax);
    RName = cycle(RName);
    RBitS = cycle(RBitS);
    RBitL = cycle(RBitL);
    REndian = cycle(REndian);
    Rsigned = cycle(Rsigned);
    Rscale = cycle(Rscale);
    Roffset = cycle(Roffset);
    Rmin = cycle(Rmin);
    Rmax = cycle(Rmax);
    Runit = cycle(Runit);
}

int main() {
    //calls Translate
    Translate();
    //starts to read the log file
    ifstream LogFile("../dump.log");
    ofstream WriteFile("../output.txt");
    string LogText;
    string TimeStamp;
    string Interface;
    string LogID;
    string data;
    std::list<int>::iterator it;
    std::list<int>::iterator it2;

    while (getline (LogFile, LogText)) 
    {
        //breakes apart log file data to gain relevant data
        string ParseString = LogText;
        TimeStamp = ParseString.substr(ParseString.find("("),ParseString.find(")")+1);
        ParseString = ParseString.substr(ParseString.find(")")+1,-1);
        Interface = ParseString.substr(ParseString.find(" ")+1,ParseString.substr(ParseString.find(" ")+1,-1).find(" "));
        ParseString = ParseString.substr(ParseString.find(" ")+ParseString.substr(ParseString.find(" ")+1,-1).find(" ")+2,-1);
        LogID = ParseString.substr(0,ParseString.find("#"));
        ParseString = ParseString.substr(ParseString.find("#")+1,-1);
        data = ParseString;
        
        //translates the id from logs Hex to dbc's decimal
        string TransID = to_string(stoul(LogID, nullptr, 16));
        bool hitline = false;
        bool found = false;
        int BOPosC = 0;
        int BOPos;
        int SGPos = 0;

        for(int i=0; i < sendID.size(); i++ )
        {
            //goes through the dbc ids and finds where its equal to log string id
            string FV = sendID.front();
            if(hitline)
            {
                //saves the position of the sub rules
                if(sendID.front() == "00")
                {
                    SGPos = SGPos + 1;
                } else {
                    hitline = false;
                }
            }
            if(FV==TransID)
            {
                //saves the position of the ruleset
                hitline = true;
                BOPos = BOPosC;
                found = true;
            }
            sendID.pop_front();
            sendID.push_back(FV);
            BOPosC = BOPosC + 1;

        }
        if(found)
        {
            //uses the rules from the dbc to translate the log string data
            int position;
            int length;
            bool bendie;
            bool signd;
            for(int i=0; i < BOPos; i++)
            {
                callCycle();
                
            }
            for(int i=0; i<SGPos+1; i++)
            {
                //reads the sub rules and specifies how to read data
                if(REndian.front()=="0")
                {
                    bendie = true;
                } else {
                    bendie = false;
                }
                if(Rsigned.front()=="+")
                {
                    signd = false;
                } else {
                    signd = true;
                }
                if(RBitS.front() != "0")
                {
                    position = stoi(RBitS.front())/4;
                } else {
                    position = 0;
                }
                //translates the specified length in binary to the hex length of log data
                if(RBitL.front() != "0")
                {
                    length = stoi(RBitL.front())/4;
                } else {
                    length = 0;
                }
                string subdata;
                string ReData;
                string output;
                int value;
                unsigned int uvalue;
                bool outRange = false;
                subdata = data.substr(position,length);
                
                for(int x=0; x<length/2;x++)
                {
                    //removes the part of the data that the sub rule referes to
                    DATAs.push_back(subdata.substr(0,2));
                    if (length/2-x != 1)
                    {
                        subdata = subdata.substr(2,-1);
                    }
                }
                if (bendie)
                {
                    //rearranges string if its endiness is big
                    for(int x=0;x<DATAs.size()+1;x++)
                    {
                        
                        if (x==0)
                        {
                            ReData = DATAs.back();
                            
                        } else{
                            ReData = ReData + DATAs.back();
                        }
                        DATAs.pop_back();
                        
                    }        
                }
                if (signd)
                {
                    //option for signed verison of int
                    float valuef;
                    value = std::stoul(ReData, nullptr, 16);
                    value = value + stoi(Roffset.front());
                    valuef = value*stof(Rscale.front());
                    stringstream s;
                    s<<valuef;
                    string valuefR = s.str();
                    if (valuef>=stoi(Rmin.front()) && valuef<=stoi(Rmax.front()) || Rmin.front()==Rmax.front())
                    {
                        outRange = false;
                    } else {
                        outRange = true;
                    }
                    if (!outRange){
                        output = TimeStamp + ": " + RName.front() + ": " + valuefR;
                    } else {
                        output = TimeStamp + ": " + RName.front() + ": " + valuefR + " error out of range";
                    }
                } else {
                    //option for unsigned verison of int
                    float uvaluef;
                    uvalue = std::stoul(ReData, nullptr, 16);
                    uvalue = uvalue + stoi(Roffset.front());
                    uvaluef = uvalue*stof(Rscale.front());
                    stringstream s;
                    s<<uvaluef;
                    string uvaluefR = s.str();
                    if (uvaluef>=stoi(Rmin.front()) && uvaluef<=stoi(Rmax.front()) || Rmin.front()==Rmax.front())
                    {
                        outRange = false;
                    } else {
                        outRange = true;
                    }
                    //checks if data value is out of range 
                    if (!outRange){
                        output = TimeStamp + ": " + RName.front() + ": " + uvaluefR;
                    } else {
                        output = TimeStamp + ": " + RName.front() + ": " + uvaluefR + " error out of range";
                    }
                
                }
                //uses an array to hold ouputs
                if (RName.front()=="WheelSpeedFR" ||RName.front()=="WheelSpeedRR")
                {
                    std::list<string> push;
                    push.push_back(RBitS.front());
                    push.push_back(output);
                    Outputs.push_back(push);
                    
                }
                //at the end of the data read arranges data from start to end then writes it to output file
                if (i == SGPos)
                {
                    Outputs.sort();
                    //cout << Outputs.size();
                    for (int m=0; m<Outputs.size()+1;m++)
                    {
                        WriteFile << Outputs.front().back() << '\n';
                        Outputs.pop_front();
                    }
                    Outputs.clear();
                    
                }
                //rotates the arrays position
                callCycle();
            }
            for (int i=0; i < sendID.size()-BOPos+SGPos+1; i++)
            {
                callCycle();
            }
        }
    }
    LogFile.close();
    WriteFile.close();
}