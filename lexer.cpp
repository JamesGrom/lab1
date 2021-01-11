#include <iostream>
#include <fstream>
#include <ctype.h>

using namespace std;

// filepath of the file used for logging the state of the compiler
string filepath = "./logFile.txt";
// global counter incremented each time a c character pointer moves to the next character
int characterCount = 0;

// define the array of keywords
// has a length of 32
string keywordArray[] = {"auto",
                         "break",
                         "case",
                         "char",
                         "const",
                         "continue",
                         "default",
                         "do",
                         "double",
                         "else",
                         "enum",
                         "extern",
                         "float",
                         "for",
                         "goto",
                         "if",
                         "int",
                         "long",
                         "register",
                         "return",
                         "short",
                         "signed",
                         "sizeof",
                         "static",
                         "struct",
                         "switch",
                         "typedef",
                         "union",
                         "unsigned",
                         "void",
                         "volatile",
                         "while"};

// define array of operators
// has a length of 30
string operatorArray[] =
    {
        "=", "|", "||", "&&", "==", "!=", "<", ">", "<=", ">=", "+", "-", "*", "/", "%", "&", "!", "++", "--", ".", "->", "(", ")", "[", "]", "{", "}", ";", ":", ","};

// delete current contents of the logfile and handle errors in file opening
bool clearLogFile()
{
    // --Mark Initialize logFile
    // create a file object
    std::ofstream logFile;
    // open file in write mode to delete file contents from last logging session
    logFile.open(filepath); //if the file doesn't exist, it will create one for us
    // opens the logfile in write mode so it's clean from the start each time the program is run
    if (logFile.is_open())
    {
        logFile << "started loging in ./logFile.txt\n";
        return true;
    }
    else
    {
        printf("failure opening log file \n");
        return false;
    }
}

// logs the current state of the character pointer and the token
// open and close file inside function to allow for live logging to the logfile.txt
void logState(string description, char c, string token)
{
    std::ofstream logFile;
    logFile.open(filepath, std::ios_base::app); //open the file in append mode

    // log the current state to the logFile
    logFile << "-------STATE---------" << endl;
    logFile << "Character Description:   " << description << "     Character as interpreted:   " << c << "    current Token:   " << token << endl;
    logFile << " -------ENDSTATE---------" << endl
            << endl
            << endl;
    logFile.close(); //close the file so the changes are submitted
    return;
}

// log the state of the token when it's being outputted
// description: description of the current character as high level interpretation
// c : explicit interpretation of the lexer (low level)
// lexical Output: the output that the token has been defined as
// token: the token that is outputed
void logTokenOutput(string description, char c, string lexicalOutput, string token)
{
    std::ofstream logFile;
    logFile.open(filepath, std::ios_base::app); //open the file in append mode

    // log the current state to the logFile
    logFile << "----------TOKEN OUTPUT---------" << endl;
    logFile << "Token Description:   " << description << "     Character as interpreted:   " << c << "    Lexical Output:   " << lexicalOutput << "    current Token:   " << token << endl;
    logFile << " --------END TOKEN OUTPUT--------" << endl
            << endl
            << endl;
    logFile.close(); //close the file so the changes are submitted
    return;
}

// log a comment string to the logfile
void logString(string temp)
{
    std::ofstream logFile;
    logFile.open(filepath, std::ios_base::app); //open the file in append mode
    logFile << endl
            << "///////////   " << temp << "  /////////" << endl
            << endl;
    logFile.close();
    return;
}

