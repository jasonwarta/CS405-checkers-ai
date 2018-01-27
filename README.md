# CS405 Checkers AI

Edited by:
  Jason Warta
  Rachel Crosley
  Jim Samson
  Cameron Showalter

This project requires [uWebSockets](https://github.com/uNetworking/uWebSockets)

# Running the Project
Before you start: (For Mac Users)
Your Xcode tools needs to be updated. Note, it will take time for it to download.
```
$ xcode-select --install
```

Clone the repository (HTTPS) 
```
$ git clone https://github.com/jasonwarta/CS405-checkers-ai.git
```
Change to the project directory
```
$ cd CS405-checkers-ai.git
```
Build the project using the Makefile
```
$ make 			# builds the project
$ make clean	# clean the build folder
$ make run 		# runs the project
$ make up 		# builds and runs the project
```
Once the project is running, open a web browser and go to `localhost:3000`


# Implementation Details

The Board will be stored as a 12 byte glob of data.  

## Board Encoding  
Binary to tile conversion

1 = 001 => _  
2 = 010 => b  
3 = 011 => B  
4 = 100 => r  
5 = 101 => R  

#### Encoding  
Use binary AND followed by a left shift by 3 bits.  

#### Decoding  
Binary AND the data with 7 (111) to get the right-most tile, then right shift by 3 bits.

## Starting Board
As Text  
```
rrrr
rrrr
rrrr
____
____
bbbb
bbbb
bbbb
```

As a Text String
```
rrrrrrrrrrrr________bbbbbbbbbbbb
```

As a binary string
```
100100100100100100100100100100100100001001001001001001001001010010010010010010010010010010010010
```
