SATSolver:	satsolver.o probleminstance.o clause.o literal.o
	g++ -lstdc++ -o SATSolver -g  satsolver.o probleminstance.o clause.o literal.o

Experiment:	randinst.o probleminstance.o clause.o literal.o
	g++ -lstdc++ -o Experiment -g randinst.o probleminstance.o clause.o literal.o
	
randinst.o:	randinst.cpp
	g++ -Wall -g -c randinst.cpp
		
satsolver.o:	satsolver.cpp
	g++ -Wall -g -c satsolver.cpp
	
probleminstance.o:	probleminstance.cpp
	g++ -Wall -g -c probleminstance.cpp
	
clause.o:	clause.cpp
	g++ -Wall -g -c clause.cpp
	
literal.o:	literal.cpp
	g++ -Wall -g -c literal.cpp

clean:
	rm -f *.o SATSolver
