all.out: Karaoke_Rest.o
	g++ $^ -o $@ -lpthread

Karaoke_rest.o: Karaoke_Rest.cpp 
	g++ $^ -c

clean:
	rm -rf *.o *.out
