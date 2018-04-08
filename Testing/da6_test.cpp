// da6_test.cpp
// Glenn G. Chappell
// 10 Nov 2016
//
// For CS 311 Fall 2016
// Test program for Assignment 3 Function & Class Templates
// Used in Assignment 5, Exercise A, B, C
// Requires catch.hpp, da6.h, llnode.h

// Includes for code to be tested
#include "da6.h"           // For Assignment 6 templates
#include "da6.h"           // Double inclusion test
#include "llnode.h"        // For LLNode

#define CATCH_CONFIG_MAIN  // We want Catch to write function main
#include "catch.hpp"       // For the "Catch" unit-testing framework

// Additional includes for this test program
#include <cstddef>
using std::size_t;
#include <stdexcept>
using std::runtime_error;

#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
#include <sstream>
using std::ostringstream;
#include <vector>
using std::vector;
#include <algorithm>
using std::sort;
#include <utility>
using std::pair;
using std::make_pair;


// *********************************************************************
// Helper Functions/Classes for This Test Program
// *********************************************************************


// class TypeCheck
// This class exists in order to have static member function "check",
// which takes a parameter of a given type, by reference. Objects of
// type TypeCheck<T> cannot be created.
// Usage:
//     TypeCheck<MyType>::check(x)
//      returns true if the type of x is (MyType) or (const MyType),
//      otherwise false.
// Invariants: None.
// Requirements on Types: None.
template<typename T>
class TypeCheck {

private:

    // No default ctor
    TypeCheck() = delete;

    // Uncopyable. Do not define copy/move ctor/assn.
    TypeCheck(const TypeCheck &) = delete;
    TypeCheck(TypeCheck &&) = delete;
    TypeCheck<T> & operator=(const TypeCheck &) = delete;
    TypeCheck<T> & operator=(TypeCheck &&) = delete;

    // Compiler-generated dctor is used (but irrelevant).
    ~TypeCheck() = default;

public:

    // check
    // The function and function template below simulate a single
    // function that takes a single parameter, and returns true iff the
    // parameter has type T or (const T).

    // check (reference-to-const T)
    // Pre: None.
    // Post:
    //     Return is true.
    // Does not throw (No-Throw Guarantee)
    static bool check(const T & param)
    { return true; }

    // check (reference-to-const non-T)
    // Pre: None.
    // Post:
    //     Return is false.
    // Requirements on types: None.
    // Does not throw (No-Throw Guarantee)
    template <typename OtherType>
    static bool check(const OtherType & param)
    { return false; }

};  // End class TypeCheck


// class Counter
// Item type for counting ctor, dctor, op= calls, counting existing
//  objects, and possibly throwing on copy. Has operator< (which always
//  returns false), allowing it to be the value type of a sorted container.
// If static member _copyThrow is set, then copy ctor and copy assn throw
//  std::runtime_error. Exception object's "what" member is set to "C" by
//  the copy ctor and "A" by copy assn.
// Increments static data member _ctorCount on default construction and
//  successful copy construction. Increments static data member _assnCount
//  on successful copy assignment. Increments static data member
//  _dctorCount on destruction.
// Increments static data member _existing on construction, and decrements
//  it on destruction.
// Static data member _maxExisting is highest value of _existing since last
//  reset, or start of program if reset has never been called.
// Invariants:
//     Counter::_existing is number of existing objects of this class.
//     Counter::_ctorCount is number of successful ctor calls since
//      most recent call to reset, or start of program if reset has never
//      been called.
//     Counter::_dctorCount is (similarly) number of dctor calls.
//     Counter::_assnCount is (similarly) number of copy assn calls.
//     Counter::_maxExisting is (similarly) highest value _existing has
//      assumed.
class Counter {

// ***** Counter: Ctors, dctor, op= *****
public:

    // Default ctor
    // Pre: None.
    // Post:
    //     (_ctorCount has been incremented.)
    //     (_existing has been incremented.)
    // Does not throw (No-Throw Guarantee)
    Counter()
    {
        ++_existing;
        if (_existing > _maxExisting)
            _maxExisting = _existing;
        ++_ctorCount;
    }

    // Dctor
    // Pre: None.
    // Post:
    //     (_dctorCount has been incremented.)
    //     (_existing has been decremented.)
    // Does not throw (No-Throw Guarantee)
    ~Counter()
    {
        --_existing;
        ++_dctorCount;
    }

