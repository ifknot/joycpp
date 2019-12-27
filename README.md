# joycpp
Joy Interpreter written in C++ and Joy

Please see the [Wiki](https://github.com/ifknot/joycpp/wiki)

## 23/12/19
The most important thing to recognise about the Joy Programming Language (joylang) is that most of it can be written in Joy. 

The Joy interpeter [original](https://github.com/ifknot/joycpp_old)
 quick and dirty version got me interested in the Joy Programming Language and after some time spent with the writings of Manfred von Thun and others it is time do this beautiful language justice. Time for a better joycpp.

A joycpp where only the most essential parts are written in C++ and the remainder is written in Joy.

Original version repository renamed [joycpp_old](https://github.com/ifknot/joycpp_old) this is the new version.

### Goals:

* simple fast modular
* minimal number of joylang atomic grammar expressed in C++ 
* remainder joylang expressed in Joy

### Implementation
* joylang is a stack based string re-writing system. Therefore, and to avoid using a garbage collector (GC), use a C++ stack of type std::string but embelished with class member functions that map the joylang stack atomics.

* joylang is made up of both context free and regular grammar. Therefore, modularise the C++ inbuilt atomics on this basis with a joylang top layer:

joy langguage     : joy
 ↓
 
joy context free  : c++
 ↓
 
joy regular       : c++
 ↓
 
joy stack         : c++
 ↓
 
input/output      : c++

### ToDo:
1. ~~input output~~
2. ~~joy_stack_t~~
3. ~~joy stack layer~~
4. ~~bdd test joy_stack~~
5. ~~joy type handling functions and make helpers~~
6. ~~bdd test joy types and tools~~
7. lexer layer (regular grammar)
8. parser layer (context free)
9. joy layer
10. finish joycpp using joy
