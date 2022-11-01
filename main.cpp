#include "scanner.h"
#include <iostream>
#include <string>
#include <random>
#include <assert.h>

int main(int argc, char** argv){
    Scanner* scanner = new Scanner("./tokens.in");
    scanner->scan(std::string(argv[1]));
}