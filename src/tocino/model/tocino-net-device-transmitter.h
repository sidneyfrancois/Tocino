/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __TOCINO_NET_DEVICE_TRANSMITTER_H__
#define __TOCINO_NET_DEVICE_TRANSMITTER_H__

#include <vector>
#include "ns3/object.h"

#include "tocino-enum.h"
#include "callback-queue.h"
#include "tocino-channel.h"
#include "tocino-net-device.h"
#include "tocino-net-device-receiver.h"

namespace ns3 {

class TocinoNetDeviceTransmitter : public Object
{
public:
  static TypeId GetTypeId(void);

  TocinoNetDeviceTransmitter();
  ~TocinoNetDeviceTransmitter() {};

  void SetXState(TocinoFlowControlState s) {m_xstate = s;}
  TocinoFlowControlState GetXState() {return m_xstate;}

  void SetChannel(Ptr<TocinoChannel> channel) {m_channel = channel;}
  void SendXOFF() {m_pending_xoff = true;}
  void SendXON() {m_pending_xon = true;}

  Ptr<NetDevice> GetNetDevice() { /*return m_tnd;*/ return NULL;}

  void Transmit();

  friend class TocinoNetDevice;
  
private:
  uint32_t m_channelNumber;

  TocinoFlowControlState m_xstate;
  Ptr<Packet> m_packet;

  enum TocinoTransmitterState {IDLE, BUSY};
  TocinoTransmitterState m_state;
  
  bool m_pending_xon;
  bool m_pending_xoff;

  Ptr<TocinoNetDevice> m_tnd; // link to owning TocinoNetDevice

  std::vector<Ptr<CallbackQueue> > m_queues; // links to queues

  Ptr<TocinoChannel> m_channel; // link to channel

  void TransmitEnd(); // can this be private? needs to be invoked by Simulator::Schedule()
  uint32_t Arbitrate();
};

}
#endif /* __TOCINO_NET_DEVICE_TRANSMITTER_H__ */
