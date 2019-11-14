OBJ_FILES = main.o vamk_sdp.o vamk_socket.o vamk_rfcomm.o
OPTS = -std=c++11 -g

bt: $(OBJ_FILES)
	g++ $(OPTS) -o bt $(OBJ_FILES) -lbluetooth 

main.o: main.cpp
	g++ $(OPTS) -c main.cpp -o main.o

vamk_sdp.o: vamk_sdp.cpp vamk_sdp.h
	g++ $(OPTS) -c vamk_sdp.cpp -o vamk_sdp.o

vamk_rfcomm.o: vamk_rfcomm.cpp vamk_rfcomm.h
	g++ $(OPTS) -c vamk_rfcomm.cpp -o vamk_rfcomm.o

vamk_socket.o: vamk_socket.cpp vamk_socket.h
	g++ $(OPTS) -c vamk_socket.cpp -o vamk_socket.o

clean:
	rm -f *.o
