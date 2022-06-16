#include <iostream>
#include <vector>
#include <math.h>

std::vector<float> read;
std::vector<float> short_read;
int mainl(){
  while (true){
    float val = 0;
    float sum = 0;
    float short_sum = 0;
    float delta_v = 5;
    float step = 10;
    int step_max = 20;
    std::cin >> val;
    short_read.push_back(val);
    read.push_back(val);
    if (read.size() > 10){
      read.erase(read.begin());
    }
    if (short_read.size() > 3){
      short_read.erase(short_read.begin());
    }
    for (int i = 0; i < read.size(); ++i){
      std::cout << "read: " << read[i] << " size " << read.size() << std::endl;
      sum = sum + read[i];
      if (i < 3){
        short_sum = sum + short_read[i];
      }
    }
    float short_mov_avg = short_sum / short_read.size();
    float mov_avg = sum / read.size();
    short_sum = 0;
    sum = 0;
    float delta_avg = short_mov_avg - mov_avg;
    float power = delta_v / fabsf(delta_avg);
    float temp_step = step * power;
    if (temp_step <= step_max){
      step = temp_step;
    } else{
      step = step_max;
    }
    std::cout << "avg - " << mov_avg << " step: " << step << std::endl;
  }
}