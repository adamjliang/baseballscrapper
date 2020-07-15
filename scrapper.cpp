#include <windows.h>
#include <tchar.h>
#include <wininet.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime> 
#include <stdio.h>
#include <time.h>
#include <array>
#include <vector>
#include <stdlib.h>
#include "Utility.h"

#pragma comment (lib, "wininet.lib")

using namespace std;


// MUST BE ON A WINDOWS COMPUTER IN ORDER FOR THIS TO WORK!


struct Player {
    string name;

};

class Entry {
public:
    string date;
    string homeTeam;
    string homeScore;
    string awayTeam;
    string awayScore;
    //string awayTeamRecord;
    //string homeTeamRecord;
    vector<Player> homeTeamSL;
    string homePitcher;
    string awayPitcher;
    vector<Player> awayTeamSL;


};

string convertToDate(int i, int j);

string PrePrep(int i, int j, int k, int p);

Entry getData(string x, int teamIndex);

ostream& operator<<(ostream& os, const Entry& e1)
{
    int spacesToFill;
    os << e1.date << " ";
    spacesToFill = 29 - e1.homeTeam.size();
    os << e1.homeTeam;
    for (int i = 0; i < spacesToFill; i++) {
        os << " ";
    }

    os << e1.homeScore;
    spacesToFill = 11 - e1.homeScore.size();
    for (int i = 0; i < spacesToFill; i++) {
        os << " ";
    }
    os << e1.awayTeam;
    spacesToFill = 29 - e1.awayTeam.size();
    for (int i = 0; i < spacesToFill; i++) {
        os << " ";
    }
    os << e1.awayScore;
    spacesToFill = 11 - e1.awayScore.size();
    for (int i = 0; i < spacesToFill; i++) {
        os << " ";
    }



    for (int i = 0; i < e1.awayTeamSL.size(); i++) {
        os << e1.awayTeamSL[i].name;
        spacesToFill = 27 - e1.awayTeamSL[i].name.size();
        for (int i = 0; i < spacesToFill; i++) {
            os << " ";
        }
    }
    //print home pitcher
    os << e1.homePitcher;
    spacesToFill = 27 - e1.homePitcher.size();
    for (int i = 0; i < spacesToFill; i++) {
        os << " ";
    }
    for (int i = 0; i < e1.homeTeamSL.size(); i++) {
        os << e1.homeTeamSL[i].name;
        spacesToFill = 27 - e1.homeTeamSL[i].name.size();
        for (int i = 0; i < spacesToFill; i++) {
            os << " ";
        }
    }
    //print away pitcher
    os << e1.awayPitcher;
    spacesToFill = 27 - e1.awayPitcher.size();
    for (int i = 0; i < spacesToFill; i++) {
        os << " ";
    }


    os << "\n";
    //ADD SPACES AND ADD THE VECTOR ENTRIES
    return os;
}



