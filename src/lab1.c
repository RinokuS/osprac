#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	printf("User Identifier: %d\nGroup Identifier: %d\n", 
            getuid(), getgid());
	return 0;
}