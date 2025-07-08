all:server.exe client.exe
server.exe: server.cc
	g++ $^ -o $@ -std=c++11
client.exe: client.cc
	g++ $^ -o $@ -std=c++11

.PHONY:clean
clean:
	rm -rf *.exe
	rm -rf myfifo
	rm -rf myfifo-0 myfifo-1 myfifo-2 myfifo-3 myfifo-4 myfifo-5 myfifo-6 myfifo-7 myfifo-8 myfifo-9