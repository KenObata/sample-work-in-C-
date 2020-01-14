# Description of files in this directory.


* ```makefile```: Needed to build the program. In order to
construct the ```calprint``` executable, type either ```make``` or
```make calprint```.

* ```calprint.c```: The C file containing the ```main()``` function.
(There must only exist one ```main``` in any program.)  This should be
suitably modified to complete the assignment.

* ```emalloc.[ch]```: Source code and header file for the
```emalloc``` function described in lecture. This is kept in its own
source-code file as it can be used independently in both
```calprint.c``` and ```listy.c```.

* ```ics.h```: The definition of the ```struct event_a``` type.

* ```listy.[ch]```: Linked-list routines based on the lectures. The
routines here may be added to or modified. Regardless, however,
students are responsible for any segmentation faults that occur as a
result of using this code.

* ```motherofalltests.sh```: Modified to run the tests using
```calprint4```.
