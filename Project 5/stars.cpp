#include "utilities.h"
#include <cstring>
#include <iostream>
using namespace std;

const char WORDFILENAME[] = "/Users/nathan/CS31 Projects/Project 5/words.txt";
const int MAXWORDS = 10000;
const int MAXWORDLENGTH = 6;

int runOneRound(const char words[][MAXWORDLENGTH+1], int nWords, int wordnum) {
    // If nWords isn't positive, return -1
    if (nWords <= 0) {
        return -1;
    }
    
    // If wordnum is not a valid index of words, return -1
    if (wordnum < 0 || wordnum >= nWords) {
        return -1;
    }
    
    char secretWord[MAXWORDLENGTH+1];
    strcpy(secretWord, words[wordnum]);
    int secretLen = strlen(secretWord);
    
    int attempts = 0;
    for (;;) {
        cout << "Trial word: ";
        char trialWord[102];
        cin.getline(trialWord, 101);
        
        int trialLen = strlen(trialWord);
        
        bool validWord = true;
        for (int i=0; i<trialLen; i++) {
            // If one of the characters is not a lowercase letter, trialWord is not valid
            if (!isalpha(trialWord[i]) || !islower(trialWord[i])) {
                validWord = false;
            }
        }
        
        // If trialWord is not valid or its length is not between 4 and 6, output an error and skip to next guess
        if (!validWord || trialLen < 4 || trialLen > 6) {
            cout << "Your trial word must be a word of 4 to 6 lower case letters." << endl;
            continue;
        }
        
        // Check if any of the words in "words" match the trialWord
        bool foundWord = false;
        for (int i=0; i<nWords; i++) {
            if (strcmp(words[i], trialWord) == 0) {
                foundWord = true;
            }
        }
        
        // If none of the words match, output an error and skip to next guess
        if (!foundWord) {
            cout << "I don't know that word." << endl;
            continue;
        }
        
        // If the trialWord is equal to the secretWord, break out of the loop
        if (strcmp(trialWord, secretWord) == 0) {
            attempts++;
            break;
        }
        
        // Intialize a letter (a to z) frequency count array for both trialWord and secretWord
        int trialCharFreq[26];
        int secretCharFreq[26];
        
        for (int i=0; i<26; i++) {
            trialCharFreq[i] = 0;
            secretCharFreq[i] = 0;
        }
        
        // Store the frequency of each character (a to z) in trialWord
        for (int i=0; i<trialLen; i++) {
            char curr = trialWord[i];
            
            // 'a' (- 'a') has index 0, 'b' (- 'a') has index 1 ... 'z' (- 'a') has index 25
            trialCharFreq[curr - 'a']++;
        }
        
        // Do the same for secretWord
        for (int i=0; i<secretLen; i++) {
            char curr = secretWord[i];
            secretCharFreq[curr - 'a']++;
        }
        
        int nStars = 0;
        
        // Count the number of stars first as they take priority over planets
        // Iterate up to the length of the shorter word to avoid accessing an index out of bounds
        for (int i=0; i<min(trialLen, secretLen); i++) {
            if (trialWord[i] == secretWord[i]) {
                nStars++;
                
                // To prevent counting this equality as a planet, decrement the char freq for both words
                char curr = trialWord[i];
                trialCharFreq[curr - 'a']--;
                secretCharFreq[curr - 'a']--;
            }
        }
        
        int nPlanets = 0;
        
        // Iterate over the char frequencies for both words
        for (int i=0; i<26; i++) {
            /*
             Taking the minimum of these two frequencies accounts for all cases:
             
             If at least one of the frequencies is zero, that means the current letter is not in both
             the trialWord and the secretWord and thus, no planets should be added to the count
             
             If both frequencies are positive, that means the current letter is in both the trialWord
             and the secretWord, and the amount of planets will be the minimum of these two frequencies
             as any extra counts of that letter in one of the strings are not considered planets
            */
            nPlanets += min(trialCharFreq[i], secretCharFreq[i]);
        }
        
        cout << "Stars: " << nStars << ", Planets: " << nPlanets << endl;
        attempts++;
    }
    
    return attempts;
}

int main()
{
    char words[MAXWORDS][MAXWORDLENGTH+1];
    int nWords = getWords(words, MAXWORDS, WORDFILENAME);

    if (nWords < 1) {
        cout << "No words were loaded, so I can't play the game." << endl;
        return 1;
    }
    
    cout << "How many rounds do you want to play? ";
    
    int nRounds;
    cin >> nRounds;
    cin.ignore(10000, '\n');
    
    if (nRounds <= 0) {
        cout << "The number of rounds must be positive." << '\n';
        return 1;
    }
    
    double averageScore = 0;
    int minScore = -1;
    int maxScore = -1;
    int sumScore = 0;
    
    for (int i=1; i<=nRounds; i++) {
        cout << endl;
        cout << "Round " << i << endl;
        
        int randIdx = randInt(0, nWords - 1);
        cout << "The secret word is " << strlen(words[randIdx]) << " letters long." << endl;
        
        int currScore = runOneRound(words, nWords, randIdx);
        
        if (currScore == -1) {
            cout << "An error occurred. Please try again." << endl;
            return 1;
        }
        
        sumScore += currScore;
        averageScore = (double) sumScore / i;
        
        if (minScore == -1) {
            minScore = currScore;
        } else {
            minScore = min(minScore, currScore);
        }
        
        if (maxScore == -1) {
            maxScore = currScore;
        } else {
            maxScore = max(maxScore, currScore);
        }
        
        // All decimal outputs will be shown with 2 decimal places
        cout.setf(ios::fixed);
        cout.precision(2);
        
        if (currScore == 1) {
            cout << "You got it in 1 try." << endl;
        } else {
            cout << "You got it in " << currScore << " tries." << endl;
        }
        
        cout << "Average: " << averageScore << ", minimum: " << minScore << ", maximum: " << maxScore << endl;
    }
}
