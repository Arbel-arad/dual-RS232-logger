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


float step_now = 0.005, delta_v = 0.01;
float step_max = 2 * 0.01;
std::vector<float> short_read;
std::vector<float> read;

float get_step_dynamic(float val){
  float short_sum = 0;
  float sum = 0;
  short_read.push_back(val);
  read.push_back(val);

  while (read.size() > 15){
    read.erase(read.begin());
  }
  while (short_read.size() > 3){
    short_read.erase(short_read.begin());
  }
  for (uint8_t i = 0; i < read.size(); ++i){
    sum = sum + read[i];
    if (i < short_read.size()){
      short_sum = short_sum + short_read[i];
    }
  }
  float short_mov_avg = short_sum / short_read.size();
  float mov_avg = sum / read.size();
  short_sum = 0;
  sum = 0;
  step_now = 0.001;
  float delta_avg = short_mov_avg - mov_avg;
  std::cout << "short_mov_avg: " << short_mov_avg << " mov_avg: " << mov_avg << std::endl;

  float power = 1000 * delta_v / fabsf(delta_avg);
  power / 1000;
  float temp_step = step_now * power;
  if (temp_step <= step_max){
    step_now = temp_step;
  } else{
    step_now = step_max;
  }
  std::cout << "step_now: " << step_now << " power: " << power << " temp_step: " << temp_step << " delta_avg: " << fabsf(delta_avg) << std::endl;
  return step_now;
}


int main(){
  while (true){
    float val;
    std::cin >> val;
    get_step_dynamic(val);
  }
  std::cout << "enter voltage value" << std::endl;
  float b;
  std::cin >> b;
  float a = mv_to_c(b);
  std::cout << a << std::endl;

}

