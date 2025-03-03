
WINT_DIR= ../src_winterface

main: main.o dummy.o winterface.o
	g++ -o main main.o dummy.o winterface.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

dummy.o: dummy.cpp dummy.h
	g++ -c dummy.cpp -o dummy.o

winterface.o: $(WINT_DIR)/winterface.cpp $(WINT_DIR)/winterface.h  
	g++ -c $(WINT_DIR)/winterface.cpp -o winterface.o -D MAINBOARD -D TRAIN


.PHONY: clean
clean:
	rm *.o
	rm main
