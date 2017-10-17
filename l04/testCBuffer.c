#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "cbuffer.h"

Buffer *globalBuffer;

void takeBuffer(Buffer *b)
{
	globalBuffer = b;
	buffer_ref(globalBuffer);
}

void finish()
{
	//this should not segfault, we are "holding" a reference to the buffer
	assert(!strcmp(buffer_data(globalBuffer), "Buffers!"));
	buffer_unref(globalBuffer);
}


int main(int argc, char *argv[])
{
	// 10MB should be large enough to warrant a separate mmap allocation
	// so that free will actualy do an unmap and any access would segfault
	// TODO find a way to measure memory to see it was really allocated / deallocated once.
	//
	Buffer *b1 = buffer_new(100*1024*1024);
	assert(buffer_refs(b1) == 1);

	assert(buffer_size(b1) == 100*1024*1024);

	takeBuffer(b1);
	assert(buffer_refs(b1) == 2);

	strcpy(buffer_data(b1), "Buffers!");

	buffer_unref(b1);
	// We don't "own" the buffer anymore... 
	// If takeBuffer didn't make a new reference, it might have been released
	// already, so normally it would not be safe to use it after this.

	assert(buffer_refs(b1) == 1);

	finish();
	// this should now segfault
	// assert(buffer_refs(b1) == 0);
	return 0;
}

/* vim: set sw=4 sts=4 ts=4 noet : */