    // Copy ctor
    // Throws std::runtime_error if _copyThrow.
    // Pre: None.
    // Post:
    //     (_ctorCount has been incremented.)
    //     (_existing has been incremented.)
    // May throw std::runtime_error
    // Strong Guarantee
    Counter(const Counter & other)
    {
        if (_copyThrow)
            throw std::runtime_error("C");
        ++_existing;
        if (_existing > _maxExisting)
            _maxExisting = _existing;
        ++_ctorCount;
    }

    // Copy assignment
    // Throws std::runtime_error if _copyThrow.
    // Pre: None.
    // Post:
    //     Return value is *this.
    //     (_assnCount has been incremented.)
    // May throw std::runtime_error
    // Strong Guarantee
    Counter & operator=(const Counter & rhs)
    {
        if (_copyThrow)
            throw std::runtime_error("A");
        ++_assnCount;
        return *this;
    }

    // No move operations
    Counter(Counter && other) = delete;
    Counter & operator=(Counter && other) = delete;

// ***** Counter: Functions dealing with count *****
public:

    // reset
    // Pre: None.
    // Post:
    //     _maxExisting == _existing.
    //     _ctorCount == 0.
    //     _dctorCount == 0.
    //     _assnCount == 0.
    //     _copyThrow == shouldThrow.
    // Does not throw (No-Throw Guarantee)
    static void reset(bool shouldThrow = false)
    {
        _maxExisting = _existing;
        _ctorCount = 0;
        _dctorCount = 0;
        _assnCount = 0;
        _copyThrow = shouldThrow;
    }

    // getExisting
    // Pre: None.
    // Post:
    //     return == _existing.
    // Does not throw (No-Throw Guarantee)
    static size_t getExisting()
    { return _existing; }

    // getMaxExisting
    // Pre: None.
    // Post:
    //     return == _maxExisting.
    // Does not throw (No-Throw Guarantee)
    static size_t getMaxExisting()
    { return _maxExisting; }

    // getCtorCount
    // Pre: None.
    // Post:
    //     return == _ctorCount.
    // Does not throw (No-Throw Guarantee)
    static size_t getCtorCount()
    { return _ctorCount; }

    // getDctorCount
    // Pre: None.
    // Post:
    //     return == _dctorCount.
    // Does not throw (No-Throw Guarantee)
    static size_t getDctorCount()
    { return _dctorCount; }

    // getAssnCount
    // Pre: None.
    // Post:
    //     return == _assnCount.
    // Does not throw (No-Throw Guarantee)
    static size_t getAssnCount()
    { return _assnCount; }

    // setCopyThrow
    // Pre: None.
    // Post:
    //     _copyThrow == shouldThrow
    // Does not throw (No-Throw Guarantee)
    static void setCopyThrow(bool shouldThrow)
    { _copyThrow = shouldThrow; }

// ***** Counter: Data Members *****
private:

    static size_t _existing;     // # of existing objects
    static size_t _maxExisting;  // Max # of existing objects
    static size_t _ctorCount;    // # of successful (non-throwing) ctor calls
    static size_t _dctorCount;   // # of dctor calls
    static size_t _assnCount;    // # of successful (non-throwing) copy = calls
    static bool   _copyThrow;    // true if copy operations (ctor, =) throw

};  // End class Counter

// Definition of static data member of class Counter
size_t Counter::_existing = size_t(0);
size_t Counter::_maxExisting = size_t(0);
size_t Counter::_ctorCount = size_t(0);
size_t Counter::_dctorCount = size_t(0);
size_t Counter::_assnCount = size_t(0);
bool Counter::_copyThrow = false;


// operator< (Counter)
// Dummy-ish operator<, forming a strict weak order for Counter class
// Returns false (which is legal for a strict weak order; all objects of
//  type Counter are equivalent).
// Pre: None.
// Post:
//     Return value == false.
// Does not throw (No-Throw Guarantee)
bool operator<(const Counter & a,
               const Counter & b)
{ return false; }


// *********************************************************************
// Test Cases
// *********************************************************************


