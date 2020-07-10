#include "stdafx.h"
#include "vips_test.h"

void hello_world()
{
	// 	if (argc != 3)
	// 		vips_error_exit("usage: %s input-file output-file", argv[0]);

	const char* _in_file = "I:\\Datasets\\LargeImages\\TEST FILE 1.png";
	const char* _out_file = "I:\\Datasets\\LargeImages\\TEST FILE 1_OUT.png";
	VImage in = VImage::new_from_file(_in_file,
		VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL));

	double avg = in.avg();

	printf("avg = %g\n", avg);
	printf("width = %d\n", in.width());
		
	VImage out = in.embed(10, 10, 1000, 1000,
		VImage::option()->
		set("extend", "background")->
		set("background", 128));

	out.write_to_file(_out_file);
}

void cropping()
{
#define ymin(a,b) (((a) < (b)) ? (a) : (b))
#define FOLDER_PATH "I:\\Datasets\\LargeImages\\"
	const char* _in_file = FOLDER_PATH"TEST FILE 1.png";
	const char* _out_file = FOLDER_PATH"TEST FILE 1_OUT.png";
	
	VImage in = VImage::new_from_file(_in_file,
		VImage::option()->set("access", VIPS_ACCESS_RANDOM));
//	printf("width = %d, height = %d\n", in.width(), in.height());
	
	for (int y = 0; y < in.height(); y += 1024) {
		for (int x = 0; x < in.width(); x += 1024) {
// 			VImage in1 = VImage::new_from_file(_in_file,
// 				VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL_UNBUFFERED));
			int w = ymin(1024, in.width() - 1 - x);
			int h = ymin(1024, in.height() - 1 - y);
			VImage crop = in.extract_area(x, y, w, h);
			char szname[256];
			sprintf_s(szname, FOLDER_PATH"%05d-%05d.bmp", x, y);
			crop.write_to_file(szname);					
		}
	}
}