all: clean
	make -C stream/src
	make -C utils/src
	make -C stdstream/src
	make -C rcpp/src
	make -C user-srv-cli/src
	make -C user-srv-cli-cmd/src
	#make -C io/src
	#make -C ios/src
	#make -C pysscm/src
	#make -C user-srv-i/src
	#make -C user-srv/src


clean:
	make -C rcpp/src clean
	make -C stdstream/src clean
	make -C stream/src clean
	make -C user-srv-cli/src clean
	make -C user-srv-cli-cmd/src clean
	make -C utils/src clean
	
	#make -C io/src clean
	#make -C ios/src clean
	#make -C pysscm/src clean
	#make -C user-srv/src clean
	#make -C user-srv-i/src clean
