#include "vendor.hpp"

//constructor
Vendor::Vendor(bool mode, bool voice_control, bool no_charge){
    total_currency = 0;
    state = 1;
    list_menu = false;
    confirmation_prompt = false;
    configure_all();
    set_debug(mode);
    set_voice_control(voice_control);
    set_no_charge(no_charge);
}

//vendor methods
void Vendor::set_debug(bool mode){
    debug_mode = mode;
    if(mode){
        std::cout<<"DEBUG MODE ENABLED" << std::endl;
    }
}

void Vendor::set_voice_control(bool voice_control){
    this->voice_control = voice_control;
    if(voice_control){
        std::cout<<"STARTING IN VOICE CONTROL MODE" << std::endl;
    }
}

void Vendor::set_no_charge(bool no_charge){
    this->no_charge = no_charge;
    if(no_charge){
        std::cout<<"NO CHARGE MODE ACTIVE" << std::endl;
    }
}

char Vendor::try_vend(std::string loc, float price, std::vector<int> quantity){
    //fill in with vending sequence
    int offset;
    if(debug_mode){
        std::cout << "Vending: " << loc << "..." << std::endl;
        
    }
    if(decrease_quantity(quantity, offset)){
        char row = loc[0];
        char col = loc[1];
        col += offset;
        return get_vend_code(row,col);
    }else return 0;
    
}

bool Vendor::decrease_quantity(std::vector<int>& quantities, int &offset){ /*Function to decrement quantities*/
    int quantity = 0;

    for(size_t i = 0; i < quantities.size(); ++i){
        
        if(quantities[i] > 0){
            
            quantities[i]--; /*Decrements the quantity stored*/
            for(size_t i = 0; i < quantities.size(); ++i){
                quantity += quantities[i];
            }
            std::cout << "Units remaining: " << quantity << std::endl;
            offset = i;
            return true; 
            
        }
    }

    
    return false; /*Returns false when 0*/
}
//token methods
std::string Vendor::get_hex(std::string response){
    std::string hex_code;
    std::cout << "this is response: " << response <<std::endl;
    size_t start_index = response.find(',');
    int end_index = response.find('\n');

    hex_code = response.substr(start_index+1, end_index-start_index-1);
    
    std::cout << "Invisible characters found at indices: ";
    for (size_t i = 0; i < hex_code.size(); ++i) {
        if (!std::isprint(hex_code[i])) { // Checks for non-printable characters
            std::cout << i << " (ASCII " << static_cast<int>(hex_code[i]) << "), ";
        }
   }
    std::cout << std::endl;
    
    return hex_code;
}

float Vendor::read_hex_code(std::string hex_code){
    //no updates to share
    int compare_code = hex_code.compare("ACK");
    if(compare_code == 0){
        std::cout <<"no currency added" << std::endl;
        return 0;
    }
    compare_code = hex_code.compare("NACK");
    if(compare_code == 0){
        return 0;
    }
    
    //response is from card reader
    if(hex_code[0] == 'd'){
        //card logic
        card_payment = true;
        return 0;
    }

    //convert the string to a hexadecimal integer
    int hex = std::stoi(hex_code, nullptr, 16);
    //coin detected
    if((hex >> 15) != 1) //need to fix this statement because bill returns 2 byte code also
        return accept_coins(hex);
    //bill detected
    else return accept_bills(hex);

}

std::string Vendor::generate_prompt(Node* current_node){


    if(current_node->get_id() == vendor_menu.root->get_id()){
        return current_node->get_audio_path();
    }
    //in menu
    else if(current_node->get_price() < 0.1 ){ 
        list_menu = true;
        
        std::cout << RETURN_TO_MAIN << std::endl; //menu node
        std::cout << "---" << current_node->get_id() << " menu---\n" << std::endl; 
        //print selections
        vendor_menu.selection_menu(current_node, 0);
        return current_node->get_audio_path();
    }
    //item selected
    else{
        confirmation_prompt = true;
        return current_node->get_audio_path();
    }

}

void Vendor::parse(std::string request, Node* current_node){

    char* token;

    token = strtok((char*)request.c_str(), " ");

    while(token != NULL){
        std::string normalized_token = normalize(token);
        tokens.push_back(normalized_token);
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

std::string Vendor::check_keywords(){
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
        }else if(tokens[i] == KILL_STRING){
            if(debug_mode)
                std::cout << "keyword detected: " << KILL_STRING << "\n" << std::endl;
            return KILL_STRING;
        }
    }
    return "err";
}

