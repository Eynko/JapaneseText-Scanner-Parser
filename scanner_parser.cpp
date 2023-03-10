#include<iostream>
#include<fstream>
#include<string>
using namespace std;

void afterStory();
void afterSubject();
void noun();
void afterNoun();
void verb();
void afterObject();
void tense();
void be();
// flag for toggle tracing
bool trace = true;
// Subject flag
bool subjectFlag = false;


/*
Scanner part
*/

enum tokentype { VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR, EOFM, PERIOD, ERROR, WORD1, WORD2 };
string tokenName[16] = { "VERB", "VERBNEG", "VERBPAST","VERBPASTNEG","IS","WAS","OBJECT","SUBJECT","DESTINATION","PRONOUN","CONNECTOR","EOFM","PERIOD","ERROR","WORD1","WORD2" };
string reserved[19] = { "masu","masen","mashita","masendeshita","desu","deshita","o","wa","ni","watashi","anata","kanojo","kare","sore","mata","soshite","shikashi","dakara" };
void scanner(tokentype&, string&);  // to be called by main
fstream fin;   // file stream to use
    /*
    state -> state number based on submitted DFA in part 1.
    q0 = 0 | qt = 1
    qs = 2 |qc = 3
    qsa = 4|qy = 5
    q0q1 = 6| q0qy = 7
    RE:(vowel | vowel n | consonant vowel | consonant vowel n |
    consonant-pair vowel | consonant-pair vowel n)^+
    */
bool WORD_token(string s) {

    int state = 0;
    int charpos = 0;
    while (s[charpos] != '\0') {

        // Trs(q0,..)
        // Transitions from q0;
        if (state == 0 && s[charpos] == 't') state = 1;
        else if (state == 0 && s[charpos] == 's') state = 2;
        else if (state == 0 && s[charpos] == 'c') state = 3;
        else if (state == 0 && (s[charpos] == 'd' || s[charpos] == 'w' || s[charpos] == 'z' || s[charpos] == 'y' || s[charpos] == 'j')) state = 4;
        else if (state == 0 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' || s[charpos] == 'm' || s[charpos] == 'n' || s[charpos] == 'p' || s[charpos] == 'r')) state = 5;
        else if (state == 0 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'E' || s[charpos] == 'i' || s[charpos] == 'I' || s[charpos] == 'o' || s[charpos] == 'u')) state = 6;
        // Trs(qt,..)
        // Transitions from qt
        else if (state == 1 && s[charpos] == 't') state = 4;
        else if (state == 1 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'E' || s[charpos] == 'i' || s[charpos] == 'I' || s[charpos] == 'o' || s[charpos] == 'u')) state = 6;
        // Trs(qs,...)
        // Transitions from qs
        else if (state == 2 && s[charpos] == 'h') state = 4;
        else if (state == 2 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'E' || s[charpos] == 'i' || s[charpos] == 'I' || s[charpos] == 'o' || s[charpos] == 'u')) state = 6;
        //Trs(qc,...)
        //Transitions from qc
        else if (state == 3 && s[charpos] == 'h') state = 4;
        //Trs(qsa,..)
        //Transitions from qsa
        else if (state == 4 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'E' || s[charpos] == 'i' || s[charpos] == 'I' || s[charpos] == 'o' || s[charpos] == 'u')) state = 6;
        //Trs(qy,..)
        //Transitions from qy
        else if (state == 5 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'E' || s[charpos] == 'i' || s[charpos] == 'I' || s[charpos] == 'o' || s[charpos] == 'u')) state = 6;
        else if (state == 5 && s[charpos] == 'y') state = 4;
        //Trs(q0q1,...)
        //Transitions from q0q1
        else if (state == 6 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'E' || s[charpos] == 'i' || s[charpos] == 'I' || s[charpos] == 'o' || s[charpos] == 'u')) state = 6;
        else if (state == 6 && s[charpos] == 'n') state = 7;
        else if (state == 6 && (s[charpos] == 'd' || s[charpos] == 'w' || s[charpos] == 'z' || s[charpos] == 'y' || s[charpos] == 'j')) state = 4;
        else if (state == 6 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' || s[charpos] == 'm' || s[charpos] == 'n' || s[charpos] == 'p' || s[charpos] == 'r')) state = 5;
        else if (state == 6 && s[charpos] == 't') state = 1;
        else if (state == 6 && s[charpos] == 's') state = 2;
        else if (state == 6 && s[charpos] == 'c') state = 3;
        //Trs(q0qy,...)
        // Transitions from q0qy
        else if (state == 7 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'E' || s[charpos] == 'i' || s[charpos] == 'I' || s[charpos] == 'o' || s[charpos] == 'u')) state = 6;
        else if (state == 7 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' || s[charpos] == 'm' || s[charpos] == 'n' || s[charpos] == 'p' || s[charpos] == 'r')) state = 5;
        else if (state == 7 && (s[charpos] == 'd' || s[charpos] == 'w' || s[charpos] == 'z' || s[charpos] == 'y' || s[charpos] == 'j')) state = 4;
        else if (state == 7 && s[charpos] == 't') state = 1;
        else if (state == 7 && s[charpos] == 'c') state = 3;
        else if (state == 7 && s[charpos] == 's') state = 2;
        else return false;
        charpos++;
    }

    // What state did I end up in?
    if (state == 6 || state == 7) return true;
    else {
        return false;
    }
}

    // RE: .
