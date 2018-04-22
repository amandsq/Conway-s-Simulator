# Conway's Game of Life Simulator
#### Authors
```
Amanda Albuquerque
Paulo Medeiros
```

## Description
[Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)  is a zero-player game, this means that it's evolution depends uniquely on the input configuration.
This project is from the **Linguagem de Programação** course by **Selan R. dos Santos**, **UFRN**.
## Compiling
Once you've [downloaded](https://github.com/pauloamed/Conway-s-Simulator/archive/master.zip), you will need the following to compile
```
g++
```
For compiling, use
```
make
```
For cleaning, use
```
make clean
```
## Executing
In order to execute, you will need the initial state file (*input file*). Once it's ready, you must save it inside **/res** folder.

The input file must be built like the following

```
8 8
*
........
........
..*.*...
..***...
..*.*...
........
........
........
```

In the first line, write the height and the width from you matrix
```
8 8
```
In the second line, write the *characther* that will represent the living cells
```
*
```
In the following lines, write your cell matrix
```
........
........
..*.*...
..***...
..*.*...
........
........
........
```
