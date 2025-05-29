.PHONY:all
all:testreplace testcpp
testcpp:extra.cpp 
		@g++ $^ -o $@
testreplace:main.c
		@gcc $^ -o $@

.PHONY:clean

clean:
		@rm -rf testreplace testcpp
