SERVER = SERVER
CLIENT = CLIENT

all :
	g++ client.cpp -o $(CLIENT)
	g++ server.cpp -o $(SERVER) 

clean :
	rm -rf $(CLIENT)
	rm -rf $(SERVER)

re : clean all