#include <wiringPi.h>
#include <iostream>
//vars
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
void click_a(int gpio, int level, uint32_t tick);
void click_b(int gpio, int level, uint32_t tick);
void click_c(int gpio, int level, uint32_t tick);
void click_d(int gpio, int level, uint32_t tick);
void click_e(int gpio, int level, uint32_t tick);
void click_f(int gpio, int level, uint32_t tick);
//ISR columns
void click_one(int gpio, int level, uint32_t tick);
void click_two(int gpio, int level, uint32_t tick);
void click_three(int gpio, int level, uint32_t tick);
void click_four(int gpio, int level, uint32_t tick);
void click_five(int gpio, int level, uint32_t tick);
void click_six(int gpio, int level, uint32_t tick);
void click_seven(int gpio, int level, uint32_t tick);
void click_eight(int gpio, int level, uint32_t tick);
//methods
void set_up_interrupts();
void set_all_gpio();

int main(void)
{
    // uses BCM numbering of the GPIOs and directly accesses the GPIO registers.
    wiringPiSetupGpio();

    // pin mode ..(INPUT, OUTPUT, PWM_OUTPUT, GPIO_CLOCK)
    // set pin 17 to input
    pinMode(2, OUTPUT);

    // pull up/down mode (PUD_OFF, PUD_UP, PUD_DOWN) => down
    //pullUpDnControl(17, PUD_DOWN);

    int count = 0;
    while(count < 20){
        digitalWrite(2, HIGH);
        delay(2000);
        digitalWrite(2,LOW);
        delay(20000);
        count++;
    }


}

//method definitions
void set_up_interrupts(){
    //rows
    if(wiringPiISR(a_pin, INT_FALLING_EDGE, click_a) != 0)
        std::cout << "Pin: " << a_pin << " interrupt failed." << std::endl;
    if(wiringPiISR(b_pin, INT_FALLING_EDGE, click_b) != 0)
        std::cout << "Pin: " << b_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(c_pin, INT_FALLING_EDGE, click_c) != 0)
        std::cout << "Pin: " << c_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(d_pin, INT_FALLING_EDGE, click_d) != 0)
        std::cout << "Pin: " << d_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(e_pin, INT_FALLING_EDGE, click_e) != 0)
        std::cout << "Pin: " << e_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(f_pin, INT_FALLING_EDGE, click_f) != 0)
        std::cout << "Pin: " << f_pin << " interrupt failed." << std::endl;
    //columns
    if (wiringPiISR(one_pin, INT_FALLING_EDGE, click_one) != 0)
        std::cout << "Pin: " << one_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(two_pin, INT_FALLING_EDGE, click_two) != 0)
        std::cout << "Pin: " << two_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(three_pin, INT_FALLING_EDGE, click_three) != 0)
        std::cout << "Pin: " << three_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(four_pin, INT_FALLING_EDGE, click_four) != 0)
        std::cout << "Pin: " << four_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(five_pin, INT_FALLING_EDGE, click_five) != 0)
        std::cout << "Pin: " << five_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(six_pin, INT_FALLING_EDGE, click_six) != 0)
        std::cout << "Pin: " << six_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(seven_pin, INT_FALLING_EDGE, click_seven) != 0)
        std::cout << "Pin: " << seven_pin << " interrupt failed." << std::endl;
    if (wiringPiISR(eight_pin, INT_FALLING_EDGE, click_eight) != 0)
        std::cout << "Pin: " << eight_pin << " interrupt failed." << std::endl;
    
}
void set_all_gpio(){
    //rows
    pinMode(a_pin, INPUT);
    pinMode(b_pin, INPUT);
    pinMode(c_pin, INPUT);
    pinMode(d_pin, INPUT);
    pinMode(e_pin, INPUT);
    pinMode(f_pin, INPUT);
    //columns
    pinMode(one_pin, INPUT);
    pinMode(two_pin, INPUT);
    pinMode(three_pin, INPUT);
    pinMode(four_pin, INPUT);
    pinMode(five_pin, INPUT);
    pinMode(six_pin, INPUT);
    pinMode(seven_pin, INPUT);
    pinMode(eight_pin, INPUT);
    //rows
    pullUpDnControl(a_pin, PUD_UP);
    pullUpDnControl(b_pin, PUD_UP);
    pullUpDnControl(c_pin, PUD_UP);
    pullUpDnControl(d_pin, PUD_UP);
    pullUpDnControl(e_pin, PUD_UP);
    pullUpDnControl(f_pin, PUD_UP);
    //columns
    pullUpDnControl(one_pin, PUD_UP);
    pullUpDnControl(two_pin, PUD_UP);
    pullUpDnControl(three_pin, PUD_UP);
    pullUpDnControl(four_pin, PUD_UP);
    pullUpDnControl(five_pin, PUD_UP);
    pullUpDnControl(six_pin, PUD_UP);
    pullUpDnControl(seven_pin, PUD_UP);
    pullUpDnControl(eight_pin, PUD_UP);
}

//ISR
void click_a(int gpio, int level, uint32_t tick){
    row = 'A';
    std::cout << "Interrupt on row A (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_b(int gpio, int level, uint32_t tick){
    row = 'B';
    std::cout << "Interrupt on row B (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_c(int gpio, int level, uint32_t tick){
    row = 'C';
    std::cout << "Interrupt on row C (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_d(int gpio, int level, uint32_t tick){
    row = 'D';
    std::cout << "Interrupt on row D (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_e(int gpio, int level, uint32_t tick){
    row = 'E';
    std::cout << "Interrupt on row E (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_f(int gpio, int level, uint32_t tick){
    row = 'F';
    std::cout << "Interrupt on row F (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

// ISR Columns
void click_one(int gpio, int level, uint32_t tick){
    col = 1;
    std::cout << "Interrupt on column 1 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_two(int gpio, int level, uint32_t tick){
    col = 2;
    std::cout << "Interrupt on column 2 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_three(int gpio, int level, uint32_t tick){
    col = 3;
    std::cout << "Interrupt on column 3 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_four(int gpio, int level, uint32_t tick){
    col = 4;
    std::cout << "Interrupt on column 4 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_five(int gpio, int level, uint32_t tick){
    col = 5;
    std::cout << "Interrupt on column 5 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_six(int gpio, int level, uint32_t tick){
    col = 6;
    std::cout << "Interrupt on column 6 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_seven(int gpio, int level, uint32_t tick){
    col = 7;
    std::cout << "Interrupt on column 7 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}

void click_eight(int gpio, int level, uint32_t tick){
    col = 8;
    std::cout << "Interrupt on column 8 (GPIO " << gpio << ") at time " << tick << " µs" << std::endl;
}