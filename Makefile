MAKESHELL = /bin/sh
SHELL     = /bin/sh

arch:
	if [ ! -e Src ] ; then \
		ln -s src Src ; \
	fi
	if [ ! -d include ] ; then   \
		mkdir include ; \
		cd include ; \
		ln -s ../src/include/dolinks . ; \
		dolinks ; \
	fi;
