clean:
	rm *.o a.run

test: a.run
	./a.run data/1MSH.mmtf

mmtf_parser.o: mmtf_parser.c
	gcc -c mmtf_parser.c

demo.o: demo.c
	gcc -c demo.c

a.run: mmtf_parser.o demo.o
	gcc -o a.run mmtf_parser.o demo.o -lmsgpackc
