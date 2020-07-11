#include "stdafx.h"
#include "vips_test.h"



/* Number of tests we do in total.
*/
#define TOTAL_TESTS (NUM_IN_PARALLEL * 20)

/* Workers queue up on this.
*/
GMutex allocation_lock;

/* Our set of threads.
*/
GThread *workers[NUM_IN_PARALLEL];

/* Number of calls so far.
*/
int n_calls = 0;

/* Our test function. This is called by NUM_IN_PARALLEL threads a total of
* TOTAL_TESTS times.
*/

static int
test(const char *filename)
{
	VipsImage *im, *x;
	char output_file[256];

	snprintf(output_file, 256, "D:\\tmp-%p.jpg", g_thread_self());

	if (!(im = vips_image_new_from_file(filename,
		"access", VIPS_ACCESS_SEQUENTIAL,
		NULL)))
		return (-1);

	if (vips_resize(im, &x, 0.1, NULL))
	{
		g_object_unref(im);
		return (-1);
	}
	g_object_unref(im);
	im = x;
	
	if (vips_image_write_to_file(im, output_file, NULL))
	{
		g_object_unref(im);
		return (-1);
	}
	g_object_unref(im);

	return (0);
}

/* What we run as a thread.
*/
static void *
worker(void *data)
{
	const char *filename = (const char *)data;

	for (;;)
	{
		gboolean done;

		done = FALSE;
		g_mutex_lock(&allocation_lock);
		n_calls += 1;
		if (n_calls > TOTAL_TESTS)
			done = TRUE;
		g_mutex_unlock(&allocation_lock);

		if (done)
			break;

		if (test(filename))
			vips_error_exit(NULL);
	}

	return (NULL);
}

void cpu_thread(const char* _in_file)
{
	int i;
	g_mutex_init(&allocation_lock);

	for (i = 0; i < NUM_IN_PARALLEL; i++)
		workers[i] = g_thread_new(NULL, (GThreadFunc)worker, (gpointer)_in_file);

	for (i = 0; i < NUM_IN_PARALLEL; i++)
		g_thread_join(workers[i]);
}