TEST_CASE( "reverseList: Size & items",
           "[ex_a][actions]" )
{
    SECTION( "Empty list" )
    {
        shared_ptr<LLNode<int> > head;
        reverseList(head);
        {
        INFO( "Empty list, reversed, should be empty" );
        REQUIRE( !head );
        }
    }

    SECTION( "List of size 1" )
    {
        shared_ptr<LLNode<int> > head;
        push_front(head, 5);
        reverseList(head);
        {
        INFO( "List of size 1, reversed, should have size 1" );
        REQUIRE( size(head) == 1 );
        }

        ostringstream outs;
        print_list(head, outs);
        {
        INFO( "List of size 1, reversed, should have same item" );
        REQUIRE( outs.str() == "5\n" );
        }
    }

    SECTION( "List of size 2" )
    {
        shared_ptr<LLNode<int> > head;
        push_front(head, 14);
        push_front(head, 6);
        reverseList(head);
        {
        INFO( "List of size 2, reversed, should have size 2" );
        REQUIRE( size(head) == 2 );
        }

        ostringstream outs;
        print_list(head, outs);
        {
        INFO( "List of size 2, reversed, should have reversed items" );
        REQUIRE( outs.str() == "14 6\n" );
        }
    }

    SECTION( "List of size 3" )
    {
        shared_ptr<LLNode<int> > head;
        push_front(head, 10);
        push_front(head, 24);
        push_front(head, -3);
        reverseList(head);
        {
        INFO( "List of size 3, reversed, should have size 3" );
        REQUIRE( size(head) == 3 );
        }

        ostringstream outs;
        print_list(head, outs);
        {
        INFO( "List of size 3, reversed, should have reversed items" );
        REQUIRE( outs.str() == "10 24 -3\n" );
        }
    }

    SECTION( "List of size 4" )
    {
        shared_ptr<LLNode<int> > head;
        push_front(head, 5);
        push_front(head, 8);
        push_front(head, 1);
        push_front(head, 6);
        reverseList(head);
        {
        INFO( "List of size 4, reversed, should have size 4" );
        REQUIRE( size(head) == 4 );
        }

        ostringstream outs;
        print_list(head, outs);
        {
        INFO( "List of size 4, reversed, should have reversed items" );
        REQUIRE( outs.str() == "5 8 1 6\n" );
        }
    }

    SECTION( "List of size 20" )
    {
        shared_ptr<LLNode<int> > head;
        for (int i = 0; i < 20; ++i)
        {
            push_front(head, 3*i);
        }
        reverseList(head);
        {
        INFO( "List of size 20, reversed, should have size 20" );
        REQUIRE( size(head) == 20 );
        }

        ostringstream outs;
        print_list(head, outs);
        {
        INFO( "List of size 20, reversed, should have reversed items" );
        string s1 = "0 3 6 9 12 15 18 21 24 27 ";
        string s2 = "30 33 36 39 42 45 48 51 54 57\n";
        REQUIRE( outs.str() == s1+s2 );
        }
    }
}


TEST_CASE( "reverseList: Other requirements",
           "[ex_a][other]" )
{
    SECTION( "No value-type operations" )
    {
        const int BIGSIZE = 10000000;  // Size of large list

        Counter::reset();
        shared_ptr<LLNode<Counter> > head;
        for (int i = 0; i < BIGSIZE; ++i)
            push_front(head, Counter());
        Counter::reset();
        reverseList(head);

        {
        INFO( "reverseList does no value-type construction" );
        REQUIRE( Counter::getCtorCount() == 0 );
        }
        {
        INFO( "reverseList does no value-type destruction" );
        REQUIRE( Counter::getDctorCount() == 0 );
        {
        }
        INFO( "reverseList does no value-type assignment" );
        REQUIRE( Counter::getAssnCount() == 0 );
        }

        // Recursive dctor may have problems on some systems
        for (int i = 0; i < BIGSIZE; ++i)
            pop_front(head);
    }
}


