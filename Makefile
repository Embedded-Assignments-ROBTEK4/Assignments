DIRS= $(filter-out ./, $(sort $(dir $(wildcard ./*/))))

all::
	@echo $(DIRS)
	@for i in ${DIRS};                  \
	 do                                 \
	     if [ -f $${i}/Makefile ];      \
	     then                           \
	         make -C $${i} -j4 || exit $$?; \
	     fi;                            \
	 done

clean::
	@for i in ${DIRS};             \
	 do                            \
	     if [ -f $${i}/Makefile ]; \
	     then                      \
	         make -C $${i} clean -j4;  \
	     fi;                       \
	 done
