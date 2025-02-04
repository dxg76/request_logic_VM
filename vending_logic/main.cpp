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
            std::cout << "You have selected: " << currentNode->get_id() << " " << std::endl;

            if(currentNode->get_price() > 0.0){

                std::cout << "Price: $" << currentNode->get_price() << "\nQuantity: " << currentNode->get_quantity() << std::endl;

            }
            else{

                std::cout << "This can not be read, please select a product." << std::endl;

            }

            //This prompts the user to go back to the previous menu
            std::cout << "\nEnter b to go back to the previous menu: " << std::endl;
            
            char userChoice;
            std::cin >> userChoice;

            if(userChoice == 'b' || userChoice == 'B'){

                //If the user chooses to go back, then the current node is set to the previous node
                if(!navigateStack.empty()){

                    currentNode = navigateStack.back();
                    navigateStack.pop_back();
                    continue;

                }
                else{

                    std::cout << "Currently at Main Menu. Exiting program." << std::endl;
                    break;

                }

            }
            else{

                break;

            }
        }
        else{

        //Displays the menu at the current node
        std::cout << "\nMenu: " << currentNode->get_id() << std::endl;
        const auto& children = currentNode->get_children();

        for(size_t i = 0; i < children.size(); ++i){

            std::cout << i + 1 << ". " << children[i]->get_id() << std::endl;

        }

        std::cout << "\nEnter your choice: ";
        int choice;
        std::cin >> choice;

        if(choice == 0){

            if(currentNode != vendingMenu.get_root() && !navigateStack.empty()){

                currentNode = navigateStack.back();
                navigateStack.pop_back();

            }
            else{

                break;  //Exit loop if at root

            }
        }
        else if(choice > 0 && static_cast<size_t>(choice) <= children.size()){
            //If the user selects a valid choice, then the current node is set to the selected child
            navigateStack.push_back(currentNode);
            currentNode = children[choice - 1];

        }
        else{

            std::cout << "Invalid choice. Please try again." << std::endl;
            
        }
    }
}

    std::cout << "\nMr. Steve bids you farewell!" << std::endl;
    return 0;
}