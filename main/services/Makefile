COMPONENTS := \
	db-srv-cli \
	db-srv \
	componentfsdb-srv\
	component-srv\
	component-srv-cli\
	component-srv-cli-cmd\
	postgre-srv\
	prod-srv\
	prod-srv-cli\
	prod-srv-cli-cmd\
	user-srv\
	user-srv-cli\
	user-srv-cli-cmd
	
.PHONNY: all
all: 
	@for i in `echo $(COMPONENTS)`; do \
		make -sC $$i/src || exit 1;\
	done

.PHONNY: clean
clean:
	@for i in `echo $(COMPONENTS)`; do \
		make -sC $$i/src clean; \
	done


