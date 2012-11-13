/* -*- Mode:C++; c-file-style:"microsoft"; indent-tabs-mode:nil; -*- */

#include "ns3/packet.h"

#include "tocino-flow-control.h"
#include "tocino-flit-header.h"

namespace ns3
{

template< TocinoFlowControl::State TFCS >
Ptr< Packet> TocinoFlowControl::GetPacketHelper()
{
    static Ptr<Packet> p = NULL;

    if( p == NULL )
    {
        uint8_t DATA = TFCS;
        p = Create<Packet>( &DATA, sizeof(DATA) );

        TocinoFlitHeader h; // src, dest?
        h.SetHead();
        h.SetTail();
        h.SetType( TocinoFlitHeader::LLC );

        p->AddHeader(h);
    }

    return p;
}

template< TocinoFlowControl::State TFCS >
bool TocinoFlowControl::TestPacketHelper( Ptr< Packet> pkt )
{
    // Make a copy so we can safely RemoveHeader() later
    Ptr<Packet> p = pkt->Copy();

    // We *must* remove, not merely peek, otherwise 
    // subsequent CopyData() will return header & payload
    TocinoFlitHeader h;
    p->RemoveHeader(h);

    if( h.GetType() == TocinoFlitHeader::LLC )
    {
        NS_ASSERT( h.IsHead() );
        NS_ASSERT( h.IsTail() );

        uint8_t DATA;

        p->CopyData( &DATA, sizeof(DATA) );

        return ( DATA == TFCS );
    }

    return false;
}

Ptr< Packet> TocinoFlowControl::GetXONPacket()
{
    return GetPacketHelper<XON>();
}

Ptr< Packet> TocinoFlowControl::GetXOFFPacket()
{
    return GetPacketHelper<XOFF>();
}

bool TocinoFlowControl::IsXONPacket( Ptr< Packet> p ) // const?
{
    return TestPacketHelper<XON>( p ); 
}

bool TocinoFlowControl::IsXOFFPacket( Ptr< Packet> p ) // const?
{
    return TestPacketHelper<XOFF>( p ); 
}

}