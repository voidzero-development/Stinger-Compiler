#include "llvm/ADT/STLExtras.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Scanner aka. lexer

enum currentToken{
    token_fileEnd = -1,
    token_definition = -2,
    token_number = -3,
    token_extern = -4,
    token_identifier = -5,
};
//some defines for getting the current token correctly
static std::string identStr;
static double numberValue;

static int getCurrentToken(){
    static int lastCharacter = ' ';

    //white spaces are for humans not computers ;)
    while (isspace(lastCharacter))
    lastCharacter = getchar();

    //is it an alphanumeric character?
    if(isalpha(lastCharacter)){
        identStr = lastCharacter;
        while (isalnum((lastCharacter = getchar())))
        {
            identStr += lastCharacter;
        }

        //recognize identifiers and keywords
        if (identStr == "define"){
            return token_definition;
        }else if(identStr == "extern") {
            return tok_extern;
        }else {
            return token_identifier;
        }
    }

    //digits
    if (isdigit(lastCharacter) || lastCharacter == '.'){
        std::string numberStr;

        //ensure we get numbers correctly, so input 1.23.45.67 will compile to value 1.234567.
        bool firstDotChar = false;

        do {   
            if (lastCharacter == '.' && !firstDotChar){
                numberStr += lastCharacter;
                lastCharacter = getchar();
                firstDotChar = true; 
            }else if (LastChar == '.' && firstDotChar){
                lastCharacter = getchar();
            }else {
                numberStr += lastCharacter;
                lastCharacter = getchar();
            }
        }while (isdigit(lastCharacter) || lastCharacter == '.');

        numberValue = strtod(numberStr.c_str(), 0);
        return token_number;
    }

    //comments $, fuck poeple that want to use this over | in bitwise OR
    if (lastCharacter == '$'){
        do {
            lastCharacter = getchar();
        }while (lastCharacter != EOF && lastCharacter != '\n' && lastCharacter != '\r');

        if (lastCharacter != EOF){
            return getCurrentToken();
        }
    }

    if (lastCharacter == EOF){
        token_fileEnd;
    }

    int tChar = lastCharacter;
    lastCharacter = getchar();
    return tChar;
}

