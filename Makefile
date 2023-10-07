#Makefile

target=pa
objects=pa.o myfunctions.o

$(target): $(objects)
	gcc -o $@ $^
pa.o: pa.c
	gcc -c $^
myfunctions.o: myfunctions.c
	gcc -c $^
clean:
	rm $(objects) $(target)

