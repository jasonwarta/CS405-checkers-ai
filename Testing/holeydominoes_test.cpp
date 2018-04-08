// holeydominoes_test.cpp
// Glenn G. Chappell
// 1 Oct 2016
//
// For CS 311 Fall 2016
// Test program for function holeyDominoes
// Used in Assignment 4, Exercise A

// Includes for code to be tested
#include "holeydominoes.h"  // For function holeyDominoes
#include "holeydominoes.h"  // Double inclusion test

#define CATCH_CONFIG_MAIN   // We want Catch to write function main
#include "catch.hpp"        // For the "Catch" unit-testing framework

// Additional includes for this test program
#include <vector>
using std::vector;


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


// *********************************************************************
// Test Cases
// *********************************************************************


TEST_CASE( "Function return types are correct",
           "[types]" )
{
    SECTION( "holeyDominoes returns int by value" )
    {
        int && ii(holeyDominoes(1, 1, 0, 0));
        INFO( "holeyDominoes must return int by value" );
        REQUIRE( TypeCheck<int>::check(holeyDominoes(1, 1, 0, 0)) );
    }
}


TEST_CASE( "1 x n and n x 1 boards",
           "[counting]" )
{
    int h, w;    // Board height & width
    int r, c;    // Forbidden row & column
    int answer;  // Correct return value of holeyDominoes(h,w,r,c)

    SECTION( "Boards: h == 1, w <= 1000" )
    {
        vector<vector<int> > data {     // Test data: h, w, r, c, answer
            {   1,  1,  0,  0,    1 },  // In assignment description
            {   1,  2,  0,  0,    0 },  // In assignment description
            {   1,  2,  0,  1,    0 },
            {   1,  3,  0,  0,    1 },
            {   1,  3,  0,  1,    0 },
            {   1,  3,  0,  2,    1 },
            {   1,  5,  0,  0,    1 },
            {   1,  5,  0,  2,    1 },
            {   1,  5,  0,  3,    0 },
            {   1,  5,  0,  4,    1 },
            {   1,  6,  0,  0,    0 },
            {   1,  6,  0,  3,    0 },
            {   1,  6,  0,  4,    0 },
            {   1, 13,  0,  0,    1 },
            {   1, 13,  0,  3,    0 },
            {   1, 13,  0,  4,    1 },
            {   1, 13,  0,  9,    0 },
            {   1, 13,  0, 12,    1 },
            {   1,100,  0,  0,    0 },
            {   1,100,  0, 49,    0 },
            {   1,100,  0, 50,    0 },
            {   1,101,  0, 49,    0 },
            {   1,101,  0, 50,    1 },
            {   1,101,  0,100,    1 },
            {   1,999,  0,  0,    1 },
            {   1,999,  0,604,    1 },
            {   1,999,  0,901,    0 },
            {   1,999,  0,998,    1 },
        };

        for (auto & test : data)
        {
            h = test[0];
            w = test[1];
            r = test[2];
            c = test[3];
            answer = test[4];
            INFO( "holeyDominoes(" << h << "," << w << ","
                   << r << "," << c <<") should return " << answer );
            REQUIRE( holeyDominoes(h,w,r,c) == answer );
        }
    }

    SECTION( "Boards: w == 1, h <= 1000" )
    {
        vector<vector<int> > data {     // Test data: h, w, r, c, answer
            {   2,  1,  0,  0,    0 },
            {   2,  1,  1,  0,    0 },
            {   3,  1,  0,  0,    1 },
            {   3,  1,  1,  0,    0 },
            {   3,  1,  2,  0,    1 },
            {   5,  1,  0,  0,    1 },
            {   5,  1,  2,  0,    1 },
            {   5,  1,  3,  0,    0 },
            {   5,  1,  4,  0,    1 },
            {   6,  1,  0,  0,    0 },
            {   6,  1,  3,  0,    0 },
            {   6,  1,  4,  0,    0 },
            {  13,  1,  0,  0,    1 },
            {  13,  1,  3,  0,    0 },
            {  13,  1,  4,  0,    1 },
            {  13,  1,  9,  0,    0 },
            {  13,  1, 12,  0,    1 },
            { 100,  1,  0,  0,    0 },
            { 100,  1, 49,  0,    0 },
            { 100,  1, 50,  0,    0 },
            { 101,  1, 49,  0,    0 },
            { 101,  1, 50,  0,    1 },
            { 101,  1,100,  0,    1 },
            { 999,  1,  0,  0,    1 },
            { 999,  1,604,  0,    1 },
            { 999,  1,901,  0,    0 },
            { 999,  1,998,  0,    1 }
        };

        for (auto & test : data)
        {
            h = test[0];
            w = test[1];
            r = test[2];
            c = test[3];
            answer = test[4];
            INFO( "holeyDominoes(" << h << "," << w << ","
                   << r << "," << c <<") should return " << answer );
            REQUIRE( holeyDominoes(h,w,r,c) == answer );
        }
    }
}

