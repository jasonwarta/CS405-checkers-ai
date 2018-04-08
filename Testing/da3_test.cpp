// da3_test.cpp
// VERSION 2
// Glenn G. Chappell
// 24 Sep 2016
// Updated: 26 Sep 2016
//
// For CS 311 Fall 2016
// Test program for Assignment 3 Functions & Function Templates
// Used in Assignment 3, Exercises A, B, C, D
// Requires catch.hpp, da3.h, da3.cpp

// Includes for code to be tested
#include "da3.h"        // For Assignment 3 Functions & Templates
#include "da3.h"        // Double inclusion test

#define CATCH_CONFIG_MAIN  // We want Catch to write function main
#include "catch.hpp"       // For the "Catch" unit-testing framework

// Additional includes for this test program
#include <stdexcept>
using std::out_of_range;
using std::runtime_error;
#include <vector>
using std::vector;
#include <algorithm>
using std::equal;
#include <deque>
using std::deque;
#include <utility>
using std::pair;
#include <string>
using std::string;
#include <initializer_list>
using std::initializer_list;


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


// vector call_list
// For storing integers representing function calls.
// Used in testing function call Between.
vector<int> call_list;


// CheckCalls
// Check whether vector call_list holds given values.
bool checkCalls(const initializer_list<int> & vals)
{
    // Note: We could use the 4-iterator version of std::equal, but that
    // requires C++14.

    if (call_list.size() != vals.size())
        return false;

    return equal(vals.begin(), vals.end(),
                 call_list.begin());
}


// Function fnothrow1
// Function that does not throw #1.
void fnothrow1()
{
    call_list.push_back(11);
}


// Function fnothrow2
// Function that does not throw #2.
void fnothrow2()
{
    call_list.push_back(12);
}


// Function fnothrow3
// Function that does not throw #3.
void fnothrow3()
{
    call_list.push_back(13);
}


// Function object fonothrow1
// Function object that does not throw #1.
auto fonothrow1 = []()
{
    call_list.push_back(21);
};


// Function object fonothrow2
// Function object that does not throw #2.
auto fonothrow2 = []()
{
    call_list.push_back(22);
};


// Function object fonothrow3
// Function object that does not throw #3.
auto fonothrow3 = []()
{
    call_list.push_back(23);
};


// Function fthrowrt1
// Function that throws std::runtime_error #1.
void fthrowrt1()
{
    call_list.push_back(31);
    throw runtime_error("");
}


// Function fthrowrt2
// Function that throws std::runtime_error #2.
void fthrowrt2()
{
    call_list.push_back(32);
    throw runtime_error("");
}


// Function fthrowint1
// Function that throws int #1.
void fthrowint1()
{
    call_list.push_back(41);
    throw 0;
}


// Function fthrowint2
// Function that throws int #2.
void fthrowint2()
{
    call_list.push_back(42);
    throw 0;
}


// *********************************************************************
// Test Cases
// *********************************************************************


TEST_CASE( "Function return types are correct",
           "[types]" )
{
    SECTION( "lookUp returns value type by value" )
    {
        LLNode<int> * headi = nullptr;
        headi = new LLNode<int>(1, headi);
        try
        {
            int && ii(lookUp(headi, 0));
            REQUIRE( TypeCheck<int>::check(lookUp(headi, 0)) );
        }
        catch (...)
        {
            INFO( "lookUp should not throw here" );
        }

        LLNode<double> * headd = nullptr;
        headd = new LLNode<double>(1.1, headd);
        try
        {
            double && dd(lookUp(headd, 0));
            REQUIRE( TypeCheck<double>::check(lookUp(headd, 0)) );
        }
        catch (...)
        {
            INFO( "lookUp should not throw here" );
        }
    }

    SECTION( "uniqueCount returns size_t by value" )
    {
        vector<int> v { 1, 2, 3 };
        size_t && st(uniqueCount(v.begin(), v.end()));
        REQUIRE( TypeCheck<size_t>::check(uniqueCount(v.begin(), v.end())) );
    }

    SECTION( "isPrime returns bool by value" )
    {
        bool && b(isPrime(2));
        REQUIRE( TypeCheck<bool>::check(isPrime(2)) );
    }
}


