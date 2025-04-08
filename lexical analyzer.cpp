#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

using namespace std;

// Character classes
enum CharClass { LETTER, DIGIT, UNKNOWN };

// Token types using enum
enum TokenType {
    INT_LIT = 10,
    IDENT = 11,
    ASSIGN_OP = 20,
    ADD_OP = 21,
    SUB_OP = 22,
    MULT_OP = 23,
    DIV_OP = 24,
    LEFT_PAREN = 25,
    RIGHT_PAREN = 26,
    EOF_TOKEN = -1
};

// Globals
CharClass charClass;
string lexeme;
char nextChar;
int tokenCount = 0;
int lineNum = 1;
TokenType nextToken;
ifstream in_fp;

// Function declarations
void addChar();
void getChar();
void getNonBlank();
TokenType lex();
TokenType lookup(char ch);

// Main function
int main() {
    in_fp.open("front.in");
    if (!in_fp) {
        cout << "ERROR - cannot open front.in" << endl;
        return 1;
    }

    getChar();  // Load first character

    do {
        lex();
    } while (nextToken != EOF_TOKEN);

    cout << "Total tokens: " << tokenCount << ", Lines: " << lineNum << endl;
    return 0;
}

// Add character to lexeme
void addChar() {
    lexeme += nextChar;
}

// Read next character and set character class
void getChar() {
    if (in_fp.get(nextChar)) {
        if (nextChar == '\n') lineNum++;
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    }
    else {
        charClass = UNKNOWN;
        nextToken = EOF_TOKEN;
    }
}

// Skip whitespace
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

// Lookup symbols
TokenType lookup(char ch) {
    switch (ch) {
    case '(': addChar(); return LEFT_PAREN;
    case ')': addChar(); return RIGHT_PAREN;
    case '+': addChar(); return ADD_OP;
    case '-': addChar(); return SUB_OP;
    case '*': addChar(); return MULT_OP;
    case '/': addChar(); return DIV_OP;
    case '=': addChar(); return ASSIGN_OP;
    default:  addChar(); return EOF_TOKEN;
    }
}

// Lexical analyzer
TokenType lex() {
    lexeme.clear();
    getNonBlank();

    switch (charClass) {
    case LETTER:
        addChar(); getChar();
        while (charClass == LETTER || charClass == DIGIT) {
            addChar(); getChar();
        }
        nextToken = IDENT;
        break;

    case DIGIT:
        addChar(); getChar();
        while (charClass == DIGIT) {
            addChar(); getChar();
        }
        nextToken = INT_LIT;
        break;

    case UNKNOWN:
        nextToken = lookup(nextChar);
        getChar();
        break;
    }

    if (nextToken == EOF_TOKEN) {
        lexeme = "EOF";
    }

    // Token counter
    tokenCount++;

    // Pretty output with color
    cout << "\033[1;34mLine " << lineNum << "\033[0m - ";
    cout << "\033[1;32mToken: " << nextToken << "\033[0m, ";
    cout << "\033[1;36mLexeme: \"" << lexeme << "\"\033[0m" << endl;

    return nextToken;
}
