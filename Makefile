run:
	$(CXX) main.cpp -o main -lraylib

generate:
	bear -- make

clean:
	rm -f main
	
