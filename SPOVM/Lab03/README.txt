Two programs are created - parent and child.
Before starting the parent program, an environment variable is created in the environment
CHILD_PATH with the name of the directory where the child program is located.
The parent process (parent program) after starting receives variables
environment, sorts them into LC_COLLATE=C and prints them to stdout. After that, it enters the cycle
keyboard clicks.
The "+" character spawns a child process using fork() and execve(), runs
another instance of the program child., using directory information from the environment
which it obtains using the getenv() function. Program name (argv[0]) set to
It is cast as child_XX, where XX is a serial number from 00 to 99. The number is incremented -
is a parent.
The symbol "*" spawns a child process similar to the previous case, one
receives information about its location by scanning an array of environment parameters,
passed in the third parameter of the main() function.
The symbol "&" spawns a child process similar to the previous case, one-
receives information about its location by scanning an array of environment parameters,
specified in the extern char **environ passed in the external variable, set-
host environment at startup.
When a child process starts, it is given a reduced environment, including
a set of variables specified in the file that is passed to the parent
process as a command line option. The minimum set of variables should
include SHELL, HOSTNAME, LOGNAME, HOME, LANG, TERM, USER, LC_COLLATE, PATH.
The child process opens this file, reads the variable names, gets from
environment, and prints their value to stdout.
Child process (program child) prints its name, pid, ppid, opens file
with a set of variables, reads their names, gets from the environment passed to it
at startup, their value in the way specified when processing clicks, outputs to
stdout and exit.
The "&" character terminates the execution of the parent process.
