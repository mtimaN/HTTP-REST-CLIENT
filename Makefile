CXX=g++
CXXFLAGS=-std=c++17 -Wall -Werror

client: client.cpp requests.cpp helpers.cpp buffer.c library_api.cpp
	$(CXX) -o client client.cpp requests.cpp helpers.cpp buffer.c library_api.cpp $(CXXFLAGS)

run: client
	./client

clean:
	rm -f *.o client
