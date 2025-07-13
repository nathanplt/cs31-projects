//
//  main.cpp
//  Project 3
//
//  Created by Nathan Zhang on 10/26/24.
//

#include <iostream>
#include <string>
using namespace std;



// Return true if the argument is a two-uppercase-letter state code, or
// false otherwise.
bool isValidState(string stateCode) {
    const string codes =
        "AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
        "KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NJ.NH.NM.NY.NC."
        "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2  &&
            stateCode.find('.') == string::npos  &&  // no '.' in stateCode
            codes.find(stateCode) != string::npos);  // match found
}

// Convert string number to int
int string_to_int(string s) {
    int x = 0;
    for (int i=0; i!=s.size(); i++) {
        // Next digit, so multiply by 10
        x *= 10;
        
        // Add the int value of the char digit
        x += s.at(i) - '0';
    }
    return x;
}

bool hasRightSyntax(string poll_data) {
    int pos = 0;
    while (pos != poll_data.size()) {
        char party_code = poll_data.at(pos);
        
        // Check if the party code is a letter
        // If it's not, return false
        if (!isalpha(party_code)) {
            return false;
        }
        pos++;
        
        // Parse vote count as string
        string vote_count_s = "";
        while (pos != poll_data.size() && isdigit(poll_data.at(pos))) {
            vote_count_s += poll_data.at(pos);
            pos++;
        }
        
        // If the vote count string is empty, return false
        if (vote_count_s == "") {
            return false;
        }
        
        // Convert vote count string to int
        int vote_count = string_to_int(vote_count_s);
        
        // If the vote count isn't a positive two-digit integer or zero, return false
        if (vote_count < 0 || vote_count > 99) {
            return false;
        }
        
        
        string state_code;
        
        // If there is not enough letters for a state code, return false
        if (pos >= poll_data.size() - 1) {
            return false;
        }
        
        
        state_code = poll_data.substr(pos, 2);
        
        // toupper() is used to account for the fact that lower-case state codes are also valid
        state_code[0] = toupper(state_code[0]);
        state_code[1] = toupper(state_code[1]);
        
        // If the state code is invalid, return false
        if (!isValidState(state_code)) {
            return false;
        }
        
        // Move onto the next state prediction
        pos += 2;
    }
    
    // If no errors were found, the poll data is valid
    return true;
}

int computeVotes(string poll_data, char party, int& vote_count) {
    // If the poll data isn't valid, return 1
    if (!hasRightSyntax(poll_data)) {
        return 1;
    }
    
    // If party isn't a letter, return 3
    if (!isalpha(party)) {
        return 3;
    }
    
    int pos = 0;
    int poll_votes = 0;
    while (pos != poll_data.size()) {
        char party_code = poll_data.at(pos);
        pos++;
        
        // Parse the state's vote count as a string
        string state_votes_s = "";
        while (isdigit(poll_data.at(pos))) {
            state_votes_s += poll_data.at(pos);
            pos++;
        }
        
        // convert vote count string to int
        int state_votes = string_to_int(state_votes_s);
        
        // If electoral votes is zero, return 2
        if (state_votes == 0){
            return 2;
        }
        
        // toupper() is used to account for how case in a party code doesn't matter
        if (toupper(party_code) == toupper(party)){
            poll_votes += state_votes;
        }
        
        // Move onto the next state prediction as we don't really care what state the votes come from
        pos += 2;
    }
    
    // If all works out, set the vote count to the number of votes from the poll data
    vote_count = poll_votes;
    return 0;
}

int main(int argc, const char * argv[]) {
    for (;;) {
        cout << "Enter poll data string: ";
        string pds;
        getline(cin, pds);
        
        if (pds == "quit")
            break;
        
        cout << "Enter party code: ";
        string party_code;
        getline(cin, party_code);
        
        int vote_count = -50000;
        int return_val = computeVotes(pds, party_code.at(0), vote_count);
        cout << "Returned " << return_val << '\n';
        cout << "Vote count is now " << vote_count << '\n';
            
        cout << endl;
    }
    return 0;
}

