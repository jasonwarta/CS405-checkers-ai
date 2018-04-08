// tvsarray_test.cpp
// VERSION 2
// Glenn G. Chappell
// 28 Oct 2016
// Updated: 30 Oct 2016
//
// For CS 311 Fall 2016
// Test program for class template TVSArray
// Used in Assignment 5, Exercise A
// Uses the "Catch" unit-testing framework
// Requires catch.hpp, tvsarray.h
//
// History:
// - v2:
//   - Add checking of return value of member functions insert, remove.
//   - Eliminate resize in tests of copy & move operations.

// Includes for code to be tested
#include "tvsarray.h"   // For class template TVSArray
#include "tvsarray.h"   // Double inclusion test

#define CATCH_CONFIG_MAIN  // We want Catch to write function main
#include "catch.hpp"       // For the "Catch" unit-testing framework

// Additional includes for this test program
#include <cstddef>
using std::size_t;
#include <string>
using std::string;
#include <utility>
using std::move;
#include <vector>
using std::vector;
#include <algorithm>
using std::copy;
using std::equal;
#include <stdexcept>
using std::runtime_error;
#include <cassert>
// For assert


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


TEST_CASE( "TVSArray member types",
           "[types]" )
{
    SECTION( "Member type value_type" )
    {
        {
        TVSArray<int>::value_type x1;
        int & r(x1);
        INFO( "TVSArray<int>::value_type must be int" );
        REQUIRE( TypeCheck<int>::check(x1) );
        }

        {
        TVSArray<string>::value_type x2;
        string & r(x2);
        INFO( "TVSArray<string>::value_type must be string" );
        REQUIRE( TypeCheck<string>::check(x2) );
        }
    }

    SECTION( "Member type size_type" )
    {
        {
        TVSArray<int>::size_type x1;
        size_t & r(x1);
        INFO( "TVSArray<int>::size_type must be std::size_t" );
        REQUIRE( TypeCheck<size_t>::check(x1) );
        }

        {
        TVSArray<string>::size_type x2;
        size_t & r(x2);
        INFO( "TVSArray<string>::size_type must be std::size_t" );
        REQUIRE( TypeCheck<size_t>::check(x2) );
        }
    }

    SECTION( "Member type iterator" )
    {
        {
        TVSArray<int>::iterator x1;
        int * & r(x1);
        INFO( "TVSArray<int>::iterator must be (int *)" );
        REQUIRE( TypeCheck<int *>::check(x1) );
        }

        {
        TVSArray<string>::iterator x2;
        string * & r(x2);
        INFO( "TVSArray<string>::iterator must be (string *)" );
        REQUIRE( TypeCheck<string *>::check(x2) );
        }
    }

    SECTION( "Member type const_iterator" )
    {
        {
        TVSArray<int>::const_iterator x1;
        const int * & r(x1);
        INFO( "TVSArray<int>::const_iterator must be (const int *)" );
        REQUIRE( TypeCheck<const int *>::check(x1) );
        }

        {
        TVSArray<string>::const_iterator x2;
        const string * & r(x2);
        INFO( "TVSArray<string>::const_iterator must be (const int *)" );
        REQUIRE( TypeCheck<const string *>::check(x2) );
        }
    }
}


