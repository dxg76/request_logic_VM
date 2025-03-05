#include "vendor.hpp"

//costructor
Vendor::Vendor(bool mode){
    configure_all();
    set_debug(mode);
}

//vendor methods
void Vendor::set_debug(bool mode){
    debug_mode = mode;
    if(mode){
        std::cout<<"DEBUG MODE ENABLED" << std::endl;
    }
}

void Vendor::vend(std::string loc, float price){
    //fill in with vending sequence
    if(debug_mode){
        std::cout << "Vending..." << std::endl;
        std::cout << "vend complete! returning to main menu \n\n" << std::endl;
    }
}

//token methods
void Vendor::parse(std::string request, Node* current_node){

    char* token;
    std::string pass_token;
    
    //Prompt Generation


    if(debug_mode){
        if(current_node->get_id() == vendor_menu.root->get_id()){
            std::cout << GREETING_STRING << std::endl; //root node

        }else if(current_node->get_price() < 0.1 ){ //checking if menu or selection
            std::cout << RETURN_TO_MAIN << std::endl; //menu node

        }else std::cout << "you have selected " << current_node->get_id() 
                        << " are you sure you would like to purchase this item [Y/N] ?: "
                        << std::endl;
        std:: cout << "Enter input to be tokenized: ";
    }
    

    token = strtok((char*)request.c_str(), " ");

    while(token != NULL){
        pass_token = token;
        //remove punctuation from tokens
        pass_token.erase(std::remove_if(pass_token.begin(), pass_token.end(), ::ispunct), pass_token.end());
        tokens.push_back(pass_token);
        token = strtok(NULL,  " ");
    }
    print_tokens();

}

void Vendor::print_tokens(){
    for(auto i : tokens){
        std::cout << i << std::endl;
    }
    std::cout << "\n\n\n" << std::endl;
}

std::string Vendor::read_tokens(Node* current_node){
    if(current_node == vendor_menu.root){ //in main menu
        for(long unsigned int i = 0; i <tokens.size(); ++i){

            if(tokens[i] == CHIPS_MENU_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << CHIPS_MENU_STRING << "\n" << std::endl;
                return CHIPS_MENU_STRING;
            }
            else if(tokens[i] == CANDY_MENU_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << CANDY_MENU_STRING << "\n" << std::endl;
                return CANDY_MENU_STRING;
            }else if(tokens[i] == HELP_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << HELP_STRING << "\n" << std::endl;
                return HELP_STRING;
            }else if(tokens[i] == HOME_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << HOME_STRING << "\n" << std::endl;
                return HOME_STRING;
            }else if(tokens[i] == KILL_STRING){
                if(debug_mode)
                    std::cout << "keyword detected: " << KILL_STRING << "\n" << std::endl;
                return KILL_STRING;
            }
        }
    }else if(current_node->get_price() < .1){ //in menu searching for item
        for(long unsigned int i = 0; i <tokens.size(); ++i){
            std::vector<Node*> items = current_node->get_children();
            for(long unsigned int j = 0; j <items.size(); ++j){
                if(tokens[i] == items[j]->get_id()){
                    return tokens[i];
                }
            }
            
        }
        if(debug_mode){
            std::cout << "cannot find the item you are looking for :(\n" << std::endl;
        }
        //item not found
        return "err"; 

    }//selecting item
    else{ 
        if(tokens[0] != "y" && tokens[0] != "n"
        && tokens[0] != "Y" && tokens[0] != "y"){
            if(debug_mode)
                std::cout << "Invalid input: " << tokens[0]  << "please enter [Y/N]: " << std::endl;
            
            return "err";
        }else return tokens[0];
        
    }
    empty_tokens();
    std::cout << "I'm sorry, I didn't quite understand that, please repeat your request." //command unrecognized
              << std::endl;
    return "err";
}

void Vendor::empty_tokens(){
    tokens.clear();
    if(debug_mode){
        std::cout << "Tokens Dumped" << std::endl;
    }
}

//serial methods
int Vendor::open_serial(const char* port_name) {
    //open serial port for  synchronous IO, read/write, checks it is not a controlling terminal
    int abstract = open(port_name, O_RDWR | O_NOCTTY | O_SYNC); 
    if(abstract < 0){
        std::cerr << "error opening" << port_name << ":" 
                    << strerror(errno) << std::endl; //global error number decode
        return -1;
    }
    return abstract;
}

void Vendor::close_serial() {
    close(abstract);
}

