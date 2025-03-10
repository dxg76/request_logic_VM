#include <pigpio.h>
#include <iostream>

volatile int row = 0;
volatile int col = 0;
int a_pin = 2;
int b_pin = 3;
int c_pin = 4;
int d_pin = 17;
int e_pin = 27;
int f_pin = 22;
int one_pin = 14;
int two_pin = 15;
int three_pin = 18;
int four_pin = 23;
int five_pin = 24;
int six_pin = 25;
int seven_pin = 8;
int eight_pin = 7;

//ISR rows
#include <iostream>

// ISR Rows
void click_a(int gpio, int level, uint32_t tick) {
    row = 'A';
    std::cout << "Interrupt on row A (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}
/*
void click_b(int gpio, int level, uint32_t tick) {
    row = 'B';
    std::cout << "Interrupt on row B (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_c(int gpio, int level, uint32_t tick) {
    row = 'C';
    std::cout << "Interrupt on row C (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_d(int gpio, int level, uint32_t tick) {
    row = 'D';
    std::cout << "Interrupt on row D (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_e(int gpio, int level, uint32_t tick) {
    row = 'E';
    std::cout << "Interrupt on row E (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_f(int gpio, int level, uint32_t tick) {
    row = 'F';
    std::cout << "Interrupt on row F (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

// ISR Columns
void click_one(int gpio, int level, uint32_t tick) {
    col = 1;
    std::cout << "Interrupt on column 1 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_two(int gpio, int level, uint32_t tick) {
    col = 2;
    std::cout << "Interrupt on column 2 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_three(int gpio, int level, uint32_t tick) {
    col = 3;
    std::cout << "Interrupt on column 3 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_four(int gpio, int level, uint32_t tick) {
    col = 4;
    std::cout << "Interrupt on column 4 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_five(int gpio, int level, uint32_t tick) {
    col = 5;
    std::cout << "Interrupt on column 5 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_six(int gpio, int level, uint32_t tick) {
    col = 6;
    std::cout << "Interrupt on column 6 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_seven(int gpio, int level, uint32_t tick) {
    col = 7;
    std::cout << "Interrupt on column 7 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_eight(int gpio, int level, uint32_t tick) {
    col = 8;
    std::cout << "Interrupt on column 8 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}
*/
/*
*
*g++ -Wall -pthread -o interrupt interrupt.cpp -lpigpio -lrt
*
*
*/

void set_all_gpio(){
    //set pin mode
    if(gpioSetMode(a_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    /*
    if(gpioSetMode(b_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(c_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(d_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(e_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(f_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(one_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(two_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(three_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(four_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(five_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(six_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(seven_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    if(gpioSetMode(eight_pin, PI_INPUT) != 0){
        std::cout << "gpio set error" << std::endl;
    }
    */
    //set pullups
    if(gpioSetPullUpDown(a_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << a_pin << std::endl;
    }
    /*
    if(gpioSetPullUpDown(b_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << b_pin << std::endl;
    }
    if(gpioSetPullUpDown(c_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << c_pin << std::endl;
    }
    if(gpioSetPullUpDown(d_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << d_pin << std::endl;
    }
    if(gpioSetPullUpDown(e_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << e_pin << std::endl;
    }
    if(gpioSetPullUpDown(f_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << f_pin << std::endl;
    }
    if(gpioSetPullUpDown(one_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << one_pin << std::endl;
    }
    if(gpioSetPullUpDown(two_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << two_pin << std::endl;
    }
    if(gpioSetPullUpDown(three_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << three_pin << std::endl;
    }
    if(gpioSetPullUpDown(four_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << four_pin << std::endl;
    }
    if(gpioSetPullUpDown(five_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << five_pin << std::endl;
    }
    if(gpioSetPullUpDown(six_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << six_pin << std::endl;
    }
    if(gpioSetPullUpDown(seven_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << seven_pin << std::endl;
    }
    if(gpioSetPullUpDown(eight_pin, PI_PUD_UP) != 0) {
        std::cout << "gpio pull up error on pin " << eight_pin << std::endl;
    }
    */
   std::cout << "all gpio set" << std::endl;
}

void set_all_ISR(){
    
    int result;

    result = gpioSetISRFunc(a_pin, RISING_EDGE, 0, click_a);
    if (result != 0) {
        std::cout << "Interrupt failure on A (GPIO " << a_pin << "), Error Code: " << result << std::endl;
    }
    /*
    result = gpioSetISRFunc(b_pin, FALLING_EDGE, -1, click_b);
    if (result != 0) {
        std::cout << "Interrupt failure on B (GPIO " << b_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(c_pin, FALLING_EDGE, -1, click_c);
    if (result != 0) {
        std::cout << "Interrupt failure on C (GPIO " << c_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(d_pin, FALLING_EDGE, -1, click_d);
    if (result != 0) {
        std::cout << "Interrupt failure on D (GPIO " << d_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(e_pin, FALLING_EDGE, -1, click_e);
    if (result != 0) {
        std::cout << "Interrupt failure on E (GPIO " << e_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(f_pin, FALLING_EDGE, -1, click_f);
    if (result != 0) {
        std::cout << "Interrupt failure on F (GPIO " << f_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(one_pin, FALLING_EDGE, -1, click_one);
    if (result != 0) {
        std::cout << "Interrupt failure on column 1 (GPIO " << one_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(two_pin, FALLING_EDGE, -1, click_two);
    if (result != 0) {
        std::cout << "Interrupt failure on column 2 (GPIO " << two_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(three_pin, FALLING_EDGE, -1, click_three);
    if (result != 0) {
        std::cout << "Interrupt failure on column 3 (GPIO " << three_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(four_pin, FALLING_EDGE, -1, click_four);
    if (result != 0) {
        std::cout << "Interrupt failure on column 4 (GPIO " << four_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(five_pin, FALLING_EDGE, -1, click_five);
    if (result != 0) {
        std::cout << "Interrupt failure on column 5 (GPIO " << five_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(six_pin, FALLING_EDGE, -1, click_six);
    if (result != 0) {
        std::cout << "Interrupt failure on column 6 (GPIO " << six_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(seven_pin, FALLING_EDGE, -1, click_seven);
    if (result != 0) {
        std::cout << "Interrupt failure on column 7 (GPIO " << seven_pin << "), Error Code: " << result << std::endl;
    }
    
    result = gpioSetISRFunc(eight_pin, FALLING_EDGE, -1, click_eight);
    if (result != 0) {
        std::cout << "Interrupt failure on column 8 (GPIO " << eight_pin << "), Error Code: " << result << std::endl;
    }    
    */
}

int main(){
    //intitialize GPIO
    if(gpioInitialise() < 0){
        std::cout << "pigpio failed to initialize" << std::endl;
        return -1;
    }else std::cout << "pigpio initialized successfully" << std::endl;
    set_all_gpio();
    set_all_ISR();
    

    while( row == 0 || col == 0){
        //main program
    }
    std::cout <<"Row: " << row << " was selected and col: " 
    << col << " was selected" << std::endl;
}

