#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <InternalTemperature.h>

#define SD_CONFIG  SdioConfig(FIFO_SDIO)
SdFat SD;
FsFile file;                                    //create interface
String filename_data = "test.csv", filename_info = "test.txt";
String wire_mode = "4", compliance_v = "21", curr_min = "0.0001", curr_max = "0.005", curr_step = "0.0001", interval_c = "1";
int step = 0, step_end = 10;
float temp_init = 0, temp_prev = 0, temp_now = 0;
bool is_measuring = false;

void send_to(String addr, String data){
  if (addr == "4"){
    Serial4.println(data);
  } else if (addr == "5"){
    Serial5.println(data);
  } else if (addr == "both"){
    Serial4.println(data);
    Serial5.println(data);
  } else{
    Serial.println("address: " + addr + "not found");
    return;
  }
  Serial.printf("sent: %s to %s\n", data.c_str(), addr.c_str());
}

String get_range(String val){
  float i = val.toFloat();
  if (i <= 0.1){
    if (i <= 0.01){
      if (i <= 0.001){
        if (i <= 0.0001){
          if (i <= 0.00001){
            return "0.00001";
          } else{
            return "0.0001";
          }
        } else{
          return "0.001";
        }
      } else{
        return "0.01";
      }
    } else{
      return "0.1";
    }
  } else{
    return "1";
  }
}

void set_current(String current){
  Serial5.println(":SYSTem:KEY 19");
  //Serial5.println("SOUR:CURR:RANG " + get_range(current));
  Serial5.println("SOUR:CURR:RANG:AUTO ON");
  current += "E-3";
  Serial5.println("SOUR:CURR " + current);
  return;
}

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
  float temp_c = mv_to_c(mv);// +InternalTemperature.readTemperatureC(); //compensate for room temperature with internal sensor
  return temp_c;
}

void serialEvent(){
  String command = Serial.readStringUntil(' ');
  if (command == "restart"){
    Serial.println("restarting");
    _reboot_Teensyduino_();
  } else if (command == "help"){
    Serial.println("command list:\nsetup [file name] [sample id] [date]\n[compliance (V)] [current min (mA)] [current max (mA)] [current step (mA)] [interval (C)]\nsend [addr] [data]\n$addr [4 / 5 / both]\n ----\n");
  } else if (command == "setup"){
    Serial.println("setup - [file_name] [sample_id] [date]");
    while (!Serial.available());                                    //wait for response from user
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
    Serial.println("setup - [compliance_v ~21V] [curr_min ~100uA] [curr_max ~10mA] [curr_step ~100uA] [interval_c ~2C]");
    while (!Serial.available());
    compliance_v = Serial.readStringUntil(' ');
    curr_min = Serial.readStringUntil(' ');
    curr_max = Serial.readStringUntil(' ');
    curr_step = Serial.readStringUntil(' ');
    interval_c = Serial.readStringUntil(' ');  //read all the settings
    temp_init = get_temp_c();
    String temp_temp = temp_init;
    Serial.println(compliance_v + " " + curr_min + " " + curr_max + " " + curr_step + " " + interval_c);
    file.println("setup - [compliance_v] [curr_min] [curr_max] [curr_step] [interval_c]");
    file.println("setup - " + compliance_v + " " + curr_min + " " + curr_max + " " + curr_step + " " + interval_c);
    file.println("initial temperature: " + temp_temp);
    file.close();                              //close file
    filename_data = val1 += ".csv";
    Serial5.println("FORM:ELEM?");
    Serial5.println("SYST:BEEP:STAT OFF");
    while (!Serial5.available());
    file = SD.open(filename_data, FILE_WRITE); //open file
    file.println("TEMP," + Serial5.readString());        //write format to header
    if (wire_mode == "4"){                     //set 4wire mode
      Serial5.println("SYST:RSEN ON");
    } else{
      Serial5.println("SYST:RSEN OFF");
    }
    float temp_steps = curr_max.toFloat() - curr_min.toFloat();
    temp_steps = temp_steps / curr_step.toFloat();
    step_end = temp_steps + 0.9999999999999999;
    set_current(curr_min);
    Serial5.println("SENS:VOLT:PROT:LEV " + compliance_v);
    Serial5.println(":CONF:VOLT:DC");
    file.close();                              //close file
    Serial.println("files written - " + filename_data + " : " + filename_info);
    is_measuring = true;
  } else if (command == "send"){
    String addr = Serial.readStringUntil(' ');
    String data = Serial.readString();
    send_to(addr, data);
  } else if (command == "stop"){
    is_measuring = false;
  }
}

String measure(){
  Serial5.println("READ?");
  while (!Serial5.available());
  return Serial5.readString();
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
  delay(1000);
  while (com_state > 0){
    while (Serial4.available() > 0){
      Serial.println(Serial4.readString());
      com_state = com_state - 1;
      Serial4.read();
    }
    while (Serial5.available() > 0){
      Serial.println(Serial5.readString());
      com_state = com_state - 1;
      Serial5.read();
    }
    Serial.printf("comstate = %d\n", com_state);
    delay(100);
  }
  Serial.println("connected!");
}

void loop(void){
  if (is_measuring == true){
    temp_now = get_temp_c();
    float temp_diff = temp_now;
    temp_diff = temp_diff - temp_prev;
    temp_prev = temp_now;
    Serial.print(temp_now);
    Serial.print("-");
    Serial.println(temp_diff);
    if (temp_diff >= interval_c.toFloat() || temp_diff <= -interval_c.toFloat()){
      for (step = 1; step <= step_end; step++){
        float current_now = step * curr_step.toFloat();
        String print_current = current_now;
        String print_temp = get_temp_c();
        set_current(print_current);
        Serial.print(step);
        Serial.print(" of ");
        Serial.println(step_end);
        String value = measure();
        Serial.println(" current: " + print_current + " --> " + value);
        file = SD.open(filename_data, FILE_WRITE);
        file.println(print_temp + "," + value);
        file.close();
      }
    }
  }
  if (Serial4.available())Serial.println(Serial4.readString());
  if (Serial5.available())Serial.println(Serial5.readString());
}