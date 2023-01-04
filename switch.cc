#include "switch.h"
#include <cstdio>

SwitchBase* CreateSwitchObject() {
  // TODO : Your code.
  return new Switch();
}

void Switch::InitSwitch(int numPorts){
  this->numPorts = numPorts;
  this->switch_table.clear();
}

int Switch::ProcessFrame(int inPort, char* framePtr){
  ether_header_t* ether_frame = new ether_header_t;
  memcpy(ether_frame, framePtr, sizeof(ether_header_t));
  uint64_t ether_dest = convert_addr(ether_frame->ether_dest);
  uint64_t ether_src = convert_addr(ether_frame->ether_src);
  uint16_t ether_type = ether_frame->ether_type;
  uint64_t ether_len = ether_frame->length;
  free(ether_frame);

  if(ether_type == ETHER_DATA_TYPE){
    // Insert new entry.
    if(this->switch_table.find(ether_src) == this->switch_table.end()){
      port_counter pc{inPort, ETHER_MAC_AGING_THRESHOLD};
      this->switch_table.insert({ether_src, pc});
    }
    // Update entry.
    else{
      this->switch_table[ether_src].counter = ETHER_MAC_AGING_THRESHOLD;
    }

    if(this->switch_table.find(ether_dest) != this->switch_table.end()){
      int outPort = this->switch_table[ether_dest].port;
      if(outPort != inPort){
        return outPort;
      }
      return -1;
    }
    return 0;
  }
  else if(ether_type == ETHER_CONTROL_TYPE){
    for(auto& entry : this->switch_table){
      entry.second.counter -= 1;
      if(entry.second.counter <= ETHER_COMMAND_TYPE_AGING){
        uint64_t k = entry.first;
        this->switch_table.erase(k);
      }
    }
    return -1;
  }
  else{
    fprintf(stderr, "Error: Invalid ether_type.\n");
    return -1;
  }
}

uint64_t Switch::convert_addr(mac_addr_t ether_addr){
  uint64_t ret_addr = 0;
  for(int i = 0; i < ETH_ALEN; i++){
    ret_addr += ether_addr[i];
    ret_addr <<= 8;
  }
  return ret_addr;
}