#include "Buffer.h"
#include <cassert>
#include <cstring>

#define BigStr "Hello"
#define SmallStr "Hi!"
#define Big (10*1024*1024)
#define Small 100

void testRefs()
{
	Buffer buf(Big);

	assert(buf.refs() == 1);
	assert(buf.size() == Big);

	strcpy(buf.data(), BigStr);
	assert(!strcmp(BigStr, buf.cdata()));

	Buffer b1(buf);
	assert(buf.refs() == 2);
	assert(b1.refs() == 2);
	assert(buf.cdata() == b1.cdata());

	Buffer other(100);
	Buffer b2(other);
	strcpy(b2.data(), SmallStr);

	assert(other.refs() == 2);
	assert(b2.refs() == 2);

	b2 = b1; // operator=
	assert(b1.refs() == 3);
	assert(b2.refs() == 3);
	assert(!strcmp(BigStr, buf.cdata()));
	assert(!strcmp(BigStr, b1.cdata()));
	assert(!strcmp(BigStr, b2.cdata()));

	assert(other.refs() == 1);
	assert(!strcmp(SmallStr, other.cdata()));
}

void testContent()
{
	Buffer b;
	assert(b.size() == 0);

	b.append("Hello");
	assert(b.size() == 5);
	assert(b.capacity() >= 5);

	b.append(" ", 1);
	assert(b.size() == 6);
	assert(b.capacity() >=6);

	b.append("world!");
	assert(b.size() == 12);
	assert(b.capacity() >=12);
	assert(!strncmp("Hello world!", b.cdata(), 12));

	b.remove(2, 3);
	assert(b.size() == 9);
	assert(!strncmp("He world!", b.cdata(), 9));

	b.chop(3);
	assert(b.size() == 6);
	assert(!strncmp("He wor", b.cdata(), 6));
}

void testCapacity()
{
	Buffer b;
	assert(b.size() == 0);

	b.reserve(1024*1024);
	assert(b.size() == 0);
	assert(b.capacity() >= 1024*1024);

	b.append("Hi");
	assert(b.size() == 2);
	assert(b.capacity() >= 1024*1024);

	b.squeeze();
	assert(b.size() == 2);
	assert(b.capacity() < 1024*1024);
	assert(!strncmp("Hi", b.cdata(), 2));

	b.reserve(1);
	assert(b.size() == 2);
	assert(b.capacity() >= 2);

	b.reserve(0);
	assert(b.size() == 2);
	assert(b.capacity() >= 2);
}

void testRealloc()
{
	Buffer b;

	std::size_t lastCap = b.capacity();
	int reallocs = 0;

	for(int i = 0; i < 1123123; ++i) {
		b.append("Hello");
		if (b.capacity() != lastCap) {
			lastCap = b.capacity();
			++reallocs;
		}
	}
	assert(b.size() == 5*1123123);
	assert(reallocs < 123); // well should certainly be <20
	assert(!strncmp(b.cdata(), "HelloHello", 10));
	assert(!strncmp(b.cdata() + b.size() - 10, "HelloHello", 10));
	// some more random "security checks" ;)
	assert(!strncmp(b.cdata() + 1230, "HelloHello", 10));
	assert(!strncmp(b.cdata() + 7770, "HelloHello", 10));
	assert(!strncmp(b.cdata() + 55000, "HelloHello", 10));
	assert(!strncmp(b.cdata() + 550000, "HelloHello", 10));
	assert(!strncmp(b.cdata() + 5500000, "HelloHello", 10));

	b.resize(10);
	assert(!strncmp(b.cdata(), "HelloHello", 10));
	assert(b.capacity() < 1024*1024);
}

int main(int argc, char *argv[])
{

	testRefs();
	testContent();
	testCapacity();
	testRealloc();

	return 0;
}

/* vim: set sw=4 sts=4 ts=4 noet : */
