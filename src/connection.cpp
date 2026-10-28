#include "connection.hpp"

void get_ports(std::vector<itas109::SerialPortInfo>* ports)
{
  ports->clear();
  for (auto& p : itas109::CSerialPortInfo::availablePortInfos()){
    ports->push_back(p);
  }
  
}
