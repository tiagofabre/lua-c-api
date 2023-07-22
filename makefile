.PHONY: install-lua
install-lua :
	curl -R -O http://www.lua.org/ftp/lua-5.4.6.tar.gz && \
	tar zxf lua-5.4.6.tar.gz && \
	cd lua-5.4.6 && \
	make all test
compile :
	gcc -o main main.c -llua

run :
	./main
