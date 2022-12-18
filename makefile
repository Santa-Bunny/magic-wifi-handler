# This makefile is mainly for testing and compiling example sources. 
# There is a magic-wifi build to make a static library but
# libtins refers to main and there is no main

text_transfer:
	g++ lib/examples/text_transfer.cpp -std=c++11 -g3 -ltins -o bin/text_transfer

receiving:
	g++ lib/examples/receiving.cpp -std=c++11 -g3 -ltins -o bin/receiving

sending:
	g++ lib/examples/sending.cpp -std=c++11 -g3 -ltins -o bin/sending

magic-wifi:
	g++ lib/magicWiFi.hpp	-std=c++11 -o3 -ltins -o bin/magic-wifi.o