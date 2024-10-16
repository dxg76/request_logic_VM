#include "tokenizer.hpp"

/*

Dante Gordon dxg2@students.uwf.edu
Devan Rivera
10/013/24

This is the driver file for the MRSTV logic 
Until more elements are fleshed out this will stand as a testing ground for the tokenizer
*/
int main(int argc, const char** argv){
    bool debug_mode = false;
    if(argc>1 && strcmp(argv[1], "-d")==0){
        debug_mode = true;
    }
    Tokenizer alpha(debug_mode);
    return 0;
}