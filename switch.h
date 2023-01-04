#ifndef COMPNET_LAB4_SRC_SWITCH_H
#define COMPNET_LAB4_SRC_SWITCH_H

#include "types.h"
#include <map>

class SwitchBase {
 public:
  SwitchBase() = default;
  ~SwitchBase() = default;

  virtual void InitSwitch(int numPorts) = 0;
  virtual int ProcessFrame(int inPort, char* framePtr) = 0;
};

extern SwitchBase* CreateSwitchObject();

// TODO : Implement your switch class.
struct port_counter {
  int port;
  int counter;
};

class Switch : public SwitchBase {
private:
  int numPorts;
  std::map<uint64_t, port_counter> switch_table;  
public:
  void InitSwitch(int numPorts);
  int ProcessFrame(int inPort, char* framePtr);
  uint64_t convert_addr(mac_addr_t ether_addr);
};

#endif  // ! COMPNET_LAB4_SRC_SWITCH_H
