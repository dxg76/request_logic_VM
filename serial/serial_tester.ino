void setup() {
  Serial.begin(9600); //set baud
  pinMode(13, OUTPUT);
}

void loop() {
  int num = Serial.available();
  if(num>0){
    String received = Serial.readStringUntil('\n');  // Read until newline
    Serial.print("Arduino received: ");
    Serial.println(received);

    if (received == "PING") {  // Respond to a specific command
      Serial.println("PONG");  // Send back "PONG" to Raspberry Pi
    }
  }
}

