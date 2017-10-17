Lab 3
=====

Submit the solution to [Buffer](#buffer) tasks
according to the [Submitting instructions](#submitting) before Tuesday Oct 24
23:59:59.



CBuffer
-------

Implement (in plain C) a refernce counted buffer. The `cbuffer.h` header file
gives the full interface for it and you should implement the functions in
`cbuffer.c`.  You can modify the header, if you really need to, as long as the
tests correctly compile and run.

A new reference counted buffer is created with the `buffer_new` function. It
returns a pointer that is used as a "handle" for the buffer when invoking other
functions (what it points to depends on your implementation and whoever
uses your library should not make any assupmtions about it).

Any functions / parts of code that want to keep a reference to the buffer will
call `buffer_ref`  on it and then they can safely use it until they call
`buffer_unref`.  Note that `buffer_new` automatically *refs* the buffer and thus
the original creator should also call *unref* to release the buffer/memory when
it doesn't need it anymore.

`buffer_data` returns a pointer to the actuall buffer/data in memory that should
be used when actually working with it (such as passing to `read`/`write` calls
etc.). `buffer_size` returns the size of the buffer.

Calling these functions on invalid handles (e.g. after the reference count went
down to zero) results in *undefined behaviour* (i.e. you don't have to check and
handle such cases in the implementation, it's ok if your code crashes when
called with a wrong handle).

Buffer
------

Implement the class `Buffer` in C++ similar to the previous task. There are no
new, ref and unfref functions because that functionality can be handled byt the
constructor, copy constructor and destructor. It also has to support a `resize`
operation.

The interface is given in `Buffer.h`. You should implement it in `Buffer.cpp`.
You can also make changes to the header (ideally you should need only to add
some private methods) as long as all the tests correctly compile and run.

Submitting
----------

Submit your solution by committing required files (`Buffer.cpp`)
under the directory `l04` and creating a pull request against the `l04` branch.

A correctly created pull request should appear in the
[list of PRs for `l04`](https://github.com/pulls?utf8=%E2%9C%93&q=is%3Aopen+is%3Apr+user%3AFMFI-UK-2-AIN-118+base%3Al04).
