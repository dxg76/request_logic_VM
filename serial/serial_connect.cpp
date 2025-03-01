#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

#define BUFFER_SIZE 256
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

void close_serial(int abstract){
    close(abstract);
}

bool configure_serial(int abstract, int speed){
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

    tty.c_cc[VMIN] = 1; //block on read until 1 byte transmitted
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

int write_serial(int abstract, const char* buffer,size_t size){
    return write(abstract, buffer, size);
}

int configure_card_reader(int abstract){
    const char* message = "D,2";
    write(abstract,message, strlen(message));
    return 0;
}


int main(){
    const char* port_name = "/dev/ttyACM0";

    int abstract = open_serial(port_name);
    if(abstract < 0){
        return -1;
    }

    //configure_serial port 
    if(!configure_serial(abstract, B9600)){ // baud rate = 9600
        close_serial(abstract);
        return -1;
    }
    
    bool waiting_for_response = false;
    while(true){
        char c = 't';
        std::string buffer = "";
        const char* message = "PING";

        //send message
        if(!waiting_for_response){
            if (write_serial(abstract, message, strlen(message)) < 0) {
                std::cerr << "write error: "
                        << strerror(errno) << std::endl;
            }
            std::cout << "\nsending message...\n";
        }
        waiting_for_response = true;
        //read response
        while(c != '\n' && buffer.size() < BUFFER_SIZE){
            read(abstract,&c,1);
            buffer += c;
            
        }
            std::cout << "read{\n " << buffer <<" }\n\n";
            if(buffer == "PONG\r\n"){
                waiting_for_response = false;
            }else std::cout << "\nno match...\n";

    }

    close_serial(abstract);
    return 0;
}