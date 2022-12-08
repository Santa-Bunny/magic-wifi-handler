
#include <tins/tins.h>
#include "../magicWiFi.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>



using namespace Tins;



struct foo {
    int sniff_amount = 0;

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

        std::string payload_msg = magicwifi::payload_to_string(rpdu.payload());

        if (strcmp(payload_msg.c_str(), "Ack") == 0){
            std::cout << "--> Ack" << std::endl; 
        }
        else {
            std::cout << "--> " << dot_priority << " ";
            std::cout << payload_msg << std::endl;
            magicwifi::SendPacket(dot_priority, magicwifi::ack_reply);
        }

        if (sniff_amount > 1000) return false;
        return true;
    }
    void bar() {
        SnifferConfiguration conf;
        conf.set_promisc_mode(true);
        conf.set_filter("port 13");
        conf.set_immediate_mode(1);

        Sniffer sniff("eth0", conf);
        /* Uses the helper function to create a proxy object that
         * will call this->handle. If you're using boost or C++11,
         * you could use boost::bind or std::bind, that will also
         * work.
         */
        sniff.sniff_loop(make_sniffer_handler(this, &foo::priority_sniff));
    }
    
    bool handle(PDU&) {
        // Don't process anything
        return false;
    }
};


int main(int argc, char* argv[]) {
    foo f;
    f.bar();
}
// // same as above, just shorter
// EthernetII eth = EthernetII("77:22:33:11:ad:ad", info.hw_addr) / 
//                  IP("0.0.0.0", info.ip_addr) /
//                  TCP(13, 15) /
//                  RawPDU("I'm a payload!");