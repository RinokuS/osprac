#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    printf("I`m evil AI, and I`ll destroy humankind!\n");
    
    for (int i = 3; i >= 0; --i) {
        printf("%d\n", i);
        sleep(1);
    }

    printf("Kaboom!!1!11!1!");
    return 0;
}
