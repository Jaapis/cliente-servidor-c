server: server.cpp
		hostname -I
		g++ server.cpp
		./a.out

client: client.cpp
		g++ client.cpp
		./a.out
