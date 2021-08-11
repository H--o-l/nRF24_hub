#include <Arduino.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

void setup() {
  radio.begin();

  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setCRCLength(RF24_CRC_8);
  radio.setChannel(125);
  radio.setAutoAck(1);
  radio.setPayloadSize(8);
  radio.setRetries(5,5);  // max 15, 15
  radio.setAddressWidth(3);
  radio.openWritingPipe(0x000002);
  radio.openReadingPipe(1, 0x000001);
  radio.startListening();
  pinMode(2, INPUT);
  delay(100);

  Serial.begin(9600);
}
void loop()
{
  if(digitalRead(2) == LOW) {  //if (radio.available())
    char radio_input;
    radio.read(&radio_input, sizeof(radio_input));
    if (radio_input == *"S" || radio_input == *"1" ||
        radio_input == *"2" || radio_input == *"3" ||
        radio_input == *"4" || radio_input == *"5" ||
        radio_input == *"5" || radio_input == *"6" ||
        radio_input == *"7" || radio_input == *"8" ||
        radio_input == *"9") {
      Serial.print(radio_input);
    }
    radio.flush_rx();
    delay(30);  // don't remove otherwise next send doesn't work
  }
  if (Serial.available()) {
    char byte = Serial.read();
    if (byte == *"F") {  // Fio is ok?
      Serial.print("F");  // Fio is OK
    } else if (byte == *"S") {  // Shield is ok?
      char text[8] = "S";
      radio.stopListening();
      radio.write(&text, sizeof(text));  // Send S to shield
      radio.startListening();
    } else if (byte == *"A") {  // ack
      char text[8] = "A";
      radio.stopListening();
      radio.write(&text, sizeof(text));  // Send A to shield
      radio.startListening();
    }
  }
}
