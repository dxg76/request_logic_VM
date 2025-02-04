#include "vendor.hpp"
#include "menu-tree.hpp"

/*
*
*Dante Gordon
*Updated 10/13/24
*
*Devan Rivera
*Updated 2/3/25
*
*This is the driver file for the MRSTV logic
*/

int main(int argc, const char** argv){
   
   //setup
    bool debug_mode = false;
    if(argc>1 && strcmp(argv[1], "-d")==0){
        debug_mode = true;
    }
    Vendor alpha(debug_mode);
    string response = "temp"; //variable holding response from MRSTV
    //setup end

    //main loop
    
    while(response != "critical"){
    alpha.parse();
    response = alpha.read_tokens();
    cout << response << endl;
    }
    
    //main loop end

    //creating the menu tree
    Menu_tree vendingMenu;

    //Start at the root ("Main Menu")
    Node* currentNode = vendingMenu.get_root();

    //Keep track of previous menus for backtracking
    vector <Node*> navigateStack;

    while(true){

        if(currentNode->get_children().empty()){

            //If the current node has no children, then it is a leaf node at the end of the menu
            std::cout << "You have selected: " << currentNode->get_id() << " " << endl;

            if(currentNode->get_price() > 0.0){

                cout << "Price: $" << currentNode->get_price() << "\nQuantity: " << currentNode->get_quantity() << endl;

            }
            else{

                cout << "This can not be read, please select a product." << endl;

            }

            //This prompts the user to go back to the previous menu
            cout << "\nEnter b to go back to the previous menu: " << endl;
            
            char userChoice;
            cin >> userChoice;

            if(userChoice == 'b' || userChoice == 'B'){

                //If the user chooses to go back, then the current node is set to the previous node
                if(!navigateStack.empty()){

                    currentNode = navigateStack.back();
                    navigateStack.pop_back();
                    continue;

                }
                else{

                    cout << "Currently at Main Menu. Exiting program." << endl;
                    break;

                }

            }
            else{break;}

        }
    }

    return 0;
}