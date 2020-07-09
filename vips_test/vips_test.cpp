// vips_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vips/vips8>
using namespace vips;

int main()
{
	if (VIPS_INIT(NULL))
		vips_error_exit(NULL);

// 	if (argc != 3)
// 		vips_error_exit("usage: %s input-file output-file", argv[0]);

	const char* _in_file = "I:\\Datasets\\LargeImages\\TEST FILE 1.png";
	const char* _out_file = "I:\\Datasets\\LargeImages\\TEST FILE 1_OUT.png";
	VImage in = VImage::new_from_file(_in_file,
		VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL));

	double avg = in.avg();

	printf("avg = %g\n", avg);
	printf("width = %d\n", in.width());

	in = VImage::new_from_file(_in_file,
		VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL));

	VImage out = in.embed(10, 10, 1000, 1000,
		VImage::option()->
		set("extend", "background")->
		set("background", 128));

	out.write_to_file(_out_file);

	vips_shutdown();
	
	return (0);
}

