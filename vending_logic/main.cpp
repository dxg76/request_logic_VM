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
    //setup end

    //creating the menu tree
    Menu_tree vendingMenu;

    //Start at the root ("Main Menu")
    Node* root = vendingMenu.get_root();

    //main loop
    string vendorResult;

    while(true){

        vendor.parse();
        vendorResult = vendor.read_tokens();

        if(vendorResult == "critical"){

            std::cout << "Exiting program." << std::endl;
            break;

        }

        string targetName;

        if(vendorResult == "chips"){
            
            targetName = "chips";

        }
        else if(vendorResult == "candy"){

            targetName = "candy";

        }
        else if(vendorResult == "help"){

            targetName = "help";

        }
        else{

            std::cout << "Invalid request. Please try again." << std::endl;
            vendor.empty_tokens();
            continue;

        }

        Node* targetNode = root->find_child(targetName);

        if(targetNode != nullptr){

            std::cout << "---" << targetName << " Menu---" << std::endl; 

            vendingMenu.selection_menu(targetNode, 0);

        }
        else{

            std::cout << "Menu not found." << std::endl;

        }

        vendor.empty_tokens();

    }

    std::cout << "\nMr. Steve bids you farewell!" << std::endl;
    return 0;
}