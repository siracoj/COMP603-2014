#include <stdio.h>
int main(){
char array[30000];
char *ptr=array;
++*ptr;
++*ptr;
++*ptr;
++*ptr;
++*ptr;
++*ptr;
++*ptr;
++*ptr;
while (*ptr) {
	++ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	while (*ptr) {
	++ptr;
	++*ptr;
	++*ptr;
	++ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++ptr;
	++*ptr;
	--ptr;
	--ptr;
	--ptr;
	--ptr;
	--*ptr;
}
	++ptr;
	++*ptr;
	++ptr;
	++*ptr;
	++ptr;
	--*ptr;
	++ptr;
	++ptr;
	++*ptr;
	while (*ptr) {
	--ptr;
}
	--ptr;
	--*ptr;
}
++ptr;
++ptr;
putchar(*ptr);
++ptr;
--*ptr;
--*ptr;
--*ptr;
putchar(*ptr);
++*ptr;
++*ptr;
++*ptr;
++*ptr;
++*ptr;
++*ptr;
++*ptr;
putchar(*ptr);
putchar(*ptr);
++*ptr;
++*ptr;
++*ptr;
putchar(*ptr);
++ptr;
++ptr;
putchar(*ptr);
--ptr;
--*ptr;
putchar(*ptr);
--ptr;
putchar(*ptr);
++*ptr;
++*ptr;
++*ptr;
putchar(*ptr);
--*ptr;
--*ptr;
--*ptr;
--*ptr;
--*ptr;
--*ptr;
putchar(*ptr);
--*ptr;
--*ptr;
--*ptr;
--*ptr;
--*ptr;
--*ptr;
--*ptr;
--*ptr;
putchar(*ptr);
++ptr;
++ptr;
++*ptr;
putchar(*ptr);
++ptr;
++*ptr;
++*ptr;
putchar(*ptr);
return 0;
}
