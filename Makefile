CXX=g++
CXXFLAGS=-std=c++17 -Wall -Werror

client: client.c requests.c helpers.c buffer.c library_api.cpp
	$(CXX) -o client client.c requests.c helpers.c buffer.c library_api.cpp $(CXXFLAGS)

run: client
	./client

clean:
	rm -f *.o client
