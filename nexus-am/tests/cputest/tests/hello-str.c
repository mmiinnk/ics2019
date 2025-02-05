#include "trap.h"

char buf[128];

int main() {
	sprintf(buf, "%s", "Hello world!\n");
	printf(buf);
	nemu_assert(strcmp(buf, "Hello world!\n") == 0);
	printf("Hello World Finished!\n");

	sprintf(buf, "%d + %d = %d\n", 1, 1, 2);
	printf(buf);
	nemu_assert(strcmp(buf, "1 + 1 = 2\n") == 0);
	printf("d + d = d Finished\n");

	sprintf(buf, "%d + %d = %d\n", 2, 10, 12);
	printf(buf);
	nemu_assert(strcmp(buf, "2 + 10 = 12\n") == 0);

	printf("Bingo! You made it!\n");

	return 0;
}
