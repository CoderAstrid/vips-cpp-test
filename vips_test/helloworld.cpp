#include "stdafx.h"
#include "vips_test.h"


void hello_world()
{
	const char* _in_file = FOLDER_PATH"TEST FILE 1.png";
	const char* _out_file = FOLDER_PATH"TEST FILE 1_OUT.png";
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
	const char* _in_file = FOLDER_PATH"TEST FILE 1.bmp";
	const char* _out_file = FOLDER_PATH"TEST FILE 1_OUT.png";
	
	VImage in = VImage::new_from_file(_in_file,
		VImage::option()->set("access", VIPS_ACCESS_RANDOM));
//	printf("width = %d, height = %d\n", in.width(), in.height());
	
	for (int y = 0; y < in.height(); y += 2048) {
		for (int x = 0; x < in.width(); x += 2048) {
// 			VImage in1 = VImage::new_from_file(_in_file,
// 				VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL_UNBUFFERED));
			int w = ymin(2048, in.width() - 1 - x);
			int h = ymin(2048, in.height() - 1 - y);
			VImage crop = in.extract_area(x, y, w, h);
// 			char szname[256];
// 			sprintf_s(szname, FOLDER_PATH"%05d-%05d.bmp", x, y);
// 			crop.write_to_file(szname);
		}
	}
}

typedef struct tagCropTask {
	int x;
	int y;
	int w;
	int h;
}CropTask;

std::vector<CropTask> g_tasks;
GMutex allocation_lock1;
/* Our set of threads.
*/
GThread *workers_crop[NUM_IN_PARALLEL];
int n_calls_crop = 0;
int TOTAL_CROP = 0;


static int
test_crop(VImage* in, int idx, int x, int y, int w, int h)
{
	char output_file[256];
	sprintf_s(output_file, FOLDER_PATH"%05d-%05d.jpg", x, y);
	VImage res = in->extract_area(x, y, w, h);
	res.write_to_file(output_file);

	return (0);
}


/* What we run as a thread.
*/
static void *
worker_crop(void *data)
{
	const char *filename = (const char *)data;
	VImage in = VImage::new_from_file(filename, VImage::option()->set("access", VIPS_ACCESS_RANDOM));
	for (;;)
	{
		gboolean done;
		int idx = 0;
		done = FALSE;
		CropTask aTask;

		g_mutex_lock(&allocation_lock1);		
		n_calls_crop++;
		if (n_calls_crop > TOTAL_CROP)
			done = TRUE;
		if (!done) {
			idx = n_calls_crop - 1;
			aTask = g_tasks[idx];
		}
		g_mutex_unlock(&allocation_lock1);

		if (done)
			break;

		if (test_crop(&in, idx, aTask.x, aTask.y, aTask.w, aTask.h))
			vips_error_exit(NULL);
	}

	return (NULL);
}

void cropping_thread()
{
	const char* _in_file = FOLDER_PATH"TEST FILE 1.png";
	VImage in = VImage::new_from_file(_in_file,
		VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL_UNBUFFERED));
	for (int y = 0; y < in.height(); y += 1024) {
		for (int x = 0; x < in.width(); x += 1024) {
			CropTask task;
			task.x = x;
			task.y = y;
			task.w = ymin(1024, in.width() - 1 - x);
			task.h = ymin(1024, in.height() - 1 - y);
			g_tasks.push_back(task);
		}
	}
	TOTAL_CROP = g_tasks.size();
	n_calls_crop = 0;

	g_mutex_init(&allocation_lock1);

	for (int i = 0; i < NUM_IN_PARALLEL; i++)
		workers_crop[i] = g_thread_new(NULL, (GThreadFunc)worker_crop, (gpointer)_in_file);

	for (int i = 0; i < NUM_IN_PARALLEL; i++)
		g_thread_join(workers_crop[i]);

}