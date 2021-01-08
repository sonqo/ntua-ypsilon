#include <sys/syscall.h>
#include <unistd.h>

int main() {
	syscall(378);
	return 0;
}