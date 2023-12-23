sim.d: ../../include/simulate.h 
../../include/simulate.h: simulate.h
	@rm -f ../../include/simulate.h
	@ln -fs ../src/sim/simulate.h ../../include/simulate.h
