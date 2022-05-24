#include <SdFat.h>
#include <SPI.h>
SdFat SD;
#define SD_CONFIG  SdioConfig(FIFO_SDIO)

void setup() {
  Serial.begin(115200);
	Serial4.begin(115200);
	Serial5.begin(115200);

  if (!SD.begin(SD_CONFIG)) {
    Serial.println("initialization failed!");
    return;
  }
  FsFile file;
  file = SD.open("0.csv", FILE_WRITE);
  while(!Serial.available());
  file.println(Serial.readString());
  file.close();
}

void loop() {
        String incomingByte;
       
 if (Serial.available() > 0) {
   incomingByte = Serial.readString();
    Serial.print("USB received: ");
    Serial.println(incomingByte);
    Serial5.println(incomingByte);
    Serial4.println(incomingByte);
  }
	if (Serial5.available() > 0) {
		incomingByte = Serial5.readString();
		Serial.print("device received: ");
		Serial.println(incomingByte);
		Serial5.print("UART received:");
		Serial5.println(incomingByte);
	}
}
