#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <InternalTemperature.h>

#define SD_CONFIG  SdioConfig(FIFO_SDIO)
SdFat SD;
FsFile file;                                    //create interface
String filename_data = "test.csv", filename_info = "test.txt";
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

double mv_to_c(float val){
  int range = 0;
  if (val >= -5.891 && val <= 0.0){
    range = 0;
  } else if (val > 0.0 && val <= 20.644){
    range = 1;
  } else if (val > 20.644 && val <= 54.886){
    range = 2;
  } else Serial.println("error - value is out of range");

  double temp_c = 0;
  for (int i = 0; i < 10; i++){
    temp_c = temp_c + table[range][i] * pow(val, i);
  }
  temp_c = temp_c * 1000;
  temp_c = temp_c + 25;
  return temp_c;
}

float get_temp_c(){
  Serial4.println("MEASure:VOLTage:DC?"); //send measurement command
  while (!Serial4.available()); //wait for response from device
  String reading = Serial4.readString();
  float mv = reading.toFloat();
  float temp_c = mv_to_c(mv) + InternalTemperature.readTemperatureC(); //compensate for room temperature with internal sensor
  return temp_c;
}

String wire_mode = "4", compliance_v = "21", curr_min = "0.0001", curr_max = "0.005", curr_step = "0.0001", interval_c = "1";

void serialEvent(){
  String command = Serial.readStringUntil(' ');
  if (command == "help"){
    Serial.println("command list:     \n ----");
  } else if (command == "setup"){
    Serial.println("setup - [file_name] [sample_id] [date]");
    while (!Serial.available());                                        //wait for response from user
    String val1, val2, val3;
    for (int i = 0; i < 3 && Serial.available(); i++){              //read user input
      val1 = Serial.readStringUntil(' ');
      val2 = Serial.readStringUntil(' ');
      val3 = Serial.readStringUntil(' ');
      Serial.println(val1 + " " + val2 + " " + val3);
    }
    filename_info = val1;
    filename_info += ".txt";
    file = SD.open(filename_info, FILE_WRITE); //open file
    file.println("sample_id: " + val2 + " date: " + val3);
    file.close();                              //close file
    filename_data = val1 += ".csv";
    Serial5.println("FORM:ELEM?");
    Serial5.println("SYST:BEEP:STAT OFF");
    while (!Serial5.available());
    file = SD.open(filename_data, FILE_WRITE); //open file
    file.println(Serial5.readString());
    file.close();                              //close file
    Serial.println("files written - " + filename_data + " : " + filename_info);
  }
}

void setup(void){
  Serial.begin(115200);                              //fast baud rate for pc com port
  Serial4.begin(115200);                             //fast baud rate for GWinstek
  Serial5.begin(57600);                              //slower baud rate for keithley
  while (!Serial);                                   //wait for serial connection
  if (!SD.begin(SD_CONFIG)){                         //start SD card interface
    Serial.println("SD initialization failed!");     //checkk SD initialization state
    return;
  }

  int com_state = 2;
  Serial4.println("*IDN?");
  Serial5.println("*IDN?");
  while (com_state > 0){
    if (Serial4.available() > 0){
      Serial.println(Serial4.readString());
      com_state = com_state - 1;
    }
    if (Serial5.available() > 0){
      Serial.println(Serial5.readString());
      com_state = com_state - 1;
    }
    Serial.printf("comstate = %d\n", com_state);
  }
  Serial.println("connected!");
}

void loop(void){
  /*float temp_c = get_temp_c();
  Serial.print(" - temp - ");
  Serial.println(temp_c, 5);*/

}