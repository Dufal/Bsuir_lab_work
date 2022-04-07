Parent process actions
On pressing the "+" key, the parent process (P) spawns a child process
(C_k) and reports it.
By pressing the "-" key, P deletes the last generated C_k, reports
this and the number of those left.
When the character "k" is entered, P removes all C_k and reports it.
When the character "s" is entered, P prevents all C_k from printing statistics (see below).
When the character "g" is entered, P allows all C_k to display statistics.
When entering the characters "s<num>", P prevents C_<num> from printing statistics.
When entering the characters "g<num>", P allows C_<num> to print statistics.
When entering the characters "p<num>", P forbids all C_k output and asks
C_<num> print your statistics. After the specified time has elapsed (5 s, for example),
if 'g' is not entered, allows all C_k's to output statistics again.
On pressing the "q" key, P deletes all C_k, reports it and exits.
Child Process Actions
The child process in the outer loop sets the alarm (nanosleep(2)) and enters
an eternal loop in which, in alternation mode, fills the structure containing the pair
variables of type int, values {0, 0} and {1, 1} (see the section "Problems of non-atomic
access” of the “Signals” topic).
When receiving a signal from the alarm clock, it checks the contents of the structure, collects
statistics and repeats the body of the outer loop.
After a given number of repetitions of the outer loop (for example, after 101)
child process, if allowed, prints its PPID, PID and 4 numbers - count
different pairs registered at the time of receiving a signal from the alarm clock.
The output is carried out character by character.
C_k requests access to stdout from P and outputs after confirmation.
When output is complete, C_k informs P about it.