bool period_token(string s) {

    if (s == ".") return true;
    else  return false;
}


// Scanner to be used in main to run through each DFA
void scanner(tokentype& the_type, string& w)
{
    fin >> w;
    cout << "Scanner called using word: " << w << endl;
    // if w is eofm, break and set type.
    if (w == "eofm") {
        the_type = EOFM;
        return;
    }
    // if token is recognized as word, check reserved word table
    else if (WORD_token(w)) {
        int idx = -1;
        // if word is reserved, save index for setting type.
        for (int i = 0; i < sizeof(reserved) / sizeof(reserved[0]); i++) {
            if (w == reserved[i]) {
                idx = i;
            }
        }

        // if not a reserved word, check if it is WORD1 or WORD2
        if (idx == -1) {
            if (w[w.size() - 1] == 'I' || w[w.size() - 1] == 'E') the_type = WORD2;
            else the_type = WORD1;
        }
        // used index to determine type of reserved.
        else {
            if (idx == 0) the_type = VERB;
            else if (idx == 1) the_type = VERBNEG;
            else if (idx == 2) the_type = VERBPAST;
            else if (idx == 3) the_type = VERBPASTNEG;
            else if (idx == 4) the_type = IS;
            else if (idx == 5) the_type = WAS;
            else if (idx == 6) the_type = OBJECT;
            else if (idx == 7) the_type = SUBJECT;
            else if (idx == 8) the_type = DESTINATION;
            else if (idx == 9 || idx == 10 || idx == 11 || idx == 12 || idx == 13) the_type = PRONOUN;
            else if (idx == 14 || idx == 15 || idx == 16 || idx == 17) the_type = CONNECTOR;
        }
    }
    // if not a WORD, check if it is period.
    else if (period_token(w)) the_type = PERIOD;

    //else, none of the FAs returned TRUE, set type as ERROR.
    else
    {
        cout << endl << "Lexical Error: " << w << " is not a valid token" << endl;
        the_type = ERROR;
    }

}


// The test-driver for scanner.
//int main()
//{
//    string fname;
//    cout << "Enter the input file name:";
//    cin >> fname;
//    // open input file via fin
//    fin.open(fname.c_str(), fstream::in);
//    tokentype thetype;
//    string theword;
//    // while file can be read, call scanner on each word.
//    while (true)
//    {
//        scanner(thetype, theword);
//        // if eofm, stop reading.
//        if (theword == "eofm") break;
//        // else, output word and it's type.
//        cout << " ' " << theword << " ' " << " is token type " << tokenName[thetype] << endl;
//    }
//    cout << ">>>>>End of File encountered" << endl;
//    fin.close();
//    return 0;
//}// end of main 

tokentype saved_token;
string saved_lexeme;
bool token_available = false;

// Type of error: matching fails
void syntaxerror1(tokentype saved, string lexeme) {
    if (trace == true)
        cout << "SYNTAX ERROR: expected " << tokenName[saved] << " but found " << lexeme << endl;
}
// Type of error: switch default
void syntaxerror2(string saved_lexeme, string parserfunction) {
    if (trace == true)
        cout << "SYNTAX ERROR: unexpected " << saved_lexeme <<  " found in " << parserfunction << endl;
    exit(1);
}

// Get the next token
tokentype next_token() {
    if (!token_available) {
        scanner(saved_token, saved_lexeme);
        token_available = true;
    }
    return saved_token;
}

// Make sure expected token matches the actual token
bool match(tokentype expected) {
    if (next_token() != expected)
    {
        syntaxerror1(expected, saved_lexeme);
        // Ask user to skip or replace the error token
        cout << "Skip or replace the token? s/r ";
        string err;
        getline(cin, err);

        while (true)
        {
            // Make sure user input is correct
            if (err == "S" | err == "s" | err == "R" | err == "r")
            {
                // Skip the token
                if (err == "S" | err == "s")
                {
                    token_available = false;
                    match(expected);
                }
                // Replace the token
                else
                {
                    if (trace == true)
                        cout << "Matched " << tokenName[expected] << endl;
                    token_available = false;
                    return true;
                }
                cin.clear();
                break;
            }
            else
            {
                cout << "Not valid input. s/r ";
                cin.clear();
                getline(cin, err);
            }
        }
    }
    else {
        if (trace == true)
            cout << "Matched " << tokenName[expected] << endl;
        token_available = false;
        return true;
    }
}

