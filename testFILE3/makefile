ALL:testfile myfile
myfile:myfile.c myfile.h
		@gcc $^ -o $@ -std=c99
testfile:main.c
		@gcc $^ -o $@
.PHONY:clean
clean:
		@rm -rf testfile
