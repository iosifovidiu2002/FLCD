#include "scanner.h"
#include <iostream>
#include <string>
#include <random>
#include <assert.h>

// void test_scanner_token_classifier(Scanner* scanner){
//     assert(scanner->ident_const_token_classifier("123ova") == TOKEN);
//     assert(scanner->ident_const_token_classifier("_ova") == IDENTIFIER);
//     assert(scanner->ident_const_token_classifier("_ova123A") == IDENTIFIER);
//     assert(scanner->ident_const_token_classifier("_Ov1a23b") == IDENTIFIER);
//     assert(scanner->ident_const_token_classifier("1_Ov1a23b") == TOKEN);
//     assert(scanner->ident_const_token_classifier("_SSOVA") == IDENTIFIER);
// }

int main(int argc, char** argv){
    Scanner* scanner = new Scanner("./tokens.in");
    scanner->scan(std::string(argv[1]));
}