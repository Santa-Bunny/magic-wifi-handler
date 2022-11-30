
#include <tins/tins.h>
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

#define DROP        0x1
#define NO_DROP     0x0
#define HIGH_PRIO   0x1
#define LOW_PRIO    0x0

using namespace Tins;

int main(int argc, char* argv[]) {


    if (argc == 1 || argv[1][1] == 'h'){
        std::cout << "correct use: " << argv[0] << " <MAC Address>\n\n";
        std::cout << "write all messages in forms of <priority bit> <message>\n"
            << "For example:\n1 high priority message!\n0 low priority message\n\n";
        std::cout << "every sent message that makes it to it's destination should receive an\n" 
            << "-->Ack message. As well all messages received will be displayed with -->\n";
        return 0;
    }
    

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
    EthernetII eth(std::string(argv[1]), info.hw_addr);
    
    /* Create an IP PDU, with 192.168.0.1 as the destination address
     * and the default interface's IP address as the sender.
     */
    eth /= IP("0.0.0.0", info.ip_addr);
    
    /* Create a TCP PDU using 13 as the destination port, and 15 
     * as the source port.
     */
    eth /= TCP(13, 15);
    
    Dot1Q pdu = Dot1Q();
    pdu.id(0x00);

    std::stringstream s;
    bool priority;
    std::string message = "test";
    
    std::getline(std::cin, s);
    
    if (s[0] != 'q'){
        s >> priority;
        std::getline(s, message);
    }
    else {
        message = 'q';
    }

    

    while(message[0] != 'q') {

        pdu.cfi(NO_DROP);
        pdu.priority(HIGH_PRIO);
        RawPDU* rpdu = new RawPDU(message);
        pdu.inner_pdu(rpdu);

        /* Create a RawPDU containing the string "I'm a payload!".
        */
        eth /= pdu;
        
        // The actual sender
        PacketSender sender;
        
        // Send the packet through the default interface
        sender.send(eth, iface);
        std::getline(std::cin, s);
    
        if (s[0] != 'q'){
            s >> priority;
            std::getline(s, message);
        }
        else {
            message = 'q';
        }
    }
}
// // same as above, just shorter
// EthernetII eth = EthernetII("77:22:33:11:ad:ad", info.hw_addr) / 
//                  IP("0.0.0.0", info.ip_addr) /
//                  TCP(13, 15) /
//                  RawPDU("I'm a payload!");