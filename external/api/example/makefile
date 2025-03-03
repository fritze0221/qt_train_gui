
WINT_DIR= ../src_winterface

test: test.o winterface.o winterface-api.o
	g++ -o test test.o  winterface.o winterface-api.o

test.o: test.cpp
	g++ -c test.cpp -o test.o

winterface.o: $(WINT_DIR)/winterface.cpp $(WINT_DIR)/winterface.h  
	g++ -c $(WINT_DIR)/winterface.cpp -o winterface.o -D MAINBOARD -D TRAIN

winterface-api.o: $(WINT_DIR)/winterface-api.cpp $(WINT_DIR)/winterface-api.h  
	g++ -c $(WINT_DIR)/winterface-api.cpp -o winterface-api.o


.PHONY: clean
clean:
	rm *.o
	rm test