TEST_CASE( "Function template lookUp",
           "[ex-a]" )
{
    LLNode<int> * headi;     // Head ptr for all int Linked Lists
    LLNode<double> * headd;  // Head ptr for all double Linked Lists

    SECTION( "Size 0" )
    {
        // Construct list
        headi = nullptr;

        // Index 0
        try
        {
            int && ii = lookUp(headi, 0);
            REQUIRE( false );  // Empty list, index 0 - throws out_of_range
        }
        catch (out_of_range & e)
        {
            REQUIRE( true );  // Empty list, index 0 - throws out_of_range
            REQUIRE( string("") != e.what() );
                            // Empty list, index 0 - what() nonempty
        }
        catch (...)
        {
            REQUIRE( false );  // Empty list, index 0 - throws out_of_range
        }

        // Index 1
        try
        {
            int && ii = lookUp(headi, 1);
            REQUIRE( false );  // Empty list, index 1 - throws out_of_range
        }
        catch (out_of_range & e)
        {
            REQUIRE( true );  // Empty list, index 1 - throws out_of_range
            REQUIRE( string("") != e.what() );
                            // Empty list, index 1 - what() nonempty
        }
        catch (...)
        {
            REQUIRE( false );  // Empty list, index 1 - throws out_of_range
        }
    }

    SECTION( "Size 1" )
    {
        // Construct list
        headd = nullptr;
        headd = new LLNode<double>(3.2, headd);

        // Index 0
        try
        {
            double && dd = lookUp(headd, 0);
            REQUIRE( dd == 3.2 );  // Size 1, index 0 - returns item
        }
        catch (...)
        {
            REQUIRE( false );  // Size 1, index 0 - returns item
        }

        // Index 1
        try
        {
            double && dd = lookUp(headd, 1);
            REQUIRE( false );  // Size 1, index 1 - throws out_of_range
        }
        catch (out_of_range & e)
        {
            REQUIRE( true );  // Size 1, index 1 - throws out_of_range
            REQUIRE( string("") != e.what() );
                            // Size 1, index 1 - what() nonempty
        }
        catch (...)
        {
            REQUIRE( false );  // Size 1, index 1 - throws out_of_range
        }
    }

    SECTION( "Size 9" )
    {
        // Construct list
        headi = nullptr;
        headi = new LLNode<int>(64, headi);
        headi = new LLNode<int>(49, headi);
        headi = new LLNode<int>(36, headi);
        headi = new LLNode<int>(25, headi);
        headi = new LLNode<int>(16, headi);
        headi = new LLNode<int>(9, headi);
        headi = new LLNode<int>(4, headi);
        headi = new LLNode<int>(1, headi);
        headi = new LLNode<int>(0, headi);

        // Index 0
        try
        {
            int && ii = lookUp(headi, 0);
            REQUIRE( ii == 0 );  // Size 9, index 0 - returns item
        }
        catch (...)
        {
            REQUIRE( false );  // Size 9, index 0 - returns item
        }

        // Index 5
        try
        {
            int && ii = lookUp(headi, 5);
            REQUIRE( ii == 25 );  // Size 9, index 5 - returns item
        }
        catch (...)
        {
            REQUIRE( false );  // Size 9, index 5 - returns item
        }

        // Index 8
        try
        {
            int && ii = lookUp(headi, 8);
            REQUIRE( ii == 64 );  // Size 9, index 8 - returns item
        }
        catch (...)
        {
            REQUIRE( false );  // Size 9, index 8 - returns item
        }

        // Index 9
        try
        {
            int && ii = lookUp(headi, 9);
            REQUIRE( false );  // Size 9, index 9 - throws out_of_range
        }
        catch (out_of_range & e)
        {
            REQUIRE( true );  // Size 9, index 9 - throws out_of_range
            REQUIRE( string("") != e.what() );
                            // Size 9, index 9 - what() nonempty
        }
        catch (...)
        {
            REQUIRE( false );  // Size 9, index 9 - throws out_of_range
        }

        // Index 100
        try
        {
            int && ii = lookUp(headi, 100);
            REQUIRE( false );  // Size 9, index 100 - throws out_of_range
        }
        catch (out_of_range & e)
        {
            REQUIRE( true );  // Size 9, index 100 - throws out_of_range
            REQUIRE( string("") != e.what() );
                            // Size 9, index 9 - what() nonempty
        }
        catch (...)
        {
            REQUIRE( false );  // Size 9, index 100 - throws out_of_range
        }
    }
}


