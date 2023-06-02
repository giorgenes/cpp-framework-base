COMPONENTS := stream \
	except \
	ios \
	io \
	utils \
	stdstream \
	iniparser \
	bxtp \
	rcpp \
	netstream \
	stfactory \
	stdsrvi \
	memory \
	strfile \
	table \
	metadir \
#	cgi

.PHONNY: all
all: 
	@for i in `echo $(COMPONENTS)`; do \
		make -sC $$i/src || exit 1;\
	done
	make -sC services

.PHONNY: clean
clean:
	make -sC services clean
	@for i in `echo $(COMPONENTS)`; do \
		make -sC $$i/src clean; \
	done

