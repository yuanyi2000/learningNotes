#include "show_bytes.h"
int main(int argc, char const *argv[])
{
	int val = 0x87654321;
	byte_pointer valp = (byte_pointer) &val;
	show_bytes(valp, 1); // A
	show_bytes(valp, 2); // B
	show_bytes(valp, 3); // C
	return 0;
}