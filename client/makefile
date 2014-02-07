all:client.o

client.o:Messages.o Register.o
	g++ Client.cpp Messages.o Register.o -o client.o

Messages.o:
	g++ -c Messages.cpp

Register.o:
	g++ -c Register.cpp

clean:
	rm -rf *.o

mrproper: clean
	rm -rf ${EXEC}
