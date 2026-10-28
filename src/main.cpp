#include <iostream>

#include "display.hpp"
#include "connection.hpp"

void prueba(bool n){
  if (!n)
    return;
  std::cout << "boton" << std::endl;
}

std::vector<itas109::SerialPortInfo> ports;

int main(int argc, char* argv[]){

  display main_dis;
  itas109::CSerialPort serialport;

  get_ports(&ports);

  for (auto& p : ports){
    std::cout << p.portName << " : " << p.description << std::endl;
  }

  if (!main_dis.initialize()){
    return -1;
  }

  main_dis.set_plot(new std::vector<data_c>({
	{100,0},
	{200,10},
	{150,20},
	{300,25}
      }),
    500,
    100);

  main_dis.buttons.at(1).call = prueba;

  main_dis.main_loop();
  
  std::cout << "terminated" << std::endl;
  return 0;
}
