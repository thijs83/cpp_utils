## Thread Pool

An easy threadpool header to execute void functions.

Note: This threadpool is not threadsafe by default. If you are appending functions that write to the same variable, you will have race conditions! If needed, you should add your own locks on the data. If it is read only on the same data, then it is thread safe.

#### How to use
If you have a void function in a class with arguments arg1, arg2, ...
```
thread_pool_.enqueue(std::bind(&Class::threadingFunction, this, arg1, arg2, ...));
```
if it is not in a class
```
thread_pool_.enqueue(std::bind(&threadingFunction, arg1, arg2, ...));
```