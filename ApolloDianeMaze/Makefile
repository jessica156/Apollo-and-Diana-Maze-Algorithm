CC = g++
CFLAGS = -Wall -I$BOOST_HOME -Wno-deprecated

ApolloDianeMaze: ApolloDianeMaze.o

.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f *.o ApolloDianeMaze output.txt