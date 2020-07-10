// vips_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "vips_test.h"

int main()
{
	if (VIPS_INIT(NULL))
		vips_error_exit(NULL);

#if 1
	cropping();
#else
	hello_world();
	cpu_thread("I:\\Datasets\\LargeImages\\TEST FILE 1.png");
#endif
	vips_shutdown();
	
	return (0);
}

