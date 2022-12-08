#ifndef MAGIC_WIFI_H
#define MAGIC_WIFI_H
#include "magicWiFi/magicWiFiMain.hpp"
#include <tins/tins.h>
#include <iostream>

using namespace Tins;

#define DROP        0x1
#define NO_DROP     0x0
#define HIGH_PRIO   0x1
#define LOW_PRIO    0x0
namespace magicwifi {
    std::string payload_to_string(Tins::RawPDU::payload_type payload){
        std::string str;
        for (size_t i = 0; i < payload.size(); i++)
        {
            str += payload[i];
        }
        
        return str;
    }



    int sniff_amount = 0;
    std::string ack_reply("Ack");
    std::string MAC_addr;


    void SendPacket(bool priority, std::string& message) {
            // We'll use the default interface(default gateway)
        NetworkInterface iface = NetworkInterface::default_interface();
        
        /* Retrieve this structure which holds the interface's IP, 
        * broadcast, hardware address and the network mask.
        */
        NetworkInterface::Info info = iface.addresses();
        
        /* Create an Ethernet II PDU which will be sent to 
        * 77:22:33:11:ad:ad using the default interface's hardware 
        * address as the sender.
        */
        EthernetII eth(MAC_addr, info.hw_addr);
        
        /* Create an IP PDU, with 0.0.0.0 as the destination address
        * and the default interface's IP address as the sender.
        */
        eth /= IP("0.0.0.0", info.ip_addr);
        
        /* Create a TCP PDU using 13 as the destination port, and 15 
        * as the source port.
        */
        eth /= TCP(13, 15);
        
        Dot1Q pdu = Dot1Q();
        pdu.id(0x00);

        if (priority){
            pdu.cfi(NO_DROP);
            pdu.priority(HIGH_PRIO);
        }
        else {
            pdu.cfi(DROP);
            pdu.priority(LOW_PRIO);
        }
        RawPDU* rpdu = new RawPDU(message);
        pdu.inner_pdu(rpdu);

        /* packet loaded into sending
        */
        eth /= pdu;
        // The actual sender
        PacketSender sender;
        
        // Send the packet through the default interface
        sender.send(eth, iface);
    



    }

        bool priority_sniff(PDU & pdu){
        // Search for it. If there is no Dot1Q PDU in the packet, 
        // the loop goes on
        bool dot_priority = 0;
        try {
            const Dot1Q &dot = pdu.rfind_pdu<Dot1Q>(); // non-const works as well
            dot_priority = dot.priority();
        }
        catch (const pdu_not_found &e) {
            return true;
        }
        

        sniff_amount++;
        const RawPDU &rpdu = pdu.rfind_pdu<RawPDU>();

        std::string payload_msg = payload_to_string(rpdu.payload());

        if (strcmp(payload_msg.c_str(), "Ack") == 0){
            std::cout << "--> Ack" << std::endl; 
        }
        else {
            std::cout << "--> " << dot_priority << " ";
            std::cout << payload_msg << std::endl;
            SendPacket(dot_priority, ack_reply);
        }

        if (sniff_amount > 1000) return false;
        return true;
    }




}










#endif