std::string Vendor::check_inventory(std::vector<Node*> items){

    for(long unsigned int i = 0; i <tokens.size(); ++i){
        for(long unsigned int j = 0; j <items.size(); ++j){
            if(items[j]->get_id().find(tokens[i]) != std::string::npos && tokens[i].size() >= (items[j]->get_id().size()/2)){
                return tokens[i];
            }
        }
        if(tokens[i] == HOME_STRING){
        if(debug_mode)
            std::cout << "keyword detected: " << HOME_STRING << "\n" << std::endl;
        return HOME_STRING;
        }
    }
    

    if(debug_mode){
        std::cout << "cannot find the item you are looking for :(\n" << std::endl;
    }
    //item not found
    return "err"; 
}

std::string Vendor::confirm_selection(){
    std::cout << "confirming selection, state: " << state << std::endl;
    if(state == 1){
        for(long unsigned int i = 0; i <tokens.size(); ++i){
            if(tokens[i] == "yes"){
                state = 2;
                list_menu = true;
                return "confirmed";
            }else if(tokens[i] == "no"){
                return "denied";
            }
        }
    }else if(state == 3){
        confirmation_prompt = false;
        for(long unsigned int i = 0; i <tokens.size(); ++i){
            if(tokens[i] == "yes"){
                state = 1;
                return "home";
            }else if(tokens[i] == "no"){
                state = 0;
                return "idle";
            }
        }
    }
    return "err";
}

std::string Vendor::wake_up(){
    for(long unsigned int i = 0; i <tokens.size(); ++i){
        if(tokens[i] == "steve"){
            return "awaken";
        }
    }
    return "err";
}

std::string Vendor::read_tokens(Node* current_node){
    std::string result;

    //Idling
    if(state == 0){
        result = wake_up();
        empty_tokens();
        return result;
    }
    //in main menu
    if(current_node == vendor_menu.root){ 
        result = check_keywords();
        empty_tokens();
    }
    //in menu searching for item
    else if(current_node->get_price() < .1){
        result = check_inventory(current_node->get_children());
        empty_tokens();
    }
    //selecting item
    else{ 
        result = confirm_selection();
        empty_tokens();
    }
    return result;
}

std::string Vendor::normalize(char* token){
    std::string pass_token = token;
    //remove punctuation from tokens
    pass_token.erase(std::remove_if(pass_token.begin(), pass_token.end(), ::ispunct), pass_token.end());
    //make all lowercase
    std::transform(pass_token.begin(), pass_token.end(), pass_token.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    
    return pass_token;
}

void Vendor::empty_tokens(){
    tokens.clear();
    if(debug_mode){
        std::cout << "Tokens Dumped" << std::endl;
    }
}

char Vendor::get_vend_code(char row, char col){
    char row_code;
    char col_code;
    //form row 
    switch(row){
        case 'A':
            row_code = 0;
            break;
        case 'B':
            row_code = 1;
            break;
        case 'C':
            row_code = 2;
            break;
        case 'D':
            row_code = 3;
            break;
        case 'E':
            row_code = 4;
            break; 
        default:
            std::cout << "row not found" <<std::endl;
            row_code = 0;
    }
    //form col
    switch(col){
        case '1':
            col_code = 1;
            break;
        case '2':
            col_code = 2;
            break;
        case '3':
            col_code = 3;
            break;
        case '4':
            col_code = 4;
            break;
        case '5':
            col_code = 5;
            break;
        case '6':
            col_code = 6;
            break;
        case '7':
            col_code = 7;
            break;
        default:
            std::cout << "column not found" <<std::endl;
            col_code = 0;
    }
    char code = ((row_code<<3) | col_code) + 32;
    std::cout <<"motor control code: " << code << std::endl;
    return code;
}

//end token methods

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
    std::cout <<"MDB WRITE!!!!!! " << msg << std::endl;
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
    std::cout << "MDB READ!!!!! " << std::endl;
    while(bytes_read>0){
        bytes_read = read(abstract,buffer,BUFFER_SIZE);
        response += buffer;
    }
    if(strlen(buffer) == 0){
        return "nothing read";
    }



   std::cout << "this is the read repsonse: " << response << "\n\nthe number of bytes transmitted is: " <<strlen(buffer) <<std::endl;
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
    configure_coin_mech();
    configure_bill_validator();
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
    std::string coin_setup = "R,0C,FFFFFFFF";
    if(write_to_MDB(coin_setup) < 0){
        return -1;
    }
    std::cout << read_from_MDB() <<std::endl;
    std::cout << "[coin mech configured]" << std::endl;
    return 0;
}

int Vendor::configure_bill_validator() {
    tcflush(abstract,TCIOFLUSH);
    std::string bill_setup = "R,34,FFFFFFFF";
    if(write_to_MDB(bill_setup) < 0){
        return -1;
    }
    std::cout << read_from_MDB() <<std::endl;
    std::cout << "[bill validator configured]" << std::endl;
    return 0;
}
void Vendor::print_mdb_response(){
    std::cout << read_from_MDB() <<std::endl;
}
//MDB pay