TEST_CASE( "Small boards",
           "[counting]" )
{
    int h, w;    // Board height & width
    int r, c;    // Forbidden row & column
    int answer;  // Correct return value of holeyDominoes(h,w,r,c)

    SECTION( "Boards: h,w <= 5, h*w <= 15" )
    {
        vector<vector<int> > data {     // Test data: h, w, r, c, answer
            {   2,  2,  0,  0,    0 },
            {   2,  2,  1,  0,    0 },
            {   2,  3,  0,  0,    0 },
            {   2,  3,  0,  1,    0 },
            {   3,  2,  0,  0,    0 },
            {   3,  2,  1,  0,    0 },
            {   3,  3,  0,  0,    4 },
            {   3,  3,  0,  1,    0 },
            {   3,  3,  0,  2,    4 },  // In assignment description
            {   3,  3,  1,  0,    0 },  // In assignment description
            {   3,  3,  1,  1,    2 },  // In assignment description
            {   3,  3,  1,  2,    0 },
            {   3,  3,  2,  0,    4 },
            {   3,  3,  2,  1,    0 },
            {   3,  3,  2,  2,    4 },
            {   3,  4,  1,  2,    0 },
            {   3,  4,  1,  3,    0 },
            {   4,  3,  2,  1,    0 },
            {   4,  3,  3,  1,    0 },
            {   3,  5,  0,  0,   15 },
            {   3,  5,  0,  1,    0 },
            {   3,  5,  0,  4,   15 },
            {   3,  5,  1,  0,    0 },
            {   3,  5,  1,  1,    8 },
            {   3,  5,  1,  2,    0 },
            {   3,  5,  1,  4,    0 },
            {   3,  5,  2,  2,   15 },
            {   3,  5,  2,  3,    0 },
            {   3,  5,  2,  4,   15 },
            {   5,  3,  0,  0,   15 },
            {   5,  3,  1,  0,    0 },
            {   5,  3,  4,  0,   15 },
            {   5,  3,  0,  1,    0 },
            {   5,  3,  1,  1,    8 },
            {   5,  3,  2,  1,    0 },
            {   5,  3,  4,  1,    0 },
            {   5,  3,  2,  2,   15 },
            {   5,  3,  3,  2,    0 },
            {   5,  3,  4,  2,   15 }
        };

        for (auto & test : data)
        {
            h = test[0];
            w = test[1];
            r = test[2];
            c = test[3];
            answer = test[4];
            INFO( "holeyDominoes(" << h << "," << w << ","
                   << r << "," << c <<") should return " << answer );
            REQUIRE( holeyDominoes(h,w,r,c) == answer );
        }
    }
}

TEST_CASE( "Larger boards",
           "[counting]" )
{
    int h, w;    // Board height & width
    int r, c;    // Forbidden row & column
    int answer;  // Correct return value of holeyDominoes(h,w,r,c)

    SECTION( "Boards: h,w <= 20, h*w <= 40" )
    {
        vector<vector<int> > data {     // Test data: h, w, r, c, answer
            {   2, 20,  1, 15,    0 },
            {  20,  2, 15,  1,    0 },
            {   3, 13,  0,  8, 2911 },
            {   3, 13,  1, 11, 1560 },
            {   3, 13,  1,  4,    0 },
            {   3, 13,  2,  6, 2911 },
            {  13,  3,  8,  0, 2911 },
            {  13,  3, 11,  1, 1560 },
            {  13,  3,  4,  1,    0 },
            {  13,  3,  6,  2, 2911 },
            {   4, 10,  1,  8,    0 },
            {   4, 10,  3,  0,    0 },
            {  10,  4,  8,  1,    0 },
            {  10,  4,  0,  3,    0 },
            {   5,  7,  0,  0, 2415 },
            {   5,  7,  2,  4, 2479 },
            {   5,  7,  3,  5, 1424 },
            {   5,  7,  4,  1,    0 },
            {   7,  5,  0,  0, 2415 },
            {   7,  5,  4,  2, 2479 },
            {   7,  5,  5,  3, 1424 },
            {   7,  5,  1,  4,    0 },
            {   6,  6,  3,  5,    0 }
        };

        for (auto & test : data)
        {
            h = test[0];
            w = test[1];
            r = test[2];
            c = test[3];
            answer = test[4];
            INFO( "holeyDominoes(" << h << "," << w << ","
                   << r << "," << c <<") should return " << answer );
            REQUIRE( holeyDominoes(h,w,r,c) == answer );
        }
    }
}

