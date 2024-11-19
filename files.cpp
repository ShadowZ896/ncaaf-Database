#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

class program{
    public:
        string teamName, abbrev, mascot, location, conf;
        int inagSeason, wins, losses, ties;
        void set_property(int toSet, string prop);
        void set_property(int toSet, int prop);
        void printTeamInfo();
};

bool isNumeric(string &line)
{
    for (char c : line) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
  return true;
}

void program::set_property(int toSet, string prop){
    switch (toSet){
        case 1:
            teamName = prop;
        case 2:
            abbrev = prop;
        case 3:
            mascot = prop;
        case 4:
            location = prop;
        case 5:
            conf = prop;
    }  
}

void program::set_property(int toSet, int prop){
    switch (toSet){
        case 1:
            inagSeason = prop;
        case 2:
            wins = prop;
        case 3:
            losses = prop;
        case 4:
            ties = prop;
    }
    
}

void createTeams(string line, map<string, program> *teams, ifstream &ncaaf){
    program currTeam;
    string oldVal, toApp = " ";
    int curr_val = 0, strVals = 0, intVals = 0;
    char delimiter = ',';

    while (getline(ncaaf, line, delimiter) && strVals < 5 && intVals < 4){
        //Keep the location as one value ("City, State")
        //Conference comes out to location value instead.
        //Problem stems from end of parsing at the very end with the "conf" value.
        if(!line.empty()){
            if (line[0] == '"'){
                oldVal = line;
                getline(ncaaf, line, delimiter);
                line = oldVal + "," + line;
                strVals++;
                currTeam.set_property(strVals, line);
            } else if (intVals == 1){
                delimiter = '-';
                intVals++;
                stringstream record(line);
                for (; intVals <= 4; intVals++){
                    getline(record, line, delimiter);
                    currTeam.set_property(intVals, stoi(line));
                } 
                delimiter = '\n';
            } else {
                if (isNumeric(line)){
                    intVals++;
                    currTeam.set_property(intVals, stoi(line));
                } else {
                    strVals++;
                    currTeam.set_property(strVals, line);
                }
            }
        }
    }
    strVals++;
    currTeam.set_property(strVals, line);
    (*teams).insert(std::pair<string, program>(currTeam.teamName, currTeam));
    return;
}

void program::printTeamInfo(){
    //string teamName, abbrev, mascot, location, conf;
    //int inagSeason, wins, losses, ties;
    cout << "\nTeam: " << teamName << "\nAcronym: " << abbrev << "\nLocated in " << location << "\n";
    cout << "The " << mascot << " play out of the " << conf << 
    " conference, and their program was born in the year " << inagSeason << ".\n";
    printf("They have an all time record of %d-%d-%d (Wins-Losses-Ties).\n", wins, losses, ties); 
}

int main(){
    // Some placeholder variables, our hash map for all of the teams,
    // and finally our file that we are reading from.
    string myText = " ", toView = " ", line = " ";
    map<string, program> *teams = new map<string, program>();
    ifstream ncaaf("ncaaf.csv");

    //Account for an error opening the file.
    if (!ncaaf){return -1;}

    //This getline is just to "scratch" the line with all the labels.
    getline(ncaaf, line);

    while(!ncaaf.eof()){
        createTeams(myText, teams, ncaaf);
    } 
    ncaaf.close();

    //User interaction begins
    cout << "What team's info do you want to see: ";
    getline(cin, toView);
    while(teams->find(toView) == teams->end()){
        cout << "That team is not present in the database. Please search for a valid team: ";
        getline(cin, toView);
    }
    (*teams)[toView].printTeamInfo();

    return 0;
}
