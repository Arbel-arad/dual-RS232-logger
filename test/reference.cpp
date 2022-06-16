#include<iostream>
#include<math.h>
#include <vector>

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
  return temp_c;
}

float get_c(float val){

  std::cin >> val;
  int range;
  if (val >= -5.891 && val <= 0.0){
    range = 0;
    std::cout << "range 0";
  } else if (val > 0.0 && val <= 20.644){
    range = 1;
    std::cout << "range 1";
  } else if (val > 20.644 && val <= 54.886){
    range = 2;
    std::cout << "range 2";
  } else{
    return -999;
  }
  std::cout << std::endl;
  double temp_c;
  for (int i = 0; i < 10; i++){
    temp_c = temp_c + table[range][i] * pow(val, i);
  }
  std::cout << temp_c << std::endl;
  return temp_c;
};




std::vector<float> read;
std::vector<float> short_read;
int avg(){
  float val = 0;
  float sum = 0;
  float short_sum = 0;
  float delta_v = 5;
  float step = 10, set_step = 10;
  int step_max = 20;
  while (true){
    std::cin >> val;
    short_read.push_back(val);
    read.push_back(val);
    while (read.size() > 15){
      read.erase(read.begin());
    }
    while (short_read.size() > 3){
      short_read.erase(short_read.begin());
    }
    for (int i = 0; i < read.size(); ++i){
      sum = sum + read[i];
      if (i < short_read.size()){
        short_sum = short_sum + short_read[i];
        std::cout << "read: " << short_read[i] << " size " << short_read.size() << std::endl;
      }
    }
    float short_mov_avg = short_sum / short_read.size();
    float mov_avg = sum / read.size();
    short_sum = 0;
    sum = 0;
    step = set_step;
    float delta_avg = short_mov_avg - mov_avg;
    float power = delta_v / fabsf(delta_avg);
    float temp_step = step * power;
    if (temp_step <= step_max){
      step = temp_step;
    } else{
      step = step_max;
    }
    std::cout << "avg - " << mov_avg << " shortAVG: " << short_mov_avg << " step: " << step << std::endl;
  }
  return step;
}



int main(){
  while (true){
    avg();
    std::cout << "enter voltage value" << std::endl;
    float b;
    std::cin >> b;
    float a = mv_to_c(b);
    std::cout << a << std::endl;
  }
}