TEST_CASE( "SLQueue: Member function return Types",
           "[ex_b][types]" )
{
    SECTION( "Member function empty - return type")
    {
        {
        const SLQueue<int> q;
        bool && r(q.empty());
        INFO( "const SLQueue<T>::empty returns bool by val" );
        REQUIRE( TypeCheck<bool>::check(q.empty()) );
        }

        {
        SLQueue<int> q;
        bool && r(q.empty());
        INFO( "non-const SLQueue<T>::empty returns bool by val" );
        REQUIRE( TypeCheck<bool>::check(q.empty()) );
        }
    }

    SECTION( "Member function size - return type")
    {
        {
        const SLQueue<int> q;
        size_t && r(q.size());
        INFO( "const SLQueue<T>::size returns size_t by val" );
        REQUIRE( TypeCheck<size_t>::check(q.size()) );
        }

        {
        SLQueue<int> q;
        size_t && r(q.size());
        INFO( "non-const SLQueue<T>::size returns size_t by val" );
        REQUIRE( TypeCheck<size_t>::check(q.size()) );
        }
    }

    SECTION( "Member function front - return type")
    {
        SLQueue<int> qi;
        qi.push(1);
        SLQueue<string> qs;
        qs.push("x");

        {
        const SLQueue<int> & q(qi);
        const int & r(q.front());
        INFO( "const SLQueue<int>::size returns const ref to int" );
        REQUIRE( TypeCheck<int>::check(q.front()) );
        }

        {
        SLQueue<int> & q(qi);
        int & r(q.front());
        INFO( "non-const SLQueue<int>::size returns ref to int" );
        REQUIRE( TypeCheck<int>::check(q.front()) );
        }

        {
        const SLQueue<string> & q(qs);
        const string & r(q.front());
        INFO( "const SLQueue<string>::size returns const ref to string" );
        REQUIRE( TypeCheck<string>::check(q.front()) );
        }

        {
        SLQueue<string> & q(qs);
        string & r(q.front());
        INFO( "non-const SLQueue<string>::size returns ref to string" );
        REQUIRE( TypeCheck<string>::check(q.front()) );
        }
    }
}


TEST_CASE( "SLQueue: push, pop, front",
           "[ex_b][actions]" )
{
    SECTION( "push & pop - checking const & non-const front()" )
    {
        SLQueue<int> q;
        const SLQueue<int> & qc(q);

        REQUIRE( q.empty() );
        REQUIRE( qc.empty() );
        REQUIRE( q.size() == 0 );
        REQUIRE( qc.size() == 0 );

        q.push(5);

        REQUIRE( !q.empty() );
        REQUIRE( !qc.empty() );
        REQUIRE( q.size() == 1 );
        REQUIRE( qc.size() == 1 );
        REQUIRE( q.front() == 5 );
        REQUIRE( qc.front() == 5 );

        q.push(7);

        REQUIRE( !q.empty() );
        REQUIRE( !qc.empty() );
        REQUIRE( q.size() == 2 );
        REQUIRE( qc.size() == 2 );
        REQUIRE( q.front() == 5 );
        REQUIRE( qc.front() == 5 );

        q.pop();

        REQUIRE( !q.empty() );
        REQUIRE( !qc.empty() );
        REQUIRE( q.size() == 1 );
        REQUIRE( qc.size() == 1 );
        REQUIRE( q.front() == 7 );
        REQUIRE( qc.front() == 7 );

        q.pop();

        REQUIRE( q.empty() );
        REQUIRE( qc.empty() );
        REQUIRE( q.size() == 0 );
        REQUIRE( qc.size() == 0 );

        q.push(-2);

        REQUIRE( !q.empty() );
        REQUIRE( !qc.empty() );
        REQUIRE( q.size() == 1 );
        REQUIRE( qc.size() == 1 );
        REQUIRE( q.front() == -2 );
        REQUIRE( qc.front() == -2 );

        q.pop();

        REQUIRE( q.empty() );
        REQUIRE( qc.empty() );
        REQUIRE( q.size() == 0 );
        REQUIRE( qc.size() == 0 );
    }

    SECTION( "push & pop - modifying items using front()" )
    {
        SLQueue<string> q;
        q.push("a");
        q.push("b");
        q.push("c");

        REQUIRE( q.front() == "a" );

        q.front() = "a2";

        REQUIRE( q.front() == "a2" );

        q.pop();

        REQUIRE( q.front() == "b" );

        q.pop();

        REQUIRE( q.front() == "c" );

        q.front() = "c2";

        REQUIRE( q.front() == "c2" );

        q.pop();

        REQUIRE( q.empty() );
    }

    SECTION( "push & pop - mixing various push & pop calls" )
    {
        SLQueue<int> q;
        REQUIRE( q.size() == 0 );

        q.push(4);
        q.push(8);
        q.push(12);
        q.push(16);

        REQUIRE( q.size() == 4 );
        REQUIRE( q.front() == 4 );

        q.pop();

        REQUIRE( q.size() == 3 );
        REQUIRE( q.front() == 8 );

        q.push(20);
        q.push(24);

        REQUIRE( q.size() == 5 );
        REQUIRE( q.front() == 8 );

        q.pop();
        q.pop();

        REQUIRE( q.size() == 3 );
        REQUIRE( q.front() == 16 );

        q.push(28);
        q.push(32);
        q.push(36);
        q.push(40);
        q.push(44);
        q.push(48);
        q.push(52);

        REQUIRE( q.size() == 10 );
        REQUIRE( q.front() == 16 );

        q.pop();
        q.pop();
        q.pop();
        q.pop();
        q.pop();

        REQUIRE( q.size() == 5 );
        REQUIRE( q.front() == 36 );

        q.pop();
        q.pop();
        q.pop();
        q.pop();

        REQUIRE( q.size() == 1 );
        REQUIRE( q.front() == 52 );

        q.pop();

        REQUIRE( q.size() == 0 );

        q.push(56);
        q.push(60);
        q.push(64);

        REQUIRE( q.size() == 3 );
        REQUIRE( q.front() == 56 );

        q.pop();
        q.pop();
        q.pop();

        REQUIRE( q.size() == 0 );
    }

    SECTION( "push & pop: large number of calls" )
    {
        const int NUMOPS = 100000;  // How many ops to do in a loop
        SLQueue<int> q;
        int n = 0;  // Value to push; is incremented with each push
        for (int i = 0; i < 10; ++i)
        {
            for (int i = 0; i < NUMOPS+2; ++i)
                q.push(n++);
            for (int i = 0; i < NUMOPS; ++i)
                q.pop();
        }

        REQUIRE( q.size() == 20 );
        REQUIRE( q.front() == 10*NUMOPS );

        for (int i = 0; i < 19; ++i)
            q.pop();

        REQUIRE( q.size() == 1 );
        REQUIRE( q.front() == 10*NUMOPS+19 );

        q.pop();

        REQUIRE( q.size() == 0 );
    }
}


