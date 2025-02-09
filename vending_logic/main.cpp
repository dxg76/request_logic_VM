#include "vendor.hpp"
#include "menu-tree.hpp"

/*
*
*Dante Gordon
*Updated 10/13/24
*
*Devan Rivera
*Updated 2/3/25
*Updated 2/6/25
*Updated 2/7/25
*
*This is the driver file for the MRSTV logic
*/

int main(int argc, const char** argv){
   
   //setup
    bool debug_mode = false;
    if(argc>1 && strcmp(argv[1], "-d") == 0){
        debug_mode = true;
    }

    Vendor vendor(debug_mode);
    
    //Start at the root ("Main Menu")
    Node* current_node = vendor.vendor_menu.root;
    string vendor_result;

    //setup end

    

    //main loop

    while(true){
        
        //error handling for parse and read
        do{
            vendor.parse(current_node);
            vendor_result = vendor.read_tokens(current_node);
        }while(vendor_result == "err");

        //quit sequence
        if(vendor_result == "critical"){
            std::cout << "Exiting program." << std::endl;
            return 0;

        }


        current_node = current_node->find_child(vendor_result);

        if(current_node != vendor.vendor_menu.root && current_node->get_price() < .1){ //in menu viewing items

            std::cout << "---" << vendor_result << " menu---\n" << std::endl; 

            vendor.vendor_menu.selection_menu(current_node, 0);


        }else if(current_node != vendor.vendor_menu.root){ //making selection
            vendor.empty_tokens();
            do{
                vendor.parse(current_node);
                vendor_result = vendor.read_tokens(current_node);
            }while(vendor_result == "err");
            if(vendor_result == "y"){
                vendor.vend(current_node->get_loc(), current_node->get_price()); //vend item define later
                current_node = vendor.vendor_menu.root;
            }else current_node = vendor.vendor_menu.root;

        }

        vendor.empty_tokens();

    }

    std::cout << "\nMr. Steve bids you farewell!" << std::endl;
    return 0;
}