TEST_CASE( "TVSArray function return types",
           "[types]" )
{
    SECTION( "Member function copy= - return type",
             "[types]" )
    {
        {
        const TVSArray<int> cti;
        TVSArray<int> ti;
        TVSArray<int> & r(ti = cti);
        INFO( "TVSArray<int> copy= returns TVSArray<int> by ref" );
        REQUIRE( TypeCheck<TVSArray<int> >::check(ti = cti) );
        }

        {
        const TVSArray<string> cts;
        TVSArray<string> ts;
        TVSArray<string> & r(ts = cts);
        INFO( "TVSArray<string> copy= returns TVSArray<string> by ref" );
        REQUIRE( TypeCheck<TVSArray<string> >::check(ts = cts) );
        }
    }

    SECTION( "Member function move= - return type",
             "[types]" )
    {
        {
        TVSArray<int> ti1;
        TVSArray<int> ti2;
        TVSArray<int> ti3;
        TVSArray<int> & r(ti1 = move(ti2));
        INFO( "TVSArray<int> move= returns TVSArray<int> by ref" );
        REQUIRE( TypeCheck<TVSArray<int> >::check(ti2 = move(ti3)) );
        }

        {
        TVSArray<string> ts1;
        TVSArray<string> ts2;
        TVSArray<string> ts3;
        TVSArray<string> & r(ts1 = move(ts2));
        INFO( "TVSArray<string> move= returns TVSArray<string> by ref" );
        REQUIRE( TypeCheck<TVSArray<string> >::check(ts2 = move(ts3)) );
        }
    }

    SECTION( "Member function size - return type" )
    {
        {
        const TVSArray<int> cti(10);
        size_t && r(cti.size());
        INFO( "TVSArray<int> size returns size_t by value" );
        REQUIRE( TypeCheck<size_t>::check(cti.size()) );
        }

        {
        const TVSArray<string> cts(10);
        size_t && r(cts.size());
        INFO( "TVSArray<string> size returns size_t by value" );
        REQUIRE( TypeCheck<size_t>::check(cts.size()) );
        }
    }

    SECTION( "Member function empty - return type" )
    {
        {
        const TVSArray<int> cti(10);
        bool && r(cti.empty());
        INFO( "TVSArray<int> empty returns bool by value" );
        REQUIRE( TypeCheck<bool>::check(cti.empty()) );
        }

        {
        const TVSArray<string> cts(10);
        bool && r(cts.empty());
        INFO( "TVSArray<string> empty returns bool by value" );
        REQUIRE( TypeCheck<bool>::check(cts.empty()) );
        }
    }

    SECTION( "Member function bracket op - return type",
             "[types]" )
    {
        {
        TVSArray<int> ti(10);
        int & r(ti[0]);
        INFO( "TVSArray<int> bracket op returns int by ref" );
        REQUIRE( TypeCheck<int>::check(ti[0]) );
        }

        {
        TVSArray<string> ts(10);
        string & r(ts[0]);
        INFO( "TVSArray<string> bracket op returns string by ref" );
        REQUIRE( TypeCheck<string>::check(ts[0]) );
        }

        {
        const TVSArray<int> cti(10);
        const int & r(cti[0]);
        INFO( "const TVSArray<int> bracket op returns const int by ref" );
        REQUIRE( TypeCheck<const int>::check(cti[0]) );
        }

        {
        const TVSArray<string> cts(10);
        const string & r(cts[0]);
        INFO( "const TVSArray<string> bracket op returns const string by ref" );
        REQUIRE( TypeCheck<const string>::check(cts[0]) );
        }
    }

    SECTION( "Member function begin - return type",
             "[types]" )
    {
        {
        TVSArray<int> ti(10);
        int * && r(ti.begin());
        INFO( "TVSArray<int> begin returns (int *) by value" );
        REQUIRE( TypeCheck<int *>::check(ti.begin()) );
        }

        {
        TVSArray<string> ts(10);
        string * && r(ts.begin());
        INFO( "TVSArray<string> begin returns (string *) by value" );
        REQUIRE( TypeCheck<string *>::check(ts.begin()) );
        }

        {
        const TVSArray<int> cti(10);
        const int * && r(cti.begin());
        INFO( "const TVSArray<int> begin returns (const int *) by value" );
        REQUIRE( TypeCheck<const int *>::check(cti.begin()) );
        }

        {
        const TVSArray<string> cts(10);
        const string * && r(cts.begin());
        INFO( "const TVSArray<string> begin returns (const string *) by value" );
        REQUIRE( TypeCheck<const string *>::check(cts.begin()) );
        }
    }

    SECTION( "Member function end - return type",
             "[types]" )
    {
        {
        TVSArray<int> ti(10);
        int * && r(ti.end());
        INFO( "TVSArray<int> end returns (int *) by value" );
        REQUIRE( TypeCheck<int *>::check(ti.end()) );
        }

        {
        TVSArray<string> ts(10);
        string * && r(ts.end());
        INFO( "TVSArray<string> end returns (string *) by value" );
        REQUIRE( TypeCheck<string *>::check(ts.end()) );
        }

        {
        const TVSArray<int> cti(10);
        const int * && r(cti.end());
        INFO( "const TVSArray<int> end returns (const int *) by value" );
        REQUIRE( TypeCheck<const int *>::check(cti.end()) );
        }

        {
        const TVSArray<string> cts(10);
        const string * && r(cts.end());
        INFO( "const TVSArray<string> end returns (const string *) by value" );
        REQUIRE( TypeCheck<const string *>::check(cts.end()) );
        }
    }

    SECTION( "Member function insert - return type",
             "[types]" )
    {
        {
        TVSArray<int> ti(10);
        int * && r(ti.insert(ti.begin(), 1));
        INFO( "TVSArray<int> insert returns (int *) by value" );
        REQUIRE( TypeCheck<int *>::check(ti.insert(ti.begin(), 1)) );
        }

        {
        TVSArray<string> ts(10);
        string * && r(ts.insert(ts.begin(), string()));
        INFO( "TVSArray<string> insert returns (string *) by value" );
        REQUIRE( TypeCheck<string *>::check(ts.insert(ts.begin(), string())) );
        }
    }

    SECTION( "Member function remove - return type",
             "[types]" )
    {
        {
        TVSArray<int> ti(10);
        int * && r(ti.remove(ti.begin()));
        INFO( "TVSArray<int> remove returns (int *) by value" );
        REQUIRE( TypeCheck<int *>::check(ti.remove(ti.begin())) );
        }

        {
        TVSArray<string> ts(10);
        string * && r(ts.remove(ts.begin()));
        INFO( "TVSArray<string> remove returns (string *) by value" );
        REQUIRE( TypeCheck<string *>::check(ts.remove(ts.begin())) );
        }
    }
}


TEST_CASE( "TVSArray default ctor, size, empty",
           "[ctors]" )
{
    SECTION( "Default ctor" )
    {
        const TVSArray<int> cti;
        {
        INFO( "Default constructed TVSArray has size 0" );
        REQUIRE( cti.size() == size_t(0) );
        }
        {
        INFO( "Default constructed TVSArray is empty" );
        REQUIRE( cti.empty() );
        }
        {
        INFO( "Default constructed TVSArray: begin == end" );
        REQUIRE( cti.begin() == cti.end() );
        }
    }
}


TEST_CASE( "TVSArray ctor from size, size, empty, begin, end",
           "[ctors]" )
{
    SECTION( "Ctor from size - size 0" )
    {
        const size_t SIZE = size_t(0);
        const TVSArray<int> cti(SIZE);
        {
        INFO( "TVSArray constructed with size " << SIZE << ": size" );
        REQUIRE( cti.size() == SIZE );
        }
        {
        INFO( "TVSArray constructed with size " << SIZE << " is empty" );
        REQUIRE( cti.empty() );
        }
        {
        INFO( "TVSArray constructed with size " << SIZE << ": begin == end" );
        REQUIRE( cti.begin() == cti.end() );
        }
    }

    SECTION( "Ctor from size - small size" )
    {
        const size_t SIZE = size_t(1);
        const TVSArray<int> cti(SIZE);
        {
        INFO( "TVSArray constructed with size " << SIZE << ": size" );
        REQUIRE( cti.size() == SIZE );
        }
        {
        INFO( "TVSArray constructed with size " << SIZE << " is not empty" );
        REQUIRE_FALSE( cti.empty() );
        }
        {
        INFO( "TVSArray constructed with size " << SIZE << ": begin" );
        REQUIRE( cti.begin() == &cti[0] );
        }
        {
        INFO( "TVSArray constructed with size " << SIZE << ": end" );
        REQUIRE( cti.end() == cti.begin() + SIZE );
        }
    }

    SECTION( "Ctor from size - large size" )
    {
        const size_t SIZE = size_t(500);
        const TVSArray<int> cti(SIZE);
        {
        INFO( "TVSArray constructed with size " << SIZE << ": size" );
        REQUIRE( cti.size() == SIZE );
        }
        {
        INFO( "TVSArray constructed with size " << SIZE << " is not empty" );
        REQUIRE_FALSE( cti.empty() );
        }
        {
        INFO( "TVSArray constructed with size " << SIZE << ": begin" );
        REQUIRE( cti.begin() == &cti[0] );
        }
        {
        INFO( "TVSArray constructed with size " << SIZE << ": end" );
        REQUIRE( cti.end() == cti.begin() + SIZE );
        }
    }
}