TEST_CASE( "SLQueue: value-type operations",
           "[ex_b][other]" )
{
    SECTION( "SLQueue: value-type destructors all called" )
    {
        const int SIZE = 1000;  // Initial list size

        {
        Counter::reset();
        SLQueue<Counter> q;
        for (int i = 0; i < SIZE; ++i)
            q.push(Counter());

        {
        INFO( "Number of existing value-type items is size of Queue" );
        REQUIRE( Counter::getExisting() == SIZE );
        }

        q.push(Counter());
        q.pop();

        {
        INFO( "Number of existing value-type items is size of Queue" );
        REQUIRE( Counter::getExisting() == SIZE );
        }

        Counter::reset();
        }

        {
        INFO( "SLQueue destruction destroys all items" );
        REQUIRE( Counter::getExisting() == 0 );
        REQUIRE( Counter::getDctorCount() == SIZE );
        }

        {
        INFO( "SLQueue destruction does no value-type construction" );
        REQUIRE( Counter::getCtorCount() == 0 );
        }

        {
        INFO( "SLQueue destruction does no value-type assignment" );
        REQUIRE( Counter::getAssnCount() == 0 );
        }
    }

    SECTION( "SLQueue::empty does no value-type operations" )
    {
        bool dummyb;  // Holds return value
        Counter::reset();
        SLQueue<Counter> q;

        Counter::reset();
        dummyb = q.empty();
        {
        INFO( "SLQueue::empty does no value-type construction" );
        REQUIRE( Counter::getCtorCount() == 0 );
        }
        {
        INFO( "SLQueue::empty does no value-type destruction" );
        REQUIRE( Counter::getDctorCount() == 0 );
        }
        {
        INFO( "SLQueue::empty does no value-type assignment" );
        REQUIRE( Counter::getAssnCount() == 0 );
        }

        q.push(Counter());
        q.push(Counter());

        Counter::reset();
        dummyb = q.empty();
        {
        INFO( "SLQueue::empty does no value-type construction" );
        REQUIRE( Counter::getCtorCount() == 0 );
        }
        {
        INFO( "SLQueue::empty does no value-type destruction" );
        REQUIRE( Counter::getDctorCount() == 0 );
        }
        {
        INFO( "SLQueue::empty does no value-type assignment" );
        REQUIRE( Counter::getAssnCount() == 0 );
        }
    }

    SECTION( "SLQueue::size does no value-type operations" )
    {
        size_t dummys;  // Holds return value
        Counter::reset();
        SLQueue<Counter> q;

        Counter::reset();
        dummys = q.size();
        {
        INFO( "SLQueue::size does no value-type construction" );
        REQUIRE( Counter::getCtorCount() == 0 );
        {
        }
        INFO( "SLQueue::size does no value-type destruction" );
        REQUIRE( Counter::getDctorCount() == 0 );
        {
        }
        INFO( "SLQueue::size does no value-type assignment" );
        REQUIRE( Counter::getAssnCount() == 0 );
        }

        q.push(Counter());
        q.push(Counter());

        Counter::reset();
        dummys = q.size();
        {
        INFO( "SLQueue::size does no value-type construction" );
        REQUIRE( Counter::getCtorCount() == 0 );
        }
        {
        INFO( "SLQueue::size does no value-type destruction" );
        REQUIRE( Counter::getDctorCount() == 0 );
        }
        {
        INFO( "SLQueue::size does no value-type assignment" );
        REQUIRE( Counter::getAssnCount() == 0 );
        }
    }

    SECTION( "SLQueue::front does no value-type operations" )
    {
        Counter::reset();
        SLQueue<Counter> q;

        q.push(Counter());
        q.push(Counter());

        {
        Counter::reset();
        Counter & r(q.front());
        {
        INFO( "non-const SLQueue::front does no value-type construction" );
        REQUIRE( Counter::getCtorCount() == 0 );
        }
        {
        INFO( "non-const SLQueue::front does no value-type destruction" );
        REQUIRE( Counter::getDctorCount() == 0 );
        }
        {
        INFO( "non-const SLQueue::front does no value-type assignment" );
        REQUIRE( Counter::getAssnCount() == 0 );
        }
        }

        {
        const SLQueue<Counter> & qc(q);
        Counter::reset();
        const Counter & r(qc.front());
        {
        INFO( "const SLQueue::front does no value-type construction" );
        REQUIRE( Counter::getCtorCount() == 0 );
        }
        {
        INFO( "const SLQueue::front does no value-type destruction" );
        REQUIRE( Counter::getDctorCount() == 0 );
        }
        {
        INFO( "const SLQueue::front does no value-type assignment" );
        REQUIRE( Counter::getAssnCount() == 0 );
        }
        }
    }

    SECTION( "SLQueue::pop does no value-type ops except dctor" )
    {
        Counter::reset();
        SLQueue<Counter> q;

        q.push(Counter());
        q.push(Counter());

        Counter::reset();
        q.pop();
        {
        INFO( "SLQueue::pop does no value-type construction" );
        REQUIRE( Counter::getCtorCount() == 0 );
        }
        {
        INFO( "SLQueue::pop does no value-type assignment" );
        REQUIRE( Counter::getAssnCount() == 0 );
        }
    }
}


