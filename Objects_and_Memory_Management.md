- Almost everything you create in C++, from a simple int to a complex class, is treated as an object 
with a defined type, size, and lifetime.

* Do you always need to consider memory management ?
- No, the goal of mordern C++ is to not have to think about manual memory management (new/delete). We need to achieve this by following the single most important principle in C++ :

## Resource Acquisition Is Initialization (RAII)
It means we tie the lifetime of a resource (like heap memory, a file handls, a network socket, etc) to the lifetime of a stack-based object.

- Acquisition: A resource is acquired in an object's constructor.
- Release: The resource is released in the object's destructor.

C++ guarantees that an object is destroyed when it goes out of the scope, the cleanup is automatic and leak proof