TEST_CASE( "Function callBetween",
           "[ex-b]" )
{
    call_list.clear();

    SECTION( "Three nonthrowing functions" )
    {
        try
        {
            callBetween(fnothrow1, fnothrow2, fnothrow3);
        }
        catch (...)
        {
            REQUIRE( false );  // callBetween does not throw
        }
        REQUIRE( checkCalls({11, 12, 13}) );
    }

    SECTION( "Three nonthrowing function objects" )
    {
        try
        {
            callBetween(fonothrow1, fonothrow2, fonothrow3);
        }
        catch (...)
        {
            REQUIRE( false );  // callBetween does not throw
        }
        REQUIRE( checkCalls({21, 22, 23}) );
    }

    SECTION( "Throwing START: throws std::runtime_error" )
    {
        try
        {
            callBetween(fthrowrt1, fnothrow2, fnothrow3);
            REQUIRE( false );  // callBetween throws
        }
        catch (runtime_error & e)
        {}
        catch (...)
        {
            REQUIRE( false );  // callBetween throws std::runtime_error
        }
        REQUIRE( checkCalls({31}) );
    }

    SECTION( "Throwing START: throws int" )
    {
        try
        {
            callBetween(fthrowint1, fnothrow2, fnothrow3);
            REQUIRE( false );  // callBetween throws
        }
        catch (int & e)
        {}
        catch (...)
        {
            REQUIRE( false );  // callBetween throws int
        }
        REQUIRE( checkCalls({41}) );
    }

    SECTION( "Throwing MIDDLE: throws std::runtime_error" )
    {
        try
        {
            callBetween(fnothrow1, fthrowrt2, fnothrow3);
            REQUIRE( false );  // callBetween throws
        }
        catch (runtime_error & e)
        {}
        catch (...)
        {
            REQUIRE( false );  // callBetween throws std::runtime_error
        }
        REQUIRE( checkCalls({11,32,13}) );
    }

    SECTION( "Throwing MIDDLE: throws int" )
    {
        try
        {
            callBetween(fnothrow1, fthrowint2, fnothrow3);
            REQUIRE( false );  // callBetween throws
        }
        catch (int & e)
        {}
        catch (...)
        {
            REQUIRE( false );  // callBetween throws int
        }
        REQUIRE( checkCalls({11,42,13}) );
    }
}


