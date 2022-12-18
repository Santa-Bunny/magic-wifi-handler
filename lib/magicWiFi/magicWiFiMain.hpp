#ifndef MAGIC_WIFI_MAIN_H
#define MAGIC_WIFI_MAIN_H
#include <tins/tins.h>
#include <iostream>


#define DROP            0x1
#define NO_DROP         0x0
#define HIGH_PRIORITY   0x1
#define LOW_PRIORITY    0x0
namespace magicwifi {
    std::string payload_to_string(Tins::RawPDU::payload_type payload){
        std::string str;
        for (size_t i = 0; i < payload.size(); i++)
        {
            str += payload[i];
        }
        
        return str;
    }

    
    std::string ack_reply("Ack");
    std::string MAC_addr;


    void SendPacket(bool priority, std::string& message) {
            // We'll use the default interface(default gateway)
        Tins::NetworkInterface iface = Tins::NetworkInterface::default_interface();
        
        /* Retrieve this structure which holds the interface's IP, 
        * broadcast, hardware address and the network mask.
        */
        Tins::NetworkInterface::Info info = iface.addresses();
        
        /* Create an Ethernet II PDU which will be sent to 
        * 77:22:33:11:ad:ad using the default interface's hardware 
        * address as the sender.
        */
        Tins::EthernetII eth(MAC_addr, info.hw_addr);
        
        /* Create an IP PDU, with 0.0.0.0 as the destination address
        * and the default interface's IP address as the sender.
        */
        eth /= Tins::IP("0.0.0.0", info.ip_addr);
        
        /* Create a TCP PDU using 13 as the destination port, and 15 
        * as the source port.
        */
        eth /= Tins::TCP(13, 15);
        
        Tins::Dot1Q pdu = Tins::Dot1Q();
        pdu.id(0x00);

        if (priority){
            pdu.cfi(NO_DROP);
            pdu.priority(HIGH_PRIORITY);
        }
        else {
            pdu.cfi(DROP);
            pdu.priority(LOW_PRIORITY);
        }
        Tins::RawPDU* rpdu = new Tins::RawPDU(message);
        pdu.inner_pdu(rpdu);

        /* packet loaded into sending
        */
        eth /= pdu;
        // The actual sender
        Tins::PacketSender sender;
        
        // Send the packet through the default interface
        sender.send(eth, iface);

    }

    struct Priority_Sniff{
        int MAX_SNIFF = 1000;
        int sniff_amount = 0;
        bool sniff(Tins::PDU & pdu){
            // Search for it. If there is no Dot1Q PDU in the packet, 
            // the loop goes on
            bool dot_priority = 0;
            try {
                const Tins::Dot1Q &dot = pdu.rfind_pdu<Tins::Dot1Q>(); // non-const works as well
                dot_priority = dot.priority();
            }
            catch (const Tins::pdu_not_found &e) {
                return true;
            }
            

            sniff_amount++;
            const Tins::RawPDU &rpdu = pdu.rfind_pdu<Tins::RawPDU>();

            std::string payload_msg = payload_to_string(rpdu.payload());

            if (strcmp(payload_msg.c_str(), "Ack") == 0){
                std::cout << "--> Ack" << std::endl; 
            }
            else {
                std::cout << "--> " << dot_priority << " ";
                std::cout << payload_msg << std::endl;
                SendPacket(dot_priority, ack_reply);
            }

            if (sniff_amount > MAX_SNIFF) return false;
            return true;
        }

    };

}




#endif