TEST_CASE( "SLMap: Member function return Types",
           "[ex_c][types]" )
{
    SECTION( "Member function empty - return type")
    {
        {
        const SLMap<int, string> m;
        bool && r(m.empty());
        INFO( "const SLMap<T,U>::empty returns bool by val" );
        REQUIRE( TypeCheck<bool>::check(m.empty()) );
        }

        {
        SLMap<int, string> m;
        bool && r(m.empty());
        INFO( "non-const SLMap<T,U>::empty returns bool by val" );
        REQUIRE( TypeCheck<bool>::check(m.empty()) );
        }
    }

    SECTION( "Member function size - return type")
    {
        {
        const SLMap<int, string> m;
        size_t && r(m.size());
        INFO( "const SLMap<T,U>::size returns size_t by val" );
        REQUIRE( TypeCheck<size_t>::check(m.size()) );
        }

        {
        SLMap<int, string> m;
        size_t && r(m.size());
        INFO( "non-const SLMap<T,U>::size returns size_t by val" );
        REQUIRE( TypeCheck<size_t>::check(m.size()) );
        }
    }

    SECTION( "Member function find - return type")
    {
        {
        const SLMap<int, string> m;
        const string * && r(m.find(1));
        INFO( "const SLMap<int,string>::find returns (const string *) by val" );
        REQUIRE( TypeCheck<const string *>::check(m.find(1)) );
        }

        {
        SLMap<int, string> m;
        string * && r(m.find(1));
        INFO( "non-const SLMap<int,string>::find returns (string *) by val" );
        REQUIRE( TypeCheck<string *>::check(m.find(1)) );
        }

        {
        const SLMap<int, int> m;
        const int * && r(m.find(1));
        INFO( "const SLMap<int,int>::find returns (const int *) by val" );
        REQUIRE( TypeCheck<const int *>::check(m.find(1)) );
        }

        {
        SLMap<int, int> m;
        int * && r(m.find(1));
        INFO( "non-const SLMap<int,int>::find returns (int *) by val" );
        REQUIRE( TypeCheck<int *>::check(m.find(1)) );
        }
    }
}


