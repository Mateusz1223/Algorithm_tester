# Algorithm_tester

Program which tests alghorithms

## Usage
Windows:
`tester (program to be tested) (tests directory) -h to hide fault message (optional) -a for analyzing mode (optional) -s to stop program after fault (optional)`

Linux:
`./tester (program to be tested) (tests directory) -h to hide fault message (optional) -a for analyzing mode (optional) -s to stop program after fault (optional)`

###### Example:

Windows:
`tester example\add.exe example\tests`

Linux:
`./tester example/add example/tests`

###### Analyzing mode
This mode checks each line of output separately and prints how many lines were correct. Usefull for tasks such as https://szkopul.edu.pl/problemset/problem/v2Y2_UW56ENMcbwP22tkTb7a/site/?key=statement.

## Compilation
`g++ tester.cpp -o tester`

