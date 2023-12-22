est.d: ../../include/exec.h 
../../include/exec.h: exec.h
	@rm -f ../../include/exec.h
	@ln -fs ../src/est/exec.h ../../include/exec.h