TEST_CASE( "TVSArray bracket operator",
           "[member ops]" )
{
    SECTION( "Bracket op" )
    {
        const size_t SIZE = size_t(10);
        TVSArray<int> ti(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            ti[i] = 15-int(i)*int(i);
        }

        {
        INFO( "Bracket op (non-const) - check values" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE ( ti[i] == 15-int(i)*int(i) );
        }
        }

        ti[2] = 1000;
        {
        INFO( "Bracket op (non-const) - check values after changing a value" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE( ti[i] == (i == size_t(2) ? 1000 : 15-int(i)*int(i)) );
        }
        }

        // Make const version, no copy
        const TVSArray<int> & ctiref = ti;
        {
        INFO( "Bracket op (const) - check values" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE( ctiref[i] == (i == size_t(2) ? 1000 : 15-int(i)*int(i)) );
        }
        }
    }
}


TEST_CASE( "TVSArray copy ctor",
           "[ctors]" )
{
    SECTION( "Copy ctor - separate arrays" )
    {
        const size_t SIZE = size_t(10);
        TVSArray<int> ti(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            ti[i] = 15-int(i)*int(i);
        }

        // Make const version, no copy
        const TVSArray<int> & ctiref(ti);
        // Make copy (copy ctor)
        TVSArray<int> ticopy(ctiref);

        {
        INFO( "Copy ctor - check size of copy" );
        REQUIRE( ticopy.size() == SIZE );
        }

        {
        INFO( "Copy ctor - check values of copy" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE ( ticopy[i] == 15-int(i)*int(i) );
        }
        }

        // Change original
        ti[2] = 1000;

        {
        INFO( "Copy ctor - change original, check values of copy" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE ( ticopy[i] == 15-int(i)*int(i) );
        }
        }

        {
        INFO( "Copy ctor - change original, check values of original" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE( ti[i] == (i == size_t(2) ? 1000 : 15-int(i)*int(i)) );
        }
        }

        // Change copy
        ticopy[3] = 2000;

        {
        INFO( "Copy ctor - change copy, check values of copy" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE( ticopy[i] == (i == size_t(3) ? 2000 : 15-int(i)*int(i)) );
        }
        }

        {
        INFO( "Copy ctor - change copy, check values of original" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE( ti[i] == (i == size_t(2) ? 1000 : 15-int(i)*int(i)) );
        }
        }
    }

    SECTION( "Copy ctor - destroy original" )
    {
        const size_t SIZE = size_t(10);
        TVSArray<int> * tip = new TVSArray<int>(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            (*tip)[i] = 15-int(i)*int(i);
        }

        // Make copy (copy ctor)
        TVSArray<int> ticopy(*tip);
        // Destroy original
        delete tip;

        {
        INFO( "Copy ctor - check size of copy" );
        REQUIRE( ticopy.size() == SIZE );
        }

        {
        INFO( "Copy ctor - check values of copy" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE ( ticopy[i] == 15-int(i)*int(i) );
        }
        }
    }
}


TEST_CASE( "TVSArray copy assignment operator",
           "[member ops]" )
{
    SECTION( "Copy= - separate arrays" )
    {
        const size_t SIZE = size_t(10);
        TVSArray<int> ti(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            ti[i] = 15-int(i)*int(i);
        }

        // Make const version, no copy
        const TVSArray<int> & ctiref(ti);
        // Make copy (copy assn)
        TVSArray<int> ticopy;
        ticopy = ctiref;

        {
        INFO( "Copy= - check size of copy" );
        REQUIRE( ticopy.size() == SIZE );
        }

        {
        INFO( "Copy= - check values of copy" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE ( ticopy[i] == 15-int(i)*int(i) );
        }
        }

        // Change original
        ti[2] = 1000;

        {
        INFO( "Copy= - change original, check values of copy" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE ( ticopy[i] == 15-int(i)*int(i) );
        }
        }

        {
        INFO( "Copy= - change original, check values of original" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE( ti[i] == (i == size_t(2) ? 1000 : 15-int(i)*int(i)) );
        }
        }

        // Change copy
        ticopy[3] = 2000;

        {
        INFO( "Copy= - change copy, check values of copy" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE( ticopy[i] == (i == size_t(3) ? 2000 : 15-int(i)*int(i)) );
        }
        }

        {
        INFO( "Copy= - change copy, check values of original" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE( ti[i] == (i == size_t(2) ? 1000 : 15-int(i)*int(i)) );
        }
        }
    }

    SECTION( "Copy= - destroy original" )
    {
        const size_t SIZE = size_t(10);
        TVSArray<int> * tip = new TVSArray<int>(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            (*tip)[i] = 15-int(i)*int(i);
        }

        // Make copy (copy assn)
        TVSArray<int> ticopy;
        ticopy = *tip;
        // Destroy original
        delete tip;

        {
        INFO( "Copy ctor - check size of copy" );
        REQUIRE( ticopy.size() == SIZE );
        }

        {
        INFO( "Copy ctor - check values of copy" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE ( ticopy[i] == 15-int(i)*int(i) );
        }
        }
    }
}


