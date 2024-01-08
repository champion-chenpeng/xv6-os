#include<stdio.h>
#include<string.h>

int main() {
	char *str1, *str2 = "abcdefghi";
	strncat(str1, str2, 4);
	printf("%s\n", str1);
	return 0;
}
