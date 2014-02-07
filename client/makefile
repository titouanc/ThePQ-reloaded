all:client.o

client.o:Messages.o Connexion.o
	g++ Client.cpp Messages.o Connexion.o -o client.o

Messages.o:
	g++ -c Messages.cpp

Connexion.o:
	g++ -c Connexion.cpp

clean:
	rm -rf *.o

mrproper: clean
	rm -rf ${EXEC}