bool Vendor::try_payment(float item_cost){
    item_cost = 1;
    //paid by card
    std::cout << "paying..." << std::endl;
    if(!no_charge){
        total_currency = 0;
        tcflush(abstract,TCIOFLUSH);
        //poll payment peripherals
        while(total_currency < item_cost && !card_payment){
            card_payment = check_card_payment(item_cost);
        }
    }            
    std::cout << "payment complete!" << std::endl;
    list_menu = true;
    state = 3;
    return true;

    return false;
}
bool Vendor::check_card_payment(float item_cost) {
    std::cout << "card waiting." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    int item_cost_int = item_cost;
    std::string request_payment = "D,REQ," + std::to_string(item_cost_int);
    
    std::string vend_confirmed;
    std::string vend_rejected;
    std::string response;

    tcflush(abstract,TCIOFLUSH);

    if(write_to_MDB(request_payment) != 0){
        std::cout << "write error" << std::endl;
    }

    response = read_from_MDB();
  
    if(response.find("d,STATUS,RESULT,1") == std::string::npos){
        std::cout << "no card..." <<std::endl;
        return false;
    }

    //approve vend
    write_to_MDB("C,VEND,1");
    if(read_from_MDB() != "d,status,result,-1"){
        write_to_MDB("D,END,-1");
        return false;
    }else write_to_MDB("D,END");
    return true;
}

float Vendor::check_bills(){
    std::string poll_bill = "R,33";
    float inserted_currency = 0;

    //request bill validator for update
    write_to_MDB(poll_bill);
    std::cout << "checking bills..." << std::endl;
    std::string response = read_from_MDB();
    if(debug_mode){
        std::cout << response << std::endl;
    }
    
    //get hex code from machine  
    response = get_hex(response);
    //read code
    std::cout <<"hex code: " << response << std::endl;
    std::cout << "\nend hex code" << std::endl;

    inserted_currency = read_hex_code(response);
    //implement some string parsing and cost calculation
    if(inserted_currency > .01){
        return inserted_currency;
    }else return 0;
}

float Vendor::check_coins(){
    std::string poll_coin = "R,0B";
    float inserted_currency = 0;
    //request coin mech for update
    write_to_MDB(poll_coin);
    std::cout << "checking coins... " << std::endl;
    std::string response = read_from_MDB();
    if(debug_mode){
        std::cout << response << std::endl;
    }
    
    //get hex code from machine
    response = get_hex(response);
    //read code
    std::cout <<"hex code: " << response << std::endl;
    std::cout << "\nend hex code" << std::endl;
    inserted_currency = read_hex_code(response);

    if(inserted_currency > .01){
        return inserted_currency;
    }else return 0;
}

float Vendor::accept_coins(int hex) {
    //get coin type from code
    int coin_type = (hex>>8) & 0xF;

    std::cout << "coin id: " << coin_type << std::endl;
    if(coin_type == 2){
        std::cout << "quarter inserted" << std::endl;
        return .25; //quarter
    }else if(coin_type == 1){
        std::cout << "dime inserted" << std::endl;
        return .10; //dime
    }else if(coin_type == 0){
        std::cout << "nickel inserted" << std::endl;
        return .05; //nickel
    }else return 0; //unknown coin type
    //get bytes
    return 0;    
}

float Vendor::accept_bills(int hex) {
    //get bill type
    int bill_type = (hex>>8)  & 0xF;
    if(bill_type == 2){
        std::cout << "5 dollar bill inserted!" << std::endl;
        //accept bill from the escrow
        write_to_MDB("R,35,1");
        std::cout << "accepting from escrow..." <<std::endl;
        print_mdb_response();
        //poll to ensure the bill is in the clip
        write_to_MDB("R,33");
        std::cout << "bill accepted: " <<std::endl;
        print_mdb_response();
        return 5;
    }
    if(bill_type == 0){
        std::cout << "1 dollar bill inserted!" << std::endl;
        //accept bill from the escrow
        write_to_MDB("R,35,1");
        std::cout << "accepting from escrow..." <<std::endl;
        print_mdb_response();
        //poll to ensure the bill is in the clip
        write_to_MDB("R,33");
        std::cout << "bill accepted! " <<std::endl;
        print_mdb_response();
        return 1;            
    }else{
        //reject bill return to user
        write_to_MDB("R,35,0");
        std::cout << "rejecting bill from escrow..." << std::endl;
        print_mdb_response();
        //poll to ensure the bill was returned
        write_to_MDB("R,33");
        std::cout << "bill rejected!" << std::endl;
        print_mdb_response();

        return 0; //unknown bill type  
    }
}