int main() {
    ofstream outFS;
    outFS.open("2019_season.txt");
    if (!(outFS.is_open())) {
        cout << " Not Opened!";
        return 0;
    }

    int percent = 0;
    int amtTrues = 0;
    vector<Entry> entries;
    Entry e;
    //cout << classAbrev[0];
    //***CHANGE TO WHATEVER YOU WOULD LIKE:
    // i - changes the month
    // j - changes the day
    // k - changes the home team search (specified by Utility.h)
    for (int i = 3; i < 11; i++) { // [mar, oct] or [mar, nov), 8
        for (int j = 1; j < 32; j++) { // [1-31] , 31
            for (int k = 0; k < 30; k++) { // 30
                if (j < 14 && i == 9) {
                    j = 14;
                }
                for (int p = 0; p < 1; p++) {
                    e = getData(PrePrep(i, j, k, p), k);
                    e.date = convertToDate(i, j);

                    if (e.homeTeam != "N/A") {
                        entries.push_back(e);
                        amtTrues++;
                    }
                    cout << "Sites Prepped: " << amtTrues << " ";
                    //system("CLS");
                    percent++;

                    cout << "Percent: " << floor((percent * 100) / 21750) << "%" << '\n';
                }
            }
        }

    }
    //cout << "Date:  Home Team:    Away Team:        Away Score:                   \n";
    //system("CLS");
    outFS << "Date Home Team                    Home Score Away Team                    Away Score A1                         A2                         A3                         A4                         A5                         A6                         A7                         A8                         A9                         HP                         H1                         H2                         H3                         H4                         H5                         H6                         H7                         H8                         H9                         AP                         \n";
    for (int o = 0; o < entries.size(); o++) {
        outFS << entries[o];
    }


    //for (int i = 0; i < 30; i++) { //size of ClassAbrev LPCSTR ARRAY.
    //    for (int j = 0; j < 30; j++) { //classes start at 100 and aren't above 1000
    //        cout << "Prepping Data... This may take a while.\n";
    //        //cout << "Prepping " << teamAbrev[j] << " data. (" << (j / 10) << "%)\n";
    //        //cout << "Full Prep: (" << percent / 2430 << "%)\n";
    //        //cout << "Sites Prepped: " << amtTrues;
    //        //percent++;
    //        getData(PrePrep(i, j));
    //        //system("CLS");
    //    }
    //}
    //cin.get();
    outFS.close();
    return 0;

}


Entry getData(string x, int teamIndex) {
    Entry e;
    if (x.size() == 0) {
        e.homeTeam = "N/A";
        return e;
    }

    bool keepGoing = true;
    bool isPitcher = false;
    bool inAwayTeamName = true; //starts true
    bool inAwayTeamScore = true; //starts true;
    string awayTeamName = "";
    string homeTeamName = "";

    string awayTeamScore = "";
    string homeTeamScore = "";
    int j = 0;
    int playersCount = 1;
    Player input;
    vector<Player> homeTeamStarters;
    vector<Player> awayTeamStarters;
    bool inAwayTeamStarters = true;

    //bool inHomeTeamNAme = false; //starts false, shouldnt need

    for (int i = 0; i < x.size(); i++) {
        if (x[i] == 'g' && keepGoing) {
            if (x.substr(i, 7) == "google ") {
                keepGoing = false;
            }

        }
        if (x[i] == '"' && keepGoing) {
            //cout << x.substr(i, 18);
            if (x.substr(i, 18) == "\" itemprop=\"name\">") {
                if (inAwayTeamName) {
                    j = 0;
                    while (x[i + 18 + j] != '<') {
                        awayTeamName += x[i + 18 + j];
                        j++;
                    }
                    inAwayTeamName = false;
                }
                else {
                    j = 0;
                    while (x[i + 18 + j] != '<') {
                        homeTeamName += x[i + 18 + j];
                        j++;
                    }
                }
            }
        }
        if (x[i] == 'r' && keepGoing) {
            if (x.substr(i, 4) == "re\">") {
                if (inAwayTeamScore) {
                    j = 0;
                    while (x[i + 4 + j] != '<') {
                        awayTeamScore += x[i + 4 + j];
                        j++;
                    }
                    inAwayTeamScore = false;
                }
                else {
                    j = 0;
                    while (x[i + 4 + j] != '<') {
                        homeTeamScore += x[i + 4 + j];
                        j++;
                    }
                }

            }
        }
        if (x[i] == '<' && keepGoing && playersCount < 10) {
            if (x.substr(i, 22) == "<td><a href=\"/players/") {
                input.name = "";
                isPitcher = false;
                j = 0;
                while (x[i + 39 + j] != '<') {
                    if (x[i + 39 + j] != '>' && x[i + 39 + j] != '"') {
                        input.name += x[i + 39 + j];
                    }
                    j++;
                }

                if (x.substr(i + 39 + j, 30).find('p') != std::string::npos) {
                    isPitcher = true;
                }

                if (inAwayTeamStarters) {
                    awayTeamStarters.push_back(input);
                    if (isPitcher) {
                        e.awayPitcher = input.name;
                    }

                }
                else {
                    homeTeamStarters.push_back(input);
                    if (isPitcher) {
                        e.homePitcher = input.name;
                    }
                }
                playersCount++;
                if (playersCount > 9 && teamIndex > 14) {
                    playersCount = 1;
                    inAwayTeamStarters = false;

                }


            }
        }
        if (x[i] == '<' && keepGoing && playersCount == 10) {
            if (x.substr(i, 25) == "<td>   <a href=\"/players/") {
                input.name = "";
                j = 0;
                while (x[i + 43 + j] != '<') {
                    if (x[i + 43 + j] != '>') {
                        input.name += x[i + 43 + j];
                    }
                    j++;
                }

                if (inAwayTeamStarters) {
                    e.awayPitcher = input.name;
                }
                else {
                    e.homePitcher = input.name;
                }
                playersCount++;
                playersCount = 1;
                inAwayTeamStarters = false;


            }
        }

    }

    e.awayScore = awayTeamScore;
    e.awayTeam = awayTeamName;
    e.homeScore = homeTeamScore;
    e.homeTeam = homeTeamName;
    e.homeTeamSL = homeTeamStarters;
    e.awayTeamSL = awayTeamStarters;
    return e;


}

