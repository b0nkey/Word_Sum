#include <iostream>
#include <vector>
#include <map>
// For some reason, these two are needed on Windows machines but not on Mac
// #include <time.h>
// #include <math.h>
using namespace std;



typedef struct c{ // Node struct for an individual char. 
    char letter;
    int value;
    int frequency;
}c;


class WordSum{
private:
    int attempts;
    vector<string> words;
    string sum;
    vector<bool> use; // marks used numbers
    vector<c> charData; 
    double elapsed;

public:
    WordSum(vector<string>, string); // Default Constructor
    bool permutation(int);
    bool isValid(); // validity check
    bool leadingZero(string);
    int getWordValue(string); // returns value of a word using the current solution
    bool addChar(char);
    friend std::ostream& operator<<(std::ostream &out, const WordSum &right);
};


WordSum::WordSum(vector<string> userWords, string userSum){ // default constructor
    clock_t start = clock();
    attempts = 0;
    int i,j;

    // Populate words vector
    for(string st : userWords)  words.push_back(st);
    sum = userSum;
    
    // Fill with false. <use> is for used numbers
    for(i=0; i<10; i++)  use.push_back(false);

    // Add every char to a vector as an object containing its value and frequency (struct c)
    for(i=0; i<words.size(); i++) // For amount of words
        for(j=0; j<words[i].length(); j++) // For each char in word
            addChar(words[i][j]);
    for(i=0; i<sum.length(); i++)  addChar(sum[i]); // Loop for sum

    // Passing Permutation <0> will start a recursive loop.
    permutation(0);

    clock_t end = clock();
    elapsed = double(end - start)/CLOCKS_PER_SEC;
}

std::ostream& operator<< (std::ostream &out, const WordSum &right){
    int i;
    // Stores list data in <out> object and returns it
    out << "Problem: ";
    for(i=0; i<right.words.size()-1; i++)
        out << right.words[i] << " + ";
    
    out << right.words[right.words.size()-1]
        << "  CPU = " << right.elapsed << endl;

    for(i=0; i<right.charData.size(); i++)
        out << right.charData[i].letter << " = " << right.charData[i].value << ", ";
    out << "Attempts = " << right.attempts << endl;

    return out;
};  

bool WordSum::addChar(char ch){
    for(int i=0; i<charData.size(); i++){
        if(charData[i].letter == ch){
            charData[i].frequency++;
            return 0;
        }
    }
    charData.push_back((c){ch, 0, 1});
    return 1;
}

bool WordSum::permutation(int n){ // true if all letters are assigned with vals to correctly solve sum
    // A permutation is an ordered arrangement of objects
    // from a group without repetitions.

    if(n == charData.size()-1){ // when all chars have values assigned
        for(int i=0; i<10; i++){
            if(use[i] == false){ // for those numbers, which are not used
                charData[n].value = i; //assign value i
                if(isValid() == true)  return true;
            }
        }
        return false;
    }

    for (int i=0; i<10; i++){
        if(use[i] == false){ // for those numbers, which are not used
            charData[n].value = i; //assign value i and mark as not available for future use
            use[i] = true;
            if(permutation(n+1)) //go for next characters
                return true;
            use[i] = false; //when backtracks, make available again
        }
    }
    return false;
}

bool WordSum::leadingZero(string s){
    for(int i=0; i<charData.size(); i++){
        if(charData[i].value == 0)
            if(charData[i].letter == s[0]) return false;
    }
    return true;
}

bool WordSum::isValid(){ // true if words = sum
    attempts++;
    int i;
    int values[words.size()];
    int sumVal=0, addVal=0;

    // Leading 0 checks
    for(string w : words) if(!leadingZero(w)) return false;
    if(!leadingZero(sum)) return false;

    for(i=0; i<words.size(); i++) // Get total value of words of left side of =
        addVal += getWordValue(words[i]);
    sumVal = getWordValue(sum); // Get value of word on right side of =

    if(addVal == sumVal)  return true;
    else return false;
}

int WordSum::getWordValue(string word){
    int i, j, returnAmount=0, m=1;        // m is the base 10 multiplier
    for(i=word.length()-1; i>=0; i--){    // chars backwards
        for(j=0; j<charData.size(); j++)  // letters available
            if(word[i] == charData[j].letter)  break;
        returnAmount += m * charData[j].value;
        m *= 10;
    }
    return returnAmount;
}

 

int main() {
    WordSum ws1({
        "SEND",
        "MORE"
    }, "MONEY");
    cout << ws1 << endl;
    WordSum ws2({
        "EARTH",
        "AIR",
        "FIRE",
        "WATER"
    }, "NATURE");
    cout << ws2 << endl;
    WordSum ws3({
        "SATURN",
        "URANUS",
        "NEPTUNE",
        "PLUTO"
    }, "PLANETS");
    cout << ws3 << endl;

   
    cout << "done" << endl;
    return 0;
}