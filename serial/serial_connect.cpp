#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

int open_serial(char* port_name){
    //open serial port for  synchronous IO, read/write, checks it is not a controlling terminal
    int abstract = open(portname, O_RDWR | O_NOCTTY | O_SYNC); 
    if(abstract < 0){
        std::cerr << "error opening" << portname << ":" 
                  << strerror(errno) << std::endl; //global error number decode
        return -1;
    }
    return abstract
}

bool configure_serial(int abstract, int speed){
    struct termios tty; //structure to hold serial port attributes

    if(tcgetattr(fd, &tty) != 0){ //store attributes
        std::cerr << "tcgetattr() error" << strerror(errno);
                  << std::endl;
    }

    cfsetospeed(&tty, speed); //set output speed
    cfsetispeed(&tty, speed); //set input speed

    tty.c_cflag =  (tty.c_cflag &  ~CSIZE) | CS8; //clears size config and sets to 8 bit data chunks
}
int main(){
    char* port_name
    std::cout << "enter serial port name: ";
    std::cin >> port_name;

    int abstract = open_serial(port_name);
    if(abstract < 0){
        return -1;
    }

    //configure_serial port for 9600 baud rate
    if(!configure_serial(abstract, 9600))







    return 0;
}