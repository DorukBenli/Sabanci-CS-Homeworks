/*
    Created by doruk benli at 22.04.2023
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <regex>
#include <fcntl.h>
using namespace std;


struct CommandValues {
    string field;
    string value;
};


void parseCommands(const string &line, string & correspondingInpFile, ofstream &outputFile, vector<vector<CommandValues>> & toExec) {
    istringstream ss(line);
    string word;
    regex txtFile(R"(\.txt)");

    vector<CommandValues> Data{
        //initialize this such that it appears in correct format.
        {"Command", ""},
        {"Inputs", ""},
        {"Options", ""},
        {"Redirection", "-"},
        {"Background Job", "n"},
    };

    while (ss >> word) {
       if (word == "ls" || word == "man" || word == "grep" || word == "wait" || word == "wc")
       {
        //then its command
            Data[0].value = word;
       }
       else if(word[0] == '-')
       {
        //means we have option
            Data[2].value = word;
       }
       else if ((word[0] == '<' || word[0] == '>')) 
       {
        //redirection
            Data[3].value = word[0];
       } 
       else if (word[0] == '&' && word.length() == 1) 
        {
            //background job or not.
            Data[4].value = "y";
        }
        else if(regex_search(word,txtFile))
        {
            //put the file to given string, searched by regex pattern.
            correspondingInpFile = word;
        }
        else
        {
            //input done in else since there is no identifier for distinguishing it.
            Data[1].value = word;
        }
    }
    for(int i= 0; i< Data.size();i++)
    {
        outputFile << Data[i].field << ": " << Data[i].value << endl;
    }
    outputFile << "----------" << endl;
    toExec.push_back(Data);
}

int main()
{
    vector<vector<CommandValues>> commands; // holds a vector of commands
    vector<pair<int,string>> actionFiles; // array which holds which command will write or read which file.
    ifstream commands_file;
    ofstream parsed_file("parse.txt");
    string line, fname;
    fname = "commands.txt";
    commands_file.open(fname.c_str());

    int ctr = 0;
    while (getline(commands_file, line)) {
        ctr += 1;
        string inpfname =""; // will have empty string if corresponding command has no filename
        parsed_file << "----------\n";
        parseCommands(line, inpfname,parsed_file,commands);
        actionFiles.push_back({ctr,inpfname});
    }
    
    parsed_file.close();
    commands_file.close();
    return 0;
}
