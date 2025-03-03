#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

#define BUFFER_SIZE 512
int abstract;
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

std::string read_from_mdb(){
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
    std::cout << "mdb response: \n" << read_from_mdb() <<std::endl;
    
    //enable cashless peripheral
    std::cout << c_peripheral << std::endl;
    if(write_to_MDB(c_peripheral) != 0){
        return -1;
    }
    //MDB response to enabling cashless peripheral
    std::cout << "mdb response: \n" << read_from_mdb() <<std::endl;


    //enable reader
    std::cout << enable_reader << std::endl;
    if(write_to_MDB(enable_reader) != 0){
        return -1;
    }
    //MDB response to enabling reader
    std::cout << "mdb response: \n" << read_from_mdb() <<std::endl;

    //card reader success confirmation
    std::cout <<"[configured card reader]" << std::endl;

    return 0;
}

bool accept_payment(float item_cost){
    std::string request_payment = "D,REQ," + std::to_string(item_cost);
    std::string vend_confirmed;
    std::string vend_rejected;

    while (read_from_mdb() != "placeholder"){}
    write_to_MDB(request_payment);
    if(read_from_mdb() != "placeholder"){
        write_to_MDB(vend_rejected);
        return false;
    }else write_to_MDB(vend_confirmed);
    return true;
}

int main(){
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
    close_serial(); 
    return 0;
}