TEST_CASE( "SLMap: insert, remove, find",
           "[ex_c][actions]" )
{
    SECTION( "Empty map" )
    {
        SLMap<int, string> m;

        {
        INFO( "empty #1 returns true for empty SLMap" );
        REQUIRE( m.empty() );
        }
        {
        INFO( "size #1 returns 0 for empty SLMap" );
        REQUIRE( m.size() == 0 );
        }
        {
        INFO( "find #1 on empty SLMap returns nullptr" );
        REQUIRE( m.find(1) == nullptr );
        }
        {
        INFO( "find #2 on empty SLMap returns nullptr" );
        REQUIRE( m.find(100) == nullptr );
        }
        {
        INFO( "empty #2 returns true for empty SLMap" );
        REQUIRE( m.empty() );
        }
        {
        INFO( "size #2 returns 0 for empty SLMap" );
        REQUIRE( m.size() == 0 );
        }
    }

    SECTION( "Size 1 map" )
    {
        SLMap<int, string> m;
        m.insert(100, "abc");

        {
        INFO( "empty returns false for size 1 SLMap" );
        REQUIRE( !m.empty() );
        }
        {
        INFO( "size returns 1 for size-1 SLMap" );
        REQUIRE( m.size() == 1 );
        }
        {
        INFO( "find #1 on size-1 SLMap returns nullptr" );
        REQUIRE( m.find(1) == nullptr );
        }
        {
        INFO( "find #2 on size-1 SLMap returns non-nullptr" );
        REQUIRE( m.find(100) != nullptr );
        }
        {
        INFO( "find #2 on size-1 SLMap returns ptr to value inserted" );
        REQUIRE( *(m.find(100)) == "abc" );
        }
    }

    SECTION( "Size 2 map" )
    {
        SLMap<int, string> m;
        m.insert(100, "abc");
        m.insert(101, "def");

        {
        INFO( "empty returns false for size 2 SLMap" );
        REQUIRE( !m.empty() );
        }
        {
        INFO( "size returns 2 for size-2 SLMap" );
        REQUIRE( m.size() == 2 );
        }
        {
        INFO( "find #1 on size-2 SLMap returns nullptr" );
        REQUIRE( m.find(1) == nullptr );
        }
        {
        INFO( "find #2 on size-2 SLMap returns non-nullptr" );
        REQUIRE( m.find(100) != nullptr );
        }
        {
        INFO( "find #2 on size-2 SLMap returns ptr to value inserted" );
        REQUIRE( *(m.find(100)) == "abc" );
        }
        {
        INFO( "find #3 on size-2 SLMap returns non-nullptr" );
        REQUIRE( m.find(101) != nullptr );
        }
        {
        INFO( "find #3 on size-2 SLMap returns ptr to value inserted" );
        REQUIRE( *(m.find(101)) == "def" );
        }
        {
        INFO( "find #4 on size-2 SLMap returns nullptr" );
        REQUIRE( m.find(200) == nullptr );
        }
    }

    SECTION( "Modifying value in map" )
    {
        SLMap<int, string> m;
        m.insert(100, "abc");
        m.insert(101, "def");

        {
        INFO( "find #1 on size-2 SLMap returns ptr to value inserted" );
        REQUIRE( *(m.find(100)) == "abc" );
        }
        {
        INFO( "find #2 on size-2 SLMap returns ptr to value inserted" );
        REQUIRE( *(m.find(101)) == "def" );
        }

        *(m.find(100)) = "x";

        {
        INFO( "find #1 on size-2 SLMap returns ptr to new value" );
        REQUIRE( *(m.find(100)) == "x" );
        }
        {
        INFO( "find #2 on size-2 SLMap returns ptr to original value" );
        REQUIRE( *(m.find(101)) == "def" );
        }
    }

    SECTION( "Larger map, with remove calls" )
    {
        const int SIZE = 1000;  // Size for loops
        SLMap<int, int> m;
        for (int i = 0; i < SIZE*2; ++i)
        {
            m.insert(i, 2*i);
        }

        {
        INFO( "Size is correct after inserts" );
        REQUIRE( m.size() == SIZE*2 );
        }

        for (int i = 0; i < SIZE*2; i += 2)
        {
            m.remove(i);
        }

        {
        INFO( "Size is correct after removes" );
        REQUIRE( m.size() == SIZE );
        }

        {
        INFO( "find #1 returns nullptr" );
        REQUIRE( m.find(10) == nullptr );
        }
        {
        INFO( "find #2 returns non-nullptr" );
        REQUIRE( m.find(11) != nullptr );
        }
        {
        INFO( "find #2 returns ptr to inserted value" );
        REQUIRE( *(m.find(11)) == 22 );
        }

        m.remove(11);

        {
        INFO( "find #2 returns nullptr after single remove" );
        REQUIRE( m.find(11) == nullptr );
        }
        {
        INFO( "Size is correct after single remove" );
        REQUIRE( m.size() == SIZE-1 );
        }

        m.insert(11, 72);

        {
        INFO( "find #2 returns non-nullptr after single insert" );
        REQUIRE( m.find(11) != nullptr );
        }
        {
        INFO( "find #2 returns ptr to inserted value after single insert" );
        REQUIRE( *(m.find(11)) == 72 );
        }
    }
}