TEST_CASE( "Function template uniqueCount",
           "[ex-c]" )
{
    deque<int> di {                  // Test data
        1, 1, 2, 1, 2, 2, 3, -1, -1, -1, 5, 3, 3, 3, 2, 2, 1, 1, 1
    };
    const size_t bigsize = 10000;  // Size of very long ranges
                                   //  Must be > 9000

    SECTION( "Empty range" )
    {
        size_t && result = uniqueCount(di.rbegin(), di.rbegin());
        REQUIRE( result == 0 );
    }

    SECTION( "Range of size 1" )
    {
        size_t && result = uniqueCount(di.rbegin()+3, di.rbegin()+4);
        REQUIRE( result == 1 );
    }

    SECTION( "Range of size 2, equal integers" )
    {
        size_t && result = uniqueCount(di.rbegin()+3, di.rbegin()+5);
        REQUIRE( result == 1 );
    }

    SECTION( "Range of size 2, distinct integers" )
    {
        size_t && result = uniqueCount(di.rbegin()+4, di.rbegin()+6);
        REQUIRE( result == 2 );
    }

    SECTION( "Range of size 3, equal integers" )
    {
        size_t && result = uniqueCount(di.rbegin()+9, di.rbegin()+12);
        REQUIRE( result == 1 );
    }

    SECTION( "Range of size 3, two values" )
    {
        size_t && result = uniqueCount(di.rbegin()+8, di.rbegin()+11);
        REQUIRE( result == 2 );
    }

    SECTION( "Range of size 3, all distinct" )
    {
        size_t && result = uniqueCount(di.rbegin()+7, di.rbegin()+10);
        REQUIRE( result == 3 );
    }

    SECTION( "Longer range #1" )
    {
        size_t && result = uniqueCount(di.rbegin()+5, di.rbegin()+13);
        REQUIRE( result == 3 );
    }

    SECTION( "Longer range #2" )
    {
        size_t && result = uniqueCount(di.rbegin()+3, di.rbegin()+15);
        REQUIRE( result == 4 );
    }

    SECTION( "Longer range #3" )
    {
        size_t && result = uniqueCount(di.rbegin(), di.rend());
        REQUIRE( result == 5 );
    }

    SECTION( "Very long range #1" )
    {
        deque<int> di2(bigsize, 3);
        size_t && result = uniqueCount(di2.rbegin(), di2.rend());
        REQUIRE( result == 1 );
    }

    SECTION( "Very long range #2" )
    {
        deque<int> di2(bigsize, 4);
        di2[9000] = 3;
        size_t && result = uniqueCount(di2.rbegin(), di2.rend());
        REQUIRE( result == 2 );
    }

    SECTION( "Very long range #3" )
    {
        deque<int> di2(bigsize, 5);
        for (size_t i = 0; i < bigsize; i += 2)
            di2[i] = 6;
        size_t && result = uniqueCount(di2.rbegin(), di2.rend());
        REQUIRE( result == 2 );
    }

    SECTION( "Very long range #4" )
    {
        deque<size_t> dsi2(bigsize);
        for (size_t i = 0; i < bigsize; ++i)
            dsi2[i] = i;
        size_t && result = uniqueCount(dsi2.rbegin(), dsi2.rend());
        REQUIRE( result == bigsize );
    }

    SECTION( "Strings, equal" )
    {
        vector<string> vs { "abc", "abc" };
        size_t && result = uniqueCount(vs.begin(), vs.end());
        REQUIRE( result == 1 );
    }

    SECTION( "Strings, distinct" )
    {
        vector<string> vs { "abc", "def" };
        size_t && result = uniqueCount(vs.begin(), vs.end());
        REQUIRE( result == 2 );
    }
}


TEST_CASE( "Function isPrime",
           "[ex-d]" )
{
    // Test data
    vector<pair<int, bool> > tdata {
        { -11,   false },
        { 97,    true },
        { -100,  false },
        { -3,    false },
        { -2,    false },
        { -1,    false },
        { 0,     false },
        { 1,     false },
        { 2,     true },
        { 3,     true },
        { 4,     false },
        { 5,     true },
        { 6,     false },
        { 7,     true },
        { 8,     false },
        { 9,     false },
        { 10,    false },
        { 11,    true },
        { 12,    false },
        { 13,    true },
        { 14,    false },
        { 15,    false },
        { 16,    false },
        { 17,    true },
        { 18,    false },
        { 19,    true },
        { 20,    false },
        { 10000, false },
        { 10001, false },
        { 10002, false },
        { 10003, false },
        { 10004, false },
        { 10005, false },
        { 10006, false },
        { 10007, true },
        { 10008, false },
        { 10009, true },
        { 7,     true },
        { 8,     false },
        { -1000, false }
    };

    for (const auto p : tdata)
    {
        bool && result = isPrime(p.first);
        INFO( p.first << " checked; this number "
                      << (p.second ? "IS" : "is NOT") << " prime" );
        REQUIRE( result == p.second );
    }
}

