run:
	$(CXX) main.cpp -o main -lraylib -I./include/

gen:
	bear -- make

clean:
	rm -f main
	