// ----- RDP functions - one per non-term -------------------

// Grammar:  <s> ::= [CONNECTOR] <noun>...
void afterStory() {
    if (trace == true)
        cout << "Processing <s>" << endl;
    /* EOFM is possible at end of file
       CONNECTOR is possible but not required
       Noun is required */
    next_token();
    if (saved_token == EOFM) {
        return;
    }
    if (saved_token == CONNECTOR) {
        match(CONNECTOR);
        next_token();
        noun();
    }
    else {
        noun();
    }
}

// Grammar:  <noun> ::= WORD1 | PRONOUN 
void noun() {
    if (trace == true)
        cout << "Processing <noun>" << endl;
    switch (saved_token) {
    case PRONOUN:
        match(PRONOUN);
        break;
    case WORD1:
        match(WORD1);
        break;
    default:
        syntaxerror2(saved_lexeme, "noun parser function");
    }

    // Check if we have gotten a subject yet
    if (subjectFlag == false) {
        // After [CONNECTOR] <noun> is SUBJECT
        subjectFlag = true;
        match(SUBJECT);
        afterSubject();
    }
    // If not SUBJECT then must be <after noun>
    else
    {
        afterNoun();
    }

}

// Grammar:  <after subject> ::= <verb>... | <noun>...
void afterSubject() {
    if (trace == true)
        cout << "Processing <afterSubject>" << endl;
    switch (next_token()) {
    case WORD2:
        verb();
        break;
    case WORD1:
    case PRONOUN:
        noun();
        break;
    default:
        syntaxerror2(saved_lexeme, "afterSubject parser function");
    }
}


// Grammar:  <after noun> ::= <be>... | DESTINATION <verb>... | OBJECT <after object>
void afterNoun() {
    if (trace == true)
        cout << "Processing <afterNoun>" << endl;
    switch (next_token()) {
    case IS:
    case WAS:
        be();
        break;
    case DESTINATION:
        match(DESTINATION);
        verb();
        break;
    case OBJECT:
        match(OBJECT);
        afterObject();
        break;
    default:
        syntaxerror2(saved_lexeme, "afterNoun parser function");
    }

}

// Grammar:  <verb> ::= WORD2
void verb() {
    if (trace == true)
        cout << "Processing <verb>" << endl;
    match(WORD2);
    // After <verb> is <tense>
    tense();
}

// Grammar:  <after object> ::= <verb>... | <noun>...
void afterObject() {
    if (trace == true)
        cout << "Processing <afterObject>" << endl;
    switch (next_token()) {
    case WORD2:
        verb();
        break;
    case WORD1:
    case PRONOUN:
        noun();
        break;
    default:
        syntaxerror2(saved_lexeme, "afterObject parser function");
    }
}

// Grammar:  <tense> ::= VERBPAST | VERBPASTNEG | VERB | VERBNEG
void tense() {
    if (trace == true)
        cout << "Processing <tense>" << endl; 
    switch (next_token()) {
    case VERBPAST:
        match(VERBPAST);
        break;
    case VERBPASTNEG:
        match(VERBPASTNEG);
        break;
    case VERB:
        match(VERB);
        break;
    case VERBNEG:
        match(VERBNEG);
        break;
    default:
        syntaxerror2(saved_lexeme, "tense parser function");
    }
    // After <tense> is PERIOD
    match(PERIOD);
}

// Grammar:  <be> ::= IS | WAS
void be() {
    if (trace == true)
        cout << "Processing <be>" << endl;
    switch (next_token()) {
    case IS:
        match(IS);
        break;
    case WAS:
        match(WAS);
        break;
    default:
        syntaxerror2(saved_lexeme, "be parser function");
    }
    // After <be> is PERIOD
    match(PERIOD);
}
string filename;

//----------- Driver ---------------------------

// The new driver to start the parser
int main() {
    cout << "Enter the input file name: ";
    getline(cin, filename);
    cin.clear();
    // Ask user if toggle tracings on or off
    cout << "Do you want tracings enabled for this file? Y/n ";
    string pref;
    getline(cin, pref);
    while (true)
    {
        // Make sure user input is correct
        if (pref == "Y" | pref == "y" | pref == "N" | pref == "n")
        {
            // Set the trace flag
            if (pref == "Y" | pref == "y")
                trace = true;
            else
                trace = false;
            cin.clear();
            break;
        }
        else
        {
            cout << "Not valid input. Y/n ";
            cin.clear();
            getline(cin, pref);
        }
    }

    fin.open(filename.c_str());
    if (trace == true)
        cout << "Processing <story>" << endl << endl;

    // Calls the <story> to start parsing
    // If not at the end of the file
    while (true) {
        afterStory();
        // Set the subjectFlag back 
        subjectFlag = false;
        // Check if at the end of the file
        if (saved_token == EOFM)
            break;
    }
    
    cout << endl << "Successfully parsed <story>.";
// closes the input file 
    fin.close();
}
