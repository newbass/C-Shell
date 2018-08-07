SHELL-in-C:
===========

Commands implemented:
---------------------

1. pwd: Outputs the name of current directory
2. echo <string>: Outputs the string
3. ls [-la,<dirname>]: ls shows the contents of the file,-a flags shows the hidden files also,-l shows it in long form,ls dirname shows the contents of given directory.
4. cd <dirname>: Go to directory <dirname>
5. pinfo <pid>: pinfo shows information related to <pid> if specified otherwise related to current process
6. nightswatch -n <time-in-secs> <"dirty"/"interrupt"> : Shows dirty memory or interrupts of CPU according to chosen option after every <time-in-secs> till q is pressed
7. unsetenv and setenv has the code for setting and unsetting environment variables.

HOW TO RUN:
===========

> Just type make and run ./SHELL file generated

NOTE:
-----

> q followed by <enter> ends nightswatch
> "quit" in CLI quits the shell