int main()
{
    // initialize logging file
    if (!clearLogFile())
    {
        return -1; //failure opening logging file therefore cancel the program
    }
    // logfile has been successfully cleared and initialized, use append mode from here on out

    // --Mark begin lexical analysis
    string token;
    //cin.get() returns a single character  as unformatted input
    logString("waiting on first character to enter while loop");
    char c;

    // while not at the end of the file
    while (cin.get(c))
    {
        logString("top of loop");

        // PRECONDITION:
        //      - last token should have been logged
        //      - c has been updated to the next token to be analyzed (therefore no need to get c at the start) it's retrieved in the while condition

        //delete current token state with this function to start fresh
        token.clear();

        // Case: C is a / followed by a * for a comment
        string testString;
        testString += c;
        if (testString == "/")
        {
            if (string(1, cin.peek()) == "*")
            {

                token += c;
                //the comment has now started, pull characters until we find a */ in succession
                cin.get(c);
                token += c;
                bool foundEnding = false;
                while (!foundEnding)
                {
                    //keep pulling characters till comment ending is found
                    cin.get(c);
                    token += c;
                    //anylize potential comment ending
                    if (string(1, c) == "*")
                    {
                        string peekString;
                        peekString += cin.peek();
                        if (peekString == "/")
                        {
                            // we've found the comment end
                            foundEnding = true;
                        }
                    }
                }
                // move the c pointer so it ends on the /  and comment string has been completely passed
                cin.get(c);
                token += c;
                logTokenOutput("comment", c, "", token);
                continue;
            }
        }

        // Case: C is a whitespace character
        // Identifier: C == "isWhiteSpace"
        // true if c is (' '| '\t'|'\n'|'\v|'\f'|'\r')
        // Handle: peek the next character and move c 2 spaces if it creates a whitespace character
        if (isspace(c))
        {
            string interpretedToken = "whitespace";
            token += c;
            if (token == " ")
            {
                interpretedToken = "space";
            }
            if (token == "\n")
            {
                interpretedToken = "newline";
            }
            if (token == "\t")
            {
                interpretedToken = "tab";
            }
            if (token == "\v")
            {
                interpretedToken = "slash v";
            }
            if (token == "\f")
            {
                interpretedToken = "slash f";
            }
            if (token == "\r")
            {
                interpretedToken = "slash r";
            }
            // log
            logTokenOutput(interpretedToken, c, "", token);
            continue;
        }

        // Case: C is a number
        if (isdigit(c))
        {
            string interpretedToken = "number";

            // append the current digit to the token
            token += c;
            //pull the rest of the digits in the integer until a non-digit character appears , append each digit to the token
            while (isdigit(cin.peek()))
            {
                cin.get(c); //move to the next slot
                token += c; //add the digit to the number
            }
            logTokenOutput(interpretedToken, c, interpretedToken, token);
            cout << "int:" << token << endl;
            continue;
        }

        // case: C is a " starting a string
        string C;
        C.push_back(c); //create a string out of the C character
        string tester = "\"";
        // tester == C
        if (tester == string(1, c))
        {
            token += tester; //add the " symbol to the token output

            // logString("identified a doubleQuote");
            string interpretedToken = "String";
            // // don't add the first quote to the string cause it's unessessary
            // while the next character is not a double quote
            while ((string(1, cin.peek()) != tester))
            {
                // get the next character
                cin.get(c);
                // if current character is a \ then evaluate the next character without leaving the string
                if (string(1, c) == "\\")
                {

                    // handle next character accordingly
                    //if the next character is a doublequote, then it can be added
                    if (string(1, cin.peek()) == tester)
                    {
                        token += c; //include the backslash
                        cin.get(c); //move c to the next slot (the double quote)
                        //add the doublequote to the token
                        token += c;
                    }
                    else
                    {
                        token += c; //add the \ if it's not an escape character for doublequote
                    }
                }
                else
                {
                    token += c; //add the appropriate character to the string
                }
            }
            // add the doublequote to the end of the string
            token += tester;
            // make sure to increment c to the ending doubleQuote
            cin.get(c);
            logTokenOutput(interpretedToken, c, interpretedToken, token);
            cout << "string:" << token << endl;
            continue;
        }

        //case: C is a _ or a letter starting an identifier or keyword
        if (isalpha(c) || string(1, c) == "_")
        {
            string interpretedToken = "identifier";
            token += c;
            while (isalnum(cin.peek()) || string(1, cin.peek()) == "_")
            {
                cin.get(c);
                token += c;
            }

            //check if the token is any of the keyword strings

            // determine if the identifier is a keyword or not
            bool isKeyword = false;
            for (int i = 0; i < 32; i++)
            {
                if (keywordArray[i] == token)
                {
                    isKeyword = true;
                }
            }

            // handle case where identifier is indeed a keyword
            if (isKeyword)
            {
                interpretedToken = "keyword";
                logTokenOutput(interpretedToken, c, interpretedToken, token);
                cout << "keyword:" << token << endl;
                continue;
            }
            else
            {
                logTokenOutput(interpretedToken, c, interpretedToken, token);
                cout << "identifier:" << token << endl;
                continue;
            }
        }

        // case C: is an operator
        bool isOperator = false;
        for (int i = 0; i < 30; i++)
        {
            if (operatorArray[i] == C)
            {
                isOperator = true;
            }
        }

        if (isOperator)
        {
            token += c;
            // handle case where c is an operator
            string interpretedToken = "operator";
            //check if itself appended with the next character forms one of the 2 character operators
            string possibleDualOppString;
            possibleDualOppString += c;
            possibleDualOppString += cin.peek();
            bool isDualOpperator = false;
            for (int i = 0; i < 30; i++)
            {
                if (operatorArray[i] == possibleDualOppString)
                {
                    isDualOpperator = true;
                }
            }

            if (isDualOpperator)
            {
                // fetch the character and add it to the token
                cin.get(c);
                token += c;
            }

            //log the output and output the token
            logTokenOutput(interpretedToken, c, interpretedToken, token);
            cout << "operator:" << token << endl;
            continue;
        }
    }
    logString("Reached end of the file, terminated while loop");
}