bool Vendor::configure_serial(int speed) {
    struct termios tty; //structure to hold serial port attributes

    if(tcgetattr(abstract, &tty) != 0){ //store attributes
        std::cerr << "tcgetattr() error" << strerror(errno)
                  << std::endl;
    }

    cfsetospeed(&tty, speed); //set output speed
    cfsetispeed(&tty, speed); //set input speed


    tty.c_cflag =  (tty.c_cflag &  ~CSIZE) | CS8; //clears size config and sets to 8 bit data chunks
    tty.c_iflag &= ~IGNBRK; //ignore break processing
    tty.c_lflag = 0; //no signal keys, no echo, no canonical processing
    tty.c_oflag = 0; // no remap, no delays
    tty.c_iflag &= ~(IXON | IXOFF| IXANY); // disable flow control

    tty.c_cc[VMIN] = 0; //block on read until 1 byte transmitted
    tty.c_cc[VTIME] = 10; // 1 second read timeout

    tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls, enable reading for receiver

    tty.c_cflag &= ~(PARENB | PARODD); // disable parity
    tty.c_cflag &= ~CSTOPB; //set 1 stop bit
    tty.c_cflag &= ~CRTSCTS; //disable hardware flow control

    if (tcsetattr(abstract, TCSANOW, &tty) != 0) { //set attributes and apply changes imediately(TCASNOW)
        std::cerr << "tcsetattr() error: " << strerror(errno)
             << std::endl;
        return false;
    }



    return true;
}


//mdb methods
int Vendor::write_to_MDB(std::string msg) {
    msg = msg + '\n';
    const char* msg_ready = msg.c_str();
    if (write(abstract, msg_ready, strlen(msg_ready)) < 0) {
        std::cerr << "write error: "
                << strerror(errno) << std::endl;
                return -1;
    }
    return 0;
}

std::string Vendor::read_from_MDB() {
    char buffer[BUFFER_SIZE] = "";
    int bytes_read = 1;
    std::string response = "";
    while(bytes_read>0){
        bytes_read = read(abstract,buffer,BUFFER_SIZE);
        response += buffer;
    }
    if(strlen(buffer) == 0){
        return "nothing read";
    }



    //std::cout << "this is the buffer: " << buffer << "the number of bytes transmitted is: " <<strlen(buffer) <<std::endl;
    return response; //success    
}

int Vendor::configure_all() {
    //open serial connection
    abstract = open_serial(port_name.c_str());

    //serial port failure
    if(abstract < 0){
        return -1;
    }

    //configure_serial port 
    if(!configure_serial(B115200)){ // baud rate = 115200
        close_serial();
        return -1;
    }

    //configure reader
    if(configure_card_reader() < 0){
        close_serial();
        return -1;
    }

    //start Generic Master
    std::string start_gen_master = "M,1";
    write_to_MDB(start_gen_master);
    std::cout << read_from_MDB() << std::endl;
    // Implementation goes here
    return 0;
}

int Vendor::configure_card_reader() {
    std::string c_master =  "D,2";
    std::string c_peripheral = "C,1";
    std::string enable_reader = "D,READER,1";

    //flush transmit and receive buffers
    tcflush(abstract,TCIOFLUSH);

    
    //enable cashless master
    std::cout << c_master << std::endl;
    if(write_to_MDB(c_master) != 0){
        return -1;
    }
    //MDB response to initialization
    std::cout << "mdb response: \n" << read_from_MDB() <<std::endl;
    
    //enable cashless peripheral
    std::cout << c_peripheral << std::endl;
    if(write_to_MDB(c_peripheral) != 0){
        return -1;
    }
    //MDB response to enabling cashless peripheral
    std::cout << "mdb response: \n" << read_from_MDB() <<std::endl;


    //enable reader
    std::cout << enable_reader << std::endl;
    if(write_to_MDB(enable_reader) != 0){
        return -1;
    }
    //MDB response to enabling reader
    std::cout << "mdb response: \n" << read_from_MDB() <<std::endl;

    //card reader success confirmation
    std::cout <<"[configured card reader]" << std::endl;

    return 0;
}

int Vendor::configure_coin_mech() {
    tcflush(abstract,TCIOFLUSH);
    std::string coin_setup = "R,0C,FFFF";
    if(write_to_MDB(coin_setup) < 0){
        return -1;
    }
    std::cout << read_from_MDB() <<std::endl;
    std::cout << "[coin mech configured]" << std::endl;
    return 0;
}

int Vendor::configure_bill_validator() {
    tcflush(abstract,TCIOFLUSH);
    std::string bill_setup = "R,34,FFFF";
    if(write_to_MDB(bill_setup) < 0){
        return -1;
    }
    std::cout << read_from_MDB() <<std::endl;
    std::cout << "[bill validator configured]" << std::endl;
    return 0;
}

//MDB pay
bool Vendor::accept_card_payment(float item_cost) {
    std::string request_payment = "D,REQ," + std::to_string(item_cost);
    std::string vend_confirmed;
    std::string vend_rejected;

    while (read_from_MDB() != "placeholder"){}
        write_to_MDB(request_payment);
    if(read_from_MDB() != "placeholder"){
        write_to_MDB(vend_rejected);
        return false;
    }else write_to_MDB(vend_confirmed);
    return true;
}

bool Vendor::accept_coin_payment() {
    // Implementation goes here
    return false;
}

bool Vendor::accept_cash_payment() {
    // Implementation goes here
    return false;
}