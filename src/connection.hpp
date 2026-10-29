#pragma once

#include <vector>

#include <CSerialPort/SerialPort.h>
#include <CSerialPort/SerialPortInfo.h>

#define TYPESERIAL "cp210x"

extern void get_ports_esp(std::vector<itas109::SerialPortInfo>* ports);
