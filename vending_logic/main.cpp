#include "vendor.hpp"

/*

Dante Gordon dxg2@students.uwf.edu
Devan Rivera
10/013/24

This is the driver file for the MRSTV logic 
Until more elements are fleshed out this will stand as a testing ground for the vendor
*/
int main(int argc, const char** argv){
   
   //setup
    bool debug_mode = false;
    if(argc>1 && strcmp(argv[1], "-d")==0){
        debug_mode = true;
    }
    vendor alpha(debug_mode);
    string response = "temp"; //variable holding response from MRSTV
    //setup end

    //main loop
    
    while(response != "critical"){
    alpha.parse();
    response = alpha.read_tokens();
    cout << response << endl;
    }
    
    //main loop end

    return 0;
}