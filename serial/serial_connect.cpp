#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

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

    tty.c_cc[VMIN] = 0; // no block on read even if no characters are available
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

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

int read_serial(int abstract, char* buffer, size_t size){
    return read(abstract, buffer, size); //retirns number of bits read
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
    /*
    const char* message = "Hello, Serial Port!";
    if (write_serial(abstract, message, strlen(message)) < 0) {
        std::cerr << "write error: "
                  << strerror(errno) << std::endl;
    }
    */
   while(true){
        char read_buffer[100];
        int bytes = read_serial(abstract, read_buffer, sizeof(read_buffer));
        if(bytes < 0){
            std::cerr << "read error: " 
                      << strerror(errno) <<std::endl;
        }else{
            std::cout << std::string(read_buffer, bytes) << std::endl;
        }
   }

    close_serial(abstract);
    return 0;
}