TEST_CASE( "SLMap: traverse",
           "[ex_c][actions]" )
{
    SECTION( "Traverse of empty map" )
    {
        SLMap<int, int> m;
        vector<pair<int, int> > expected {
        };

        vector<pair<int, int> > data;
        m.traverse(
            [&](int k, int v)
            { data.push_back(make_pair(k, v)); }
        );
        sort(data.begin(), data.end());
        REQUIRE( data == expected );
    }

    SECTION( "Traverse of size-1 map" )
    {
        SLMap<int, int> m;
        m.insert(5, 7);
        vector<pair<int, int> > expected {
            { 5, 7 }
        };

        vector<pair<int, int> > data;
        m.traverse(
            [&](int k, int v)
            { data.push_back(make_pair(k, v)); }
        );
        sort(data.begin(), data.end());
        REQUIRE( data == expected );
    }

    SECTION( "Traverse of size-4 map" )
    {
        SLMap<int, int> m;
        m.insert(5, 7);
        m.insert(8, 3);
        m.insert(1, 8);
        m.insert(7, 4);
        vector<pair<int, int> > expected {
            { 1, 8 },
            { 5, 7 },
            { 7, 4 },
            { 8, 3 }
        };

        vector<pair<int, int> > data;
        m.traverse(
            [&](int k, int v)
            { data.push_back(make_pair(k, v)); }
        );
        sort(data.begin(), data.end());
        REQUIRE( data == expected );
    }

    SECTION( "Traverse of map after inserts & removes" )
    {
        const int SIZE = 1000;  // Size for loops
        SLMap<int, int> m;
        for (int i = 0 ; i < SIZE*4; ++i)
        {
            m.insert(i, 3*i);
        }
        for (int i = 0; i < SIZE*4; i += 2 )
        {
            m.remove(i);
        }
        for (int i = SIZE*10; i < SIZE*12; ++i)
        {
            m.insert(i, 3*i);
        }
        for (int i = 3; i < SIZE*4; i += 4)
        {
            m.remove(i);
        }
        for (int i = SIZE*10; i < SIZE*12; i += 2)
        {
            m.remove(i);
        }

        vector<pair<int, int> > expected;
        for (int i = 1; i < SIZE*4; i += 4)
        {
            expected.push_back(make_pair(i, i*3));
        }
        for (int i = SIZE*10+1; i < SIZE*12; i += 2)
        {
            expected.push_back(make_pair(i, i*3));
        }

        vector<pair<int, int> > data;
        m.traverse(
            [&](int k, int v)
            { data.push_back(make_pair(k, v)); }
        );
        sort(data.begin(), data.end());
        REQUIRE( data == expected );
    }
}


