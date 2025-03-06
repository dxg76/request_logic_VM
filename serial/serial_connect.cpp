#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <string>

#define BUFFER_SIZE 512
int abstract;
bool debug_mode = true; 
float total_currency;
std::string port_name = "/dev/ttyACM0";
int open_serial(const char* port_name){
    //open serial port for  synchronous IO, read/write, checks it is not a controlling terminal
    int abstract = open(port_name, O_RDWR | O_NOCTTY | O_SYNC); 
    if(abstract < 0){
        std::cerr << "error opening" << port_name << ":" 
                  << strerror(errno) << std::endl; //global error number decode
        return -1;
    }
    return abstract;
}

void close_serial(){
    close(abstract);
}

bool configure_serial(int speed){
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

//token methods
std::string get_hex(std::string response){
    std::string hex_code;
    int start_index = response.find(',');
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

float read_coin_code(std::string hex_code){
    //no updates for the coin mech to share
    int compare_code = hex_code.compare("ACK");
    std::cout <<"compare code: " << compare_code <<std::endl;
    if(compare_code == 0){
        std::cout <<"no coins..." << std::endl;
        return 0;
    }
    //convert the string to a hexadecimal integer
    int hex = std::stoi(hex_code, nullptr, 16);

    //get bytes

    return 0;
}
float read_bill_code(std::string hex_code){
    //no updates for the coin mech to share
    int compare_code = hex_code.compare("ACK");
    std::cout <<"compare code: " << compare_code <<std::endl;
    if(compare_code == 0){
        std::cout <<"no bills... " <<std::endl;
        return 0;
    }
    //convert the string to a hexadecimal integer
    int hex = std::stoi(hex_code, nullptr, 16);

    //get bytes

    return 0;
}

int write_to_MDB(std::string msg){
    msg = msg + '\n';
    const char* msg_ready = msg.c_str();
    if (write(abstract, msg_ready, strlen(msg_ready)) < 0) {
        std::cerr << "write error: "
                << strerror(errno) << std::endl;
                return -1;
    }
    return 0;
}

std::string read_from_MDB(){
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

int configure_card_reader(){
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
    std::cout << "mdb response: \n" << read_from_MDB()<<std::endl;


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

int configure_coin_mech(){
    tcflush(abstract,TCIOFLUSH);
    std::string coin_setup = "R,0C,FFFFFFFF";
    if(write_to_MDB(coin_setup) < 0){
        return -1;
    }
    std::cout << read_from_MDB() <<std::endl;
    std::cout << "[coin mech configured]" << std::endl;
    return 0;
}

int configure_bill_validator() {
    tcflush(abstract,TCIOFLUSH);
    std::string bill_setup = "R,34,FFFFFFFF";
    if(write_to_MDB(bill_setup) < 0){
        return -1;
    }
    std::cout << read_from_MDB() <<std::endl;
    std::cout << "[bill validator configured]" << std::endl;
    return 0;
}

bool accept_card_payment(float item_cost) {
    std::string request_payment = "D,REQ," + std::to_string(item_cost);
    std::string vend_confirmed;
    std::string vend_rejected;

    if(read_from_MDB() != "placeholder"){
        std::cout << "no card..." <<std::endl;
        return false;
    }

    write_to_MDB(request_payment);
    if(read_from_MDB() != "placeholder"){
        write_to_MDB(vend_rejected);
        return false;
    }else write_to_MDB(vend_confirmed);
    return true;
}

float accept_coin_payment() {
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
    inserted_currency = read_coin_code(response);
    //implement some string parsing and cost calculation
    if(inserted_currency > .01){
        return inserted_currency;
    }else return 0;
}

float accept_bill_payment() {
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

    inserted_currency = read_bill_code(response);
    //implement some string parsing and cost calculation
    if(inserted_currency > .01){
        return inserted_currency;
    }else return 0;

}

int configure_all() {
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
    // Implementation goes here
    return 0;
}

bool check_payment(float item_cost){
    //paid by card
    bool card_payment = false;
    tcflush(abstract,TCIOFLUSH);
    //poll payment peripherals
    while(total_currency < item_cost && !card_payment){
        total_currency += accept_coin_payment();
        total_currency += accept_bill_payment();
        card_payment = accept_card_payment(item_cost);
    }
    std::cout << "payment satisfied " << std::endl;
    return true;
}
int main(){
    /*
    //linux usb port
    const char* port_name = "/dev/ttyACM0";
    //open serial connection
    abstract = open_serial(port_name);

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
    */

    configure_all();
    check_payment(1);
    close_serial(); 
    return 0;
}