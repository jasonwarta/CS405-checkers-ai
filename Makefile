default:
	g++ -std=c++14 server.cpp -O3 -lz -lssl -luv -luWS -o server