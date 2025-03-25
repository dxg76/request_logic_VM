
void set_all_gpio(){

    //motor control
    pinMode(m1, OUTPUT);
    pinMode(m2, OUTPUT);
    pinMode(m3, OUTPUT);
    pinMode(m4, OUTPUT);
    pinMode(m5, OUTPUT);
    pinMode(m6, OUTPUT);
    pinMode(confirm_pin, INPUT);
    digitalWrite(m1, 0);
    digitalWrite(m2, 0);
    digitalWrite(m3, 0);
    digitalWrite(m4, 0);
    digitalWrite(m5, 0);
    digitalWrite(m6, 0);
}

//ISR
void click_a(){
    row = 'A';
    std::cout << "Interrupt on row A (GPIO " << a_pin << ")" << std::endl;
}

void click_b(){
    row = 'B';
    std::cout << "Interrupt on row B (GPIO " << b_pin << ")" << std::endl;
}

void click_c(){
    row = 'C';
    std::cout << "Interrupt on row C (GPIO " << c_pin << ")" << std::endl;
}

void click_d(){
    row = 'D';
    std::cout << "Interrupt on row D (GPIO " << d_pin << ")" << std::endl;
}

void click_e(){
    row = 'E';
    std::cout << "Interrupt on row E (GPIO " << e_pin << ")" << std::endl;
}

void click_f(){
    row = 'F';
    std::cout << "Interrupt on row F (GPIO " << f_pin << ")" << std::endl;
}

// ISR Columns
void click_one(){
    col = '1';
    std::cout << "Interrupt on column 1 (GPIO " << one_pin << ")" << std::endl;
}

void click_two(){
    col = '2';
    std::cout << "Interrupt on column 2 (GPIO " << two_pin << ")" << std::endl;
}

void click_three(){
    col = '3';
    std::cout << "Interrupt on column 3 (GPIO " << three_pin << ")" << std::endl;
}

void click_four(){
    col = '4';
    std::cout << "Interrupt on column 4 (GPIO " << four_pin << ")" << std::endl;
}

void click_five(){
    col = '5';
    std::cout << "Interrupt on column 5 (GPIO " << five_pin << ")" << std::endl;
}

void click_six(){
    col = '6';
    std::cout << "Interrupt on column 6 (GPIO " << six_pin << ")" << std::endl;
}

void click_seven(){
    col = '7';
    std::cout << "Interrupt on column 7 (GPIO " << seven_pin << ")" << std::endl;
}

void click_eight(){
    col = '8';
    std::cout << "Interrupt on column 8 (GPIO " << eight_pin  << ")" << std::endl;
}

void drive_motors(char motor_control){
    //output motor code
    digitalWrite(m1,motor_control & 1);
    digitalWrite(m2,(motor_control >> 1) & 1);
    digitalWrite(m3,(motor_control >> 2) & 1);
    digitalWrite(m4,(motor_control >> 3) & 1);
    digitalWrite(m5,(motor_control >> 4) & 1);
    digitalWrite(m6,(motor_control >> 5) & 1);
    
    while(!digitalRead(confirm_pin)){
        std::cout << "vending..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    //reset to default
    digitalWrite(m1, 0);
    digitalWrite(m2, 0);
    digitalWrite(m3, 0);
    digitalWrite(m4, 0);
    digitalWrite(m5, 0);
    digitalWrite(m6, 0);
}

int main(){
    char motor_control = 0b0000001;
    drive_motors(motor_control);
}