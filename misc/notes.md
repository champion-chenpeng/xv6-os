# xv6 labs notes
## lab 0 unities
### find
- use ls structure to traverse one level
- add recursion to DIR type

### primes
1. fork()
- mixed up child and parent, which is valid in pingpong, since pipe is symmetry, and two process cannot be distinguished
- it is not valid in primes since there are lots of processes, when reverse child and parent, the structure is main to all siblings instead of grand...grandchild

2. recursion
- must return in recursion, this is a compiler check
- compiler cannot validate if exit(0) exists, only check return, which is convenient

3. pipe
- when fork, pipe is duplicated. Remember pipe is a reference to a file descriptor object. Each process(parent and child) 's fd is independent, we can thought that fd(int) corresponding to underneath pointers to file descriptor object, two pointers can point to same object but are different themselves. e.i. parent\_fd0 = child\_fd0 = &fd_\object. when in child, close(0) means child\_fd0 = nullptr, parent\_fd0 is still valid.
- the file offset must lay in fd object, thus parent and child share same offset. (TODO can extend to multiprocess > 2? A: not even a question, only fork duplication cause diff point to same fd\_object, and I believe the offset is in the fd\_object);
- so now the structure is fd\_pointer -> fd\_int -> file object, multiple fd\_int may refer to same object, and fork duplication cause multiple pointer to same int in diff process. ex. 2>&1 <=> p2 = p1; fork() <=> parent1 = child1, close1() <=> parent1 = nullptr.
