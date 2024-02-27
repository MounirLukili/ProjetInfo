# Compiler
CXX := g++
# Compilation options
CXXFLAGS := -std=c++11 -Wall -Wextra -ggdb
# Linker options
LDFLAGS := -lcbor

# Target executables
SERVER := server
CLIENT := client

# Default target
all : $(SERVER) $(CLIENT)

# Server dependencies
$(SERVER) : server.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Client dependencies
$(CLIENT) : client.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile server and client
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# Clean rule
clean : 
	rm -f *.o $(SERVER) $(CLIENT)

# Run server
run_server : $(SERVER)
	./$(SERVER)

# Run client
run_client : $(CLIENT)
	./$(CLIENT)

