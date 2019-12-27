# joycpp
Joy Interpreter written in C++ and Joy

Please see the [Wiki](https://github.com/ifknot/joycpp/wiki)

## 27/12/19

Regular expression lexer v0.1 

With a working joy stack and basic lexer it's time to:
* fix a version of the Joy language to implement and access the manual from within Joy (as per Thun)
* translate a minimal set of Joy regular grammar commands into their c++ lambda equivalents

The language specifications as per Manfred von Thun 16:57.51 on Mar 17 2003, henceforth Joy03, are available at [Kevin Albrecht's](http://www.kevinalbrecht.com/)  [mirror of Thun's original pages](http://www.kevinalbrecht.com/code/joy-mirror/joy.html) from his alma mater [La Trobe University](https://www.latrobe.edu.au/) who - for some inexplicable reason - have deleted Thun's original work!

### TODO:
Translate Joy regular grammar commands into their c++ lambda equivalents *but* only Joy grammar that either:

* 1. can not be expressed in Joy grammar
* 2. offers performance benefits as c++ lambda equivalents

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
