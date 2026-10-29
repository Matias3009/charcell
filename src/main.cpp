#include <iostream>
#include <thread>
#include <chrono>

#include "display.hpp"
#include "connection.hpp"

#define BAUD 115200

std::vector<itas109::SerialPortInfo> ports;
std::vector<std::string> ports_name;
display main_dis;
itas109::CSerialPort serialport;
char buffer[1024];
bool reading_port;
std::thread* t_read;

void open_port(bool n);
void read_loop();

void open_port(bool n){
  if (!n)
    return;
  std::cout << main_dis.port_c << std::endl;
  serialport.init(main_dis.port_c.c_str(), BAUD);
  serialport.setOperateMode(itas109::SynchronousOperate);
  if (!serialport.open()){
    std::cerr << "- No se pudo abrir el puerto, intente otro o revisa los grupos/privilegios de usuario en (Linux/Windows)!"
	      << std::endl;
    return;
  }

  main_dis.start_plotting();

  t_read = new std::thread(read_loop);
  t_read->detach();
  
  return;
}

void read_loop(){
  reading_port = true;
  while (reading_port){
    int bytes_read = serialport.readData(buffer, sizeof(buffer) -1);
    if (bytes_read > 0){
      buffer[bytes_read] = '\n';
      std::cout << buffer << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main(int argc, char* argv[]){
  
  get_ports_esp(&ports);
  int port_number = 0;

  for (std::size_t p = 0; p < ports.size(); p++){ // Imprime los puertos
    std::cout << ports.at(p).portName <<
      ": " << ports.at(p).description << std::endl;
    ports_name.push_back(ports.at(p).portName);
    if (std::string_view(ports.at(p).portName) == TYPESERIAL)
      port_number = p;
  }

  // --------------------------

  if (!main_dis.initialize()){
    return -1;
  }

  // -------------------------

  main_dis.set_plot(new std::vector<data_c>({
	{100,0},
	{200,10},
	{150,20},
	{300,25}
      }),
    500,
    100);

  main_dis.buttons.at(1).call = open_port;

  main_dis.set_ports(ports_name, port_number);

  // --------------------------
  
  main_dis.main_loop();
  if (serialport.isOpen())
    serialport.close();
  
  reading_port = false;
  
  std::cout << "terminated" << std::endl;
  return 0;
}
