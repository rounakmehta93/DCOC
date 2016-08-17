CXX = /usr/local/bin/gcc-4.9
CXXFLAGS =  -std=c99 
run.exe: main.c globals.c test_pendulum.c pendulum_model.c interpolation.c
	$(CXX) $(CXXFLAGS) $^ -o $@
clean: 
	rm *.exe