text_transfer:
	g++ lib/examples/text_transfer.cpp -std=c++11 -g3 -ltins -o bin/text_transfer

receiving:
	g++ lib/examples/receiving.cpp -std=c++11 -g3 -ltins -o bin/receiving

sending:
	g++ lib/examples/sending.cpp -std=c++11 -g3 -ltins -o bin/sending