#include <SdFat.h>
#include <SPI.h>
#define SD_CONFIG  SdioConfig(FIFO_SDIO)
SdFat SD;

double table[3][10]{
  {
    0.0000000E+00,
    2.5173462E+01,
    -1.1662878E+00,
    -1.0833638E+00,
    -8.9773540E-01,
    -3.7342377E-01,
    -8.6632643E-02,
    -1.0450598E-02,
    -5.1920577E-04,
    0.0000000E+00},

  {
    0.000000E+00,
    2.508355E+01,
    7.860106E-02,
    -2.503131E-01,
    8.315270E-02,
    -1.228034E-02,
    9.804036E-04,
    -4.413030E-05,
    1.057734E-06,
    -1.052755E-08
  },

  {
    -1.318058E+02,
    4.830222E+01,
    -1.646031E+00,
    5.464731E-02,
    -9.650715E-04,
    8.802193E-06,
    -3.110810E-08,
    0.000000E+00,
    0.000000E+00,
    0.000000E+00
  }
};

void setup(){
  Serial.begin(115200);
  Serial4.begin(115200);
  Serial5.begin(57600);

  if (!SD.begin(SD_CONFIG)){
    Serial.println("initialization failed!");
    return;
  }
  FsFile file;
  file = SD.open("0.csv", FILE_WRITE);
  while (!Serial.available());
  file.println(Serial.readString());
  file.close();
}

void loop(){
  String incomingByte;

  if (Serial.available() > 0){
    incomingByte = Serial.readString();
    Serial.print("USB received: ");
    Serial.println(incomingByte);
    Serial5.println(incomingByte);
    Serial4.println(incomingByte);
  }
  if (Serial5.available() > 0){
    incomingByte = Serial5.readString();
    Serial.print("keithley received: ");
    Serial.println(incomingByte);
  }
  if (Serial4.available() > 0){
    incomingByte = Serial4.readString();
    Serial.print("GWinstek received: ");
    Serial.println(incomingByte);
  }
}

double mv_to_C(double val){
  int range;
  if (val >= -5.891 && val <= 0.0){
    range = 0;
  } else if (val > 0.0 && val <= 20.644){
    range = 1;
  } else if (val > 20.644 && val <= 54.886){
    range = 2;
  } else{
    return -999;
  }
  double temp_c;
  for (int i = 0; i < 10; i++){
    temp_c = temp_c + table[range][i] * pow(val, i);
  }
}
