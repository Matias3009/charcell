#pragma once

#include <vector>

#include <CSerialPort/SerialPort.h>
#include <CSerialPort/SerialPortInfo.h>

extern void get_ports(std::vector<itas109::SerialPortInfo>* ports);
