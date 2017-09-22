#include <stdio.h>
#include <stdlib.h>
int main()

{

	int n = 100;
	while(n--)
		system("nc 127.0.0.1 8888  &");
}