string convertToDate(int i, int j) {
    string returnString = "";
    if (i < 10) {
        returnString += "0";
    }
    returnString += to_string(i);
    if (j < 10) {
        returnString += "0";
    }
    returnString += to_string(j);

    return returnString;
}

string PrePrep(int i, int j, int k, int p) {
    string total;
    DWORD bytesRead = 0;
    DWORD totalBytesRead = 0;

    string subURL = "/boxes/";
    subURL += teamAbrev[k];
    subURL += "/";
    subURL += teamAbrev[k];
    subURL += "2019";
    //date here
    subURL += convertToDate(i, j);
    subURL += to_string(p);
    subURL += ".shtml";
    //subURL += to_string(j);
    //subURL += "/";
    LPCSTR cSubURL = subURL.c_str();
    //cout << cSubURL;
    HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    /*if (hInternet == NULL)
        return 3;*/
        //Error(_T("Error opening internet."));

    HINTERNET hConnection = InternetConnectA(hInternet, "baseball-reference.com", INTERNET_DEFAULT_HTTP_PORT, "", "", INTERNET_SERVICE_HTTP, INTERNET_SERVICE_FTP, 0); //enter url here
    /*if (hConnection == NULL)
        return 4;*/
        //Error(_T("Error opening internet connection"));
    HINTERNET hData = HttpOpenRequestA(hConnection, "GET", cSubURL, NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);
    /*if (hData == NULL)
        return 5;*/
        //Error(_T("Error opening internet request"));

    char buf[2048];

    BOOL success = HttpSendRequestA(hData, NULL, 0, NULL, 0);
    /*   if (!success)
           return 6;*/
           //Error(_T("Error sending request"));
    total = ""; // total is the string that is couted at the end to output the source code.
    bytesRead = 0;
    totalBytesRead = 0;

    while (InternetReadFile(hData, buf, 2019, &bytesRead) && bytesRead != 0)
    {
        buf[bytesRead] = 0; // insert the null terminator.
        total += buf;
        totalBytesRead += bytesRead;
    }
    for (int i = 0; i < (int)total.size(); i++) {
        total[i] = tolower(total[i]);
    }

    if (total.find("apologize") != std::string::npos) {
        return "";
    }
    //else if (total.find("uniqname") != std::string::npos) {
    //    return false;
    //}
    //else if ((total.find("\<h5 class=\"text-small blue-highlight-text\"\>") != std::string::npos)) {

    //    return false;
    //}
    else {
        //cout << total;
        //Sleep(100000);
        return total;
    }



    InternetCloseHandle(hData);
    InternetCloseHandle(hConnection);
    InternetCloseHandle(hInternet);
    //cin.get();
}