TEST_CASE( "TVSArray move ctor",
           "[ctors]" )
{
    SECTION( "Move ctor - destroy original" )
    {
        const size_t SIZE = size_t(10);
        TVSArray<int> * tip = new TVSArray<int>(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            (*tip)[i] = 15-int(i)*int(i);
        }

        // Make copy (move ctor)
        size_t sizeold = tip->size();
        int * dataold = tip->begin();
        TVSArray<int> timove(move(*tip));

        {
        INFO( "Move= - check size of original" );
        REQUIRE( tip->size() == size_t(0) );
        }
        {
        INFO( "Move= - check address of original array" );
        REQUIRE( tip->begin() == nullptr );
        }

        {
        INFO( "Move= - check size of copy" );
        REQUIRE( timove.size() == sizeold );
        }
        {
        INFO( "Move= - check address of copy array" );
        REQUIRE( timove.begin() == dataold );
        }

        // Destroy original
        delete tip;

        {
        INFO( "Move ctor - check size of copy (again)" );
        REQUIRE( timove.size() == SIZE );
        }

        {
        INFO( "Move ctor - check values of copy" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE ( timove[i] == 15-int(i)*int(i) );
        }
        }
    }
}


TEST_CASE( "TVSArray move assignment operator",
           "[member ops]" )
{
    SECTION( "Move= - destroy original" )
    {
        const size_t SIZE = size_t(10);
        TVSArray<int> * tip = new TVSArray<int>(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            (*tip)[i] = 15-int(i)*int(i);
        }

        // Make copy (move assn)
        size_t sizeold = tip->size();
        int * dataold = tip->begin();
        TVSArray<int> timove;
        size_t sizenew = timove.size();
        int * datanew = timove.begin();
        timove = move(*tip);

        {
        INFO( "Move= - check size of original" );
        REQUIRE( tip->size() == sizenew );
        }
        {
        INFO( "Move= - check address of original array" );
        REQUIRE( tip->begin() == datanew );
        }

        {
        INFO( "Move= - check size of copy" );
        REQUIRE( timove.size() == sizeold );
        }
        {
        INFO( "Move= - check address of copy array" );
        REQUIRE( timove.begin() == dataold );
        }

        // Destroy original
        delete tip;

        {
        INFO( "Move= - check size of copy (again)" );
        REQUIRE( timove.size() == SIZE );
        }

        {
        INFO( "Move= - check values of copy" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE ( timove[i] == 15-int(i)*int(i) );
        }
        }
    }
}


TEST_CASE( "TVSArray resize",
           "[member funcs]" )
{
    SECTION( "resize smaller" )
    {
        const size_t SIZE = size_t(10);
        const size_t SIZE2 = size_t(5);
        TVSArray<int> ti(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            ti[i] = 15-int(i)*int(i);
        }
        int * savedata = ti.begin();

        // Resize
        ti.resize(SIZE2);

        {
        INFO( "resize - check size" );
        REQUIRE( ti.size() == SIZE2 );
        }
        {
        INFO( "resize - check address of array" );
        REQUIRE( ti.begin() == savedata );
        }
        {
        INFO( "resize - check values" );
        for (size_t i = 0; i < SIZE2; ++i)
        {
            REQUIRE( ti[i] == 15-int(i)*int(i) );
        }
        }
    }

    SECTION( "resize same size" )
    {
        const size_t SIZE = size_t(10);
        const size_t SIZE2 = size_t(10);
        TVSArray<int> ti(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            ti[i] = 15-int(i)*int(i);
        }
        int * savedata = ti.begin();

        // Resize
        ti.resize(SIZE2);

        {
        INFO( "resize - check size" );
        REQUIRE( ti.size() == SIZE2 );
        }
        {
        INFO( "resize - check address of array" );
        REQUIRE( ti.begin() == savedata );
        }
        {
        INFO( "resize - check values" );
        for (size_t i = 0; i < SIZE2; ++i)
        {
            REQUIRE( ti[i] == 15-int(i)*int(i) );
        }
        }
    }

    SECTION( "resize a little larger" )
    {
        const size_t SIZE = size_t(10);
        const size_t SIZE2 = size_t(15);
        TVSArray<int> ti(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            ti[i] = 15-int(i)*int(i);
        }
        int * savedata = ti.begin();

        // Resize
        ti.resize(SIZE2);
        ti[SIZE2-1] = 1000;  // Do a write, just for fun

        {
        INFO( "resize - check size" );
        REQUIRE( ti.size() == SIZE2 );
        }
        {
        INFO( "resize - check values" );
        for (size_t i = 0; i < SIZE; ++i)  // SIZE, not SIZE2
        {
            REQUIRE( ti[i] == 15-int(i)*int(i) );
        }
        }
    }

    SECTION( "resize a lot larger" )
    {
        const size_t SIZE = size_t(10);
        const size_t SIZE2 = size_t(500000);
        TVSArray<int> ti(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            ti[i] = 15-int(i)*int(i);
        }
        int * savedata = ti.begin();

        // Resize
        ti.resize(SIZE2);
        ti[SIZE2-1] = 1000;  // Do a write, just for fun

        {
        INFO( "resize - check size" );
        REQUIRE( ti.size() == SIZE2 );
        }
        {
        INFO( "resize - check address of array" );
        REQUIRE( ti.begin() != savedata );
        }
        {
        INFO( "resize - check values" );
        for (size_t i = 0; i < SIZE; ++i)  // SIZE, not SIZE2
        {
            REQUIRE( ti[i] == 15-int(i)*int(i) );
        }
        }
    }

    SECTION( "Multiple resize-one-bigger calls" )
    {
        const size_t SIZE = size_t(10);
        const size_t SIZE2 = size_t(10000);
        TVSArray<int> ti(SIZE);
        for (size_t i = 0; i < SIZE; ++i)
        {
            ti[i] = 15-int(i)*int(i);
        }

        // Do resizing
        int realloccount = 0;       // # of times realloc-&-copy done
        bool realloctwice = false;  // Did realloc-&-copy 2x in a row?
        bool realloclast = false;   // Was realloc-&-copy just done?
        for (size_t i = SIZE+1; i <= SIZE2; ++i)
        {
            int * savedata = ti.begin();
            ti.resize(i);
            if (i == size_t(1000))  // Do a write during resizes
                ti[i-size_t(1)] = 1000;
            bool reallocdone = (ti.begin() != savedata);  // realloc-&-copy?
            if (reallocdone)
            {
                ++realloccount;
                if (realloclast)
                    realloctwice = true;
            }
            realloclast = reallocdone;
        }
        {
        INFO( "Many resizes - how many times reallocate-and-copy was done" );
        REQUIRE( realloccount >= 3 );
        REQUIRE( realloccount <= 50 );
        }
        {
        INFO( "Many resizes - reallocate-and-copy never done twice in a row" );
        REQUIRE_FALSE( realloctwice );
        }
        {
        INFO( "Many resizes - check size" );
        REQUIRE( ti.size() == SIZE2 );
        }
        {
        INFO( "Many resizes - check values" );
        for (size_t i = 0; i < SIZE; ++i)  // SIZE, not SIZE2
        {
            REQUIRE( ti[i] == 15-int(i)*int(i) );
        }
        REQUIRE( ti[999] == 1000 );
        }
        {
        INFO( "Many resizes - check begin, end" );
        REQUIRE( ti.begin() == &ti[0] );
        REQUIRE( ti.end() == ti.begin() + SIZE2 );
        }
    }
}


TEST_CASE( "TVSArray insert",
           "[member funcs]" )
{
    const size_t SIZE = size_t(10);
    TVSArray<int> ti_original(SIZE);
    for (size_t i = 0; i < SIZE; ++i)
    {
        ti_original[i] = 15-int(i)*int(i);
    }

    SECTION( "insert at end" )
    {
        TVSArray<int> ti = ti_original;
        ti.insert(ti.end(), 1000);

        {
        INFO( "insert - check size" );
        REQUIRE( ti.size() == SIZE+1 );
        }
        {
        INFO( "insert - check values" );
        for (size_t i = 0; i < SIZE+1; ++i)
        {
            if (i == SIZE)
                REQUIRE( ti[i] == 1000 );
            else
                REQUIRE( ti[i] == 15-int(i)*int(i) );
        }
        }
    }

    SECTION( "insert at beginning" )
    {
        TVSArray<int> ti = ti_original;
        ti.insert(ti.begin(), 1000);

        {
        INFO( "insert - check size" );
        REQUIRE( ti.size() == SIZE+1 );
        }
        {
        INFO( "insert - check values" );
        for (size_t i = 0; i < SIZE+1; ++i)
        {
            if (i == 0)
                REQUIRE( ti[i] == 1000 );
            else
                REQUIRE( ti[i] == 15-(int(i)-1)*(int(i)-1) );
        }
        }
    }

    SECTION( "insert in middle" )
    {
        TVSArray<int> ti = ti_original;
        auto result = ti.insert(ti.begin()+5, 1000);

        {
        INFO( "insert - check size" );
        REQUIRE( ti.size() == SIZE+1 );
        }
        {
        INFO( "insert - check values" );
        for (size_t i = 0; i < SIZE+1; ++i)
        {
            if (i < 5)
                REQUIRE( ti[i] == 15-int(i)*int(i) );
            else if (i == 5)
                REQUIRE( ti[i] == 1000 );
            else
                REQUIRE( ti[i] == 15-(int(i)-1)*(int(i)-1) );
        }
        }
        {
        INFO( "insert - check return value" );
        REQUIRE( result == ti.begin()+5 );
        }
    }

    SECTION( "Multiple insert-at-end calls" )
    {
        const size_t SIZE2 = size_t(1000);
        TVSArray<int> ti = ti_original;

        // Create expected data
        vector<int> v(ti_original.begin(), ti_original.end());
        for (size_t i = SIZE+1; i <= SIZE2; ++i)
        {
            v.push_back(70000-(int(i)-1));
        }
        assert (v.size() == SIZE2);

        // Do inserting
        int realloccount = 0;       // # of times realloc-&-copy done
        bool realloctwice = false;  // Did realloc-&-copy 2x in a row?
        bool realloclast = false;   // Was realloc-&-copy just done?
        for (size_t i = SIZE+1; i <= SIZE2; ++i)
        {
            int * savedata = ti.begin();
            auto result = ti.insert(ti.end(), 70000-int(ti.size()));
            bool reallocdone = (ti.begin() != savedata);  // realloc-&-copy?
            if (reallocdone)
            {
                ++realloccount;
                if (realloclast)
                    realloctwice = true;
                {
                INFO( "Many inserts - check return value on reallocate-and-copy" );
                REQUIRE( result == ti.end()-1 );
                }
            }
            realloclast = reallocdone;
        }
        {
        INFO( "Many inserts - how many times reallocate-and-copy was done" );
        REQUIRE( realloccount >= 3 );
        REQUIRE( realloccount <= 50 );
        }
        {
        INFO( "Many inserts - reallocate-and-copy never done twice in a row" );
        REQUIRE_FALSE( realloctwice );
        }
        {
        INFO( "Many inserts - check size" );
        REQUIRE( ti.size() == SIZE2 );
        }
        {
        INFO( "Many inserts - check values" );
        REQUIRE( equal(v.begin(), v.end(), ti.begin()) );
        }
        {
        INFO( "Many inserts - check begin, end" );
        REQUIRE( ti.begin() == &ti[0] );
        REQUIRE( ti.end() == ti.begin() + SIZE2 );
        }
    }
}


TEST_CASE( "TVSArray remove",
           "[member funcs]" )
{
    const size_t SIZE = size_t(10);
    TVSArray<int> ti_original(SIZE);
    for (size_t i = 0; i < SIZE; ++i)
    {
        ti_original[i] = 15-int(i)*int(i);
    }

    SECTION( "remove at end" )
    {
        TVSArray<int> ti = ti_original;
        int * savedata = ti.begin();
        ti.remove(ti.end()-1);

        {
        INFO( "remove - no reallocate-and-copy" );
        REQUIRE( ti.begin() == savedata );
        }
        {
        INFO( "remove - check size" );
        REQUIRE( ti.size() == SIZE-1 );
        }
        {
        INFO( "remove - check values" );
        for (size_t i = 0; i < SIZE-1; ++i)
        {
            REQUIRE( ti[i] == 15-int(i)*int(i) );
        }
        }
    }

    SECTION( "remove at beginning" )
    {
        TVSArray<int> ti = ti_original;
        int * savedata = ti.begin();
        ti.remove(ti.begin());

        {
        INFO( "remove - no reallocate-and-copy" );
        REQUIRE( ti.begin() == savedata );
        }
        {
        INFO( "remove - check size" );
        REQUIRE( ti.size() == SIZE-1 );
        }
        {
        INFO( "remove - check values" );
        for (size_t i = 0; i < SIZE-1; ++i)
        {
            REQUIRE( ti[i] == 15-(int(i)+1)*(int(i)+1) );
        }
        }
    }

    SECTION( "remove in middle" )
    {
        TVSArray<int> ti = ti_original;
        int * savedata = ti.begin();
        auto result = ti.remove(ti.begin()+5);

        {
        INFO( "remove - no reallocate-and-copy" );
        REQUIRE( ti.begin() == savedata );
        }
        {
        INFO( "remove - check size" );
        REQUIRE( ti.size() == SIZE-1 );
        }
        {
        INFO( "remove - check values" );
        for (size_t i = 0; i < SIZE-1; ++i)
        {
            if (i < 5)
                REQUIRE( ti[i] == 15-int(i)*int(i) );
            else
                REQUIRE( ti[i] == 15-(int(i)+1)*(int(i)+1) );
        }
        }
        {
        INFO( "remove - check return value" );
        REQUIRE( result == ti.begin()+5 );
        }
    }

    SECTION( "Multiple remove-at-end calls" )
    {
        const size_t SIZE2 = size_t(1000);
        TVSArray<int> ti(SIZE2);
        copy(ti_original.begin(), ti_original.end(), ti.begin());

        // Do removing
        int * savedata = ti.begin();
        for (size_t i = SIZE2; i > SIZE; --i)
        {
            ti.remove(ti.end()-1);
        }

        {
        INFO( "Many removes - no reallocate-and-copy" );
        REQUIRE( ti.begin() == savedata );
        }
        {
        INFO( "Many removes - check size" );
        REQUIRE( ti.size() == SIZE );
        }
        {
        INFO( "Many removes - check values" );
        for (size_t i = 0; i < SIZE; ++i)
        {
            REQUIRE( ti[i] == 15-int(i)*int(i) );
        }
        }
        {
        INFO( "Many removes - check begin, end" );
        REQUIRE( ti.begin() == &ti[0] );
        REQUIRE( ti.end() == ti.begin() + SIZE );
        }
    }
}


TEST_CASE( "TVSArray insert & remove" )
{
    SECTION( "Several insert & remove calls" )
    {
        vector<int> v { 1, 3, 4, 2, 6, 8, 7, 4, 5 };
        TVSArray<int> ti(v.size());
        copy(v.begin(), v.end(), ti.begin());

        // Data: 1 3 4 2 6 8 7 4 5
        ti.remove(ti.begin());
        // Data: 3 4 2 6 8 7 4 5
        ti.remove(ti.begin()+7);
        // Data: 3 4 2 6 8 7 4
        ti.insert(ti.begin()+2, 11);
        // Data: 3 4 11 2 6 8 7 4
        ti.remove(ti.begin()+6);
        // Data: 3 4 11 2 6 8 4
        ti.insert(ti.begin(), 12);
        // Data: 12 3 4 11 2 6 8 4
        ti.insert(ti.begin()+8, 13);
        // Data: 12 3 4 11 2 6 8 4 13
        ti.remove(ti.begin()+6);
        // Data: 12 3 4 11 2 6 4 13
        ti.remove(ti.begin()+2);
        // Data: 12 3 11 2 6 4 13
        ti.remove(ti.begin()+5);
        // Data: 12 3 11 2 6 13
        ti.insert(ti.begin()+4, 14);
        // Data: 12 3 11 2 14 6 13
        ti.remove(ti.begin()+2);
        // Data: 12 3 2 14 6 13
        ti.insert(ti.begin(), 15);
        // Data: 15 12 3 2 14 6 13

        vector<int> v2 { 15, 12, 3, 2, 14, 6, 13 };

        {
        INFO( "insert + remove - check size" )
        REQUIRE( ti.size() == v2.size() );
        }
        {
        INFO( "insert + remove - check values" )
        for (size_t i = 0; i < v2.size(); ++i)
        {
            REQUIRE( ti[i] == v2[i] );
        }
        }
    }
}


TEST_CASE( "TVSArray swap",
           "[member funcs]" )
{
    SECTION( "swap" )
    {
        const size_t SIZE1 = size_t(10);
        const size_t SIZE2 = size_t(1000);
        TVSArray<int> ti1(SIZE1);
        for (size_t i = 0; i < SIZE1; ++i)
        {
            ti1[i] = int(i)*int(i);
        }
        TVSArray<int> ti2(SIZE2);
        for (size_t i = 0; i < SIZE2; ++i)
        {
            ti2[i] = 100-int(i);
        }

        // Do swap
        int * savedata1 = ti1.begin();
        int * savedata2 = ti2.begin();
        ti1.swap(ti2);

        {
        INFO( "swap - check size of array #1" );
        REQUIRE( ti1.size() == SIZE2 );
        }
        {
        INFO( "swap - check address of array #1" );
        REQUIRE( ti1.begin() == savedata2 );
        }
        {
        INFO( "swap - check values of array #1" );
        for (size_t i = 0; i < SIZE2; ++i)
        {
            REQUIRE( ti1[i] == 100-int(i) );
        }
        }

        {
        INFO( "swap - check size of array #2" );
        REQUIRE( ti2.size() == SIZE1 );
        }
        {
        INFO( "swap - check address of array #2" );
        REQUIRE( ti2.begin() == savedata1 );
        }
        {
        INFO( "swap - check values of array #2" );
        for (size_t i = 0; i < SIZE1; ++i)
        {
            REQUIRE( ti2[i] == int(i)*int(i) );
        }
        }
    }
}


TEST_CASE( "TVSArray ctor/dctor count",
           "[call counts]" )
{
    SECTION( "Ctor/dctor calls on construction by size, destruction" )
    {
        Counter::reset();
        size_t ctorcount;    // Number of ctor calls
        size_t dctorcount;   // Number of dctor calls
        size_t objectcount;  // Ctor calls - dctor calls
        {
            const size_t SIZE = size_t(1000);
            const TVSArray<Counter> tc(SIZE);
            ctorcount = Counter::getCtorCount();
            dctorcount = Counter::getDctorCount();
            {
            INFO( "Number of ctor calls for container creation is about size" );
            REQUIRE( ctorcount >= SIZE );
            REQUIRE( ctorcount <= 2*SIZE+50 );
            }
            {
            INFO( "Number of dctor calls for container creation is small" );
            REQUIRE( dctorcount <= ctorcount );
            REQUIRE( dctorcount <= 2 );
            }

            objectcount = ctorcount - dctorcount;
            Counter::reset();
        }
        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();

        {
        INFO( "No value-type objects constructed on container destruction" );
        REQUIRE( ctorcount == 0 );
        }
        {
        INFO( "All value-type objects destroyed on container destruction" );
        REQUIRE( dctorcount == objectcount );
        }
    }

    SECTION( "Ctor/dctor calls on default construction, destruction" )
    {
        Counter::reset();
        size_t ctorcount;    // Number of ctor calls
        size_t dctorcount;   // Number of dctor calls
        size_t objectcount;  // Ctor calls - dctor calls
        {
            const size_t SIZE = size_t(0);
            const TVSArray<Counter> tc;
            ctorcount = Counter::getCtorCount();
            dctorcount = Counter::getDctorCount();
            {
            INFO( "Number of ctor calls for container creation is about size" );
            //REQUIRE( ctorcount >= SIZE );  // Null test
            REQUIRE( ctorcount <= 2*SIZE+50 );
            }
            {
            INFO( "Number of dctor calls for container creation is small" );
            REQUIRE( dctorcount <= ctorcount );
            REQUIRE( dctorcount <= 2 );
            }

            objectcount = ctorcount - dctorcount;
            Counter::reset();
        }
        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();

        {
        INFO( "No value-type objects constructed on container destruction" );
        REQUIRE( ctorcount == 0 );
        }
        {
        INFO( "All value-type objects destroyed on container destruction" );
        REQUIRE( dctorcount == objectcount );
        }
    }

    SECTION( "Ctor/dctor calls on copy construction, destruction" )
    {
        Counter::reset();
        size_t ctorcount;    // Number of ctor calls
        size_t dctorcount;   // Number of dctor calls
        size_t objectcount;  // Ctor calls - dctor calls
        {
            const TVSArray<Counter> tc;
            const TVSArray<Counter> tc2;

            ctorcount = Counter::getCtorCount();
            dctorcount = Counter::getDctorCount();
            objectcount = ctorcount - dctorcount;
            Counter::reset();
        }
        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();

        {
        INFO( "No value-type objects constructed on container destruction" );
        REQUIRE( ctorcount == 0 );
        }
        {
        INFO( "All value-type objects destroyed on container destruction" );
        REQUIRE( dctorcount == objectcount );
        }
    }

    SECTION( "Ctor/dctor calls on copy assignment, destruction" )
    {
        Counter::reset();
        size_t ctorcount;    // Number of ctor calls
        size_t dctorcount;   // Number of dctor calls
        size_t objectcount;  // Ctor calls - dctor calls
        {
            const TVSArray<Counter> tc;
            TVSArray<Counter> tc2;
            tc2 = tc;

            ctorcount = Counter::getCtorCount();
            dctorcount = Counter::getDctorCount();
            objectcount = ctorcount - dctorcount;
            Counter::reset();
        }
        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();

        {
        INFO( "No value-type objects constructed on container destruction" );
        REQUIRE( ctorcount == 0 );
        }
        {
        INFO( "All value-type objects destroyed on container destruction" );
        REQUIRE( dctorcount == objectcount );
        }
    }

    SECTION( "Ctor/dctor calls on move construction, destruction" )
    {
        Counter::reset();
        size_t ctorcount;    // Number of ctor calls
        size_t dctorcount;   // Number of dctor calls
        size_t objectcount;  // Ctor calls - dctor calls
        {
            TVSArray<Counter> tc;
            const TVSArray<Counter> tc2(move(tc));

            ctorcount = Counter::getCtorCount();
            dctorcount = Counter::getDctorCount();
            objectcount = ctorcount - dctorcount;
            Counter::reset();
        }
        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();

        {
        INFO( "No value-type objects constructed on container destruction" );
        REQUIRE( ctorcount == 0 );
        }
        {
        INFO( "All value-type objects destroyed on container destruction" );
        REQUIRE( dctorcount == objectcount );
        }
    }

    SECTION( "Ctor/dctor calls on move assignment, destruction" )
    {
        Counter::reset();
        size_t ctorcount;    // Number of ctor calls
        size_t dctorcount;   // Number of dctor calls
        size_t objectcount;  // Ctor calls - dctor calls
        {
            TVSArray<Counter> tc;
            TVSArray<Counter> tc2;
            tc2 = move(tc);

            ctorcount = Counter::getCtorCount();
            dctorcount = Counter::getDctorCount();
            objectcount = ctorcount - dctorcount;
            Counter::reset();
        }
        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();

        {
        INFO( "No value-type objects constructed on container destruction" );
        REQUIRE( ctorcount == 0 );
        }
        {
        INFO( "All value-type objects destroyed on container destruction" );
        REQUIRE( dctorcount == objectcount );
        }
    }

    SECTION( "Ctor/dctor calls on swap, destruction" )
    {
        Counter::reset();
        size_t ctorcount;    // Number of ctor calls
        size_t dctorcount;   // Number of dctor calls
        size_t objectcount;  // Ctor calls - dctor calls
        {
            const size_t SIZE1 = size_t(100);
            const size_t SIZE2 = size_t(1000);
            TVSArray<Counter> tc1;
            TVSArray<Counter> tc2;

            ctorcount = Counter::getCtorCount();
            dctorcount = Counter::getDctorCount();
            objectcount = ctorcount - dctorcount;
            Counter::reset();

            tc1.swap(tc2);

            ctorcount = Counter::getDctorCount();
            dctorcount = Counter::getDctorCount();
            {
            INFO( "No value-type objects constructed on container swap" );
            REQUIRE( ctorcount == 0 );
            }
            {
            INFO( "No value-type objects destroyed on container swap" );
            REQUIRE( dctorcount == 0 );
            }

            Counter::reset();
        }
        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();

        {
        INFO( "No value-type objects constructed on container destruction" );
        REQUIRE( ctorcount == 0 );
        }
        {
        INFO( "All value-type objects destroyed on container destruction" );
        REQUIRE( dctorcount == objectcount );
        }
    }
}


