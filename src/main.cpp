#include <iostream>

#include "display.hpp"

int main(int argc, char* argv[]){

  display main_dis;

  if (!main_dis.initialize()){
    return -1;
  }

  main_dis.set_plot(new std::vector<data_c>({
      {100,0},
      {200,10},
      {150,20}
      }),
    500,
    100);

  main_dis.main_loop();
  
  std::cout << "terminated" << std::endl;
  return 0;
}