TEST_CASE( "TVSArray exceptions",
           "[exceptions]" )
{
    bool throws;
    bool throws_proper_type;
    size_t ctorcount;
    size_t dctorcount;

    SECTION( "Exceptions - copy ctor" )
    {
        Counter::reset(true);
        {
            const TVSArray<Counter> tc(10);

            try
            {
                TVSArray<Counter> tc2(tc);
                throws_proper_type = false;
            }
            catch (std::runtime_error & e)
            {
                throws_proper_type = true;
            }
            catch (...)
            {
                throws_proper_type = false;
            }
        }

        {
        INFO( "Copy ctor is exception-neutral" );
        REQUIRE( throws_proper_type );
        }

        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();

        {
        INFO( "Copy ctor has no memory leak" );
        REQUIRE( ctorcount == dctorcount );
        }
    }

    SECTION( "Exceptions - copy=" )
    {
        Counter::reset(true);
        {
            const TVSArray<Counter> tc(10);

            try
            {
                TVSArray<Counter> tc2;
                tc2 = tc;
                throws_proper_type = false;
            }
            catch (std::runtime_error & e)
            {
                throws_proper_type = true;
            }
            catch (...)
            {
                throws_proper_type = false;
            }
        }

        {
        INFO( "Copy assn is exception-neutral" );
        REQUIRE( throws_proper_type );
        }

        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();

        {
        INFO( "Copy assn has no memory leak" );
        REQUIRE( ctorcount == dctorcount );
        }
    }

    SECTION( "Exceptions - move ctor" )
    {
        Counter::reset(true);
        {
            TVSArray<Counter> tc(10);

            try
            {
                TVSArray<Counter> tc2(move(tc));
                throws = false;
            }
            catch (...)
            {
                throws = true;
            }
        }

        {
        INFO( "Move ctor does not throw" );
        REQUIRE_FALSE( throws );
        }

        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();
        {
        INFO( "Move ctor has no memory leak" );
        REQUIRE( ctorcount == dctorcount );
        }
    }

    SECTION( "Exceptions - move=" )
    {
        Counter::reset(true);
        {
            TVSArray<Counter> tc(10);

            try
            {
                TVSArray<Counter> tc2;
                tc2 = move(tc);
                throws = false;
            }
            catch (...)
            {
                throws = true;
            }
        }

        {
        INFO( "Move assn does not throw" );
        REQUIRE_FALSE( throws );
        }

        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();
        {
        INFO( "Move assn has no memory leak" );
        REQUIRE( ctorcount == dctorcount );
        }
    }

    SECTION( "Exceptions - resize" )
    {
        Counter::reset(true);
        {
            TVSArray<Counter> tc(10);

            // Resize smaller
            {
                try
                {
                    tc.resize(5);
                    throws = false;
                }
                catch (...)
                {
                    throws = true;
                }
            }

            {
            INFO( "resize smaller does not throw" );
            REQUIRE_FALSE( throws );
            }

            // Resize much larger
            {
                try
                {
                    tc.resize(500000000);
                    throws_proper_type = false;
                }
                catch (std::runtime_error & e)
                {
                    throws_proper_type = true;
                }
                catch (...)
                {
                    throws_proper_type = false;
                }
            }
        }

        {
        INFO( "resize is exception-neutral" );
        REQUIRE( throws_proper_type );
        }

        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();
        {
        INFO( "resize has no memory leak" );
        REQUIRE( ctorcount == dctorcount );
        }
    }

    SECTION( "Exceptions - swap" )
    {
        Counter::reset(true);
        {
            TVSArray<Counter> tc1(10);
            TVSArray<Counter> tc2(1000);

            try
            {
                tc1.swap(tc2);
                throws = false;
            }
            catch (...)
            {
                throws = true;
            }
        }

        {
        INFO( "swap does not throw" );
        REQUIRE_FALSE( throws );
        }

        ctorcount = Counter::getCtorCount();
        dctorcount = Counter::getDctorCount();
        {
        INFO( "swap has no memory leak" );
        REQUIRE( ctorcount == dctorcount );
        }
    }
}

