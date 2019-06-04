## Sleeping Barbers
Multithreaded solution to the classic Sleeping Barbers problem.

make: `g++ -std=c++11 -Werror -Wall Shop1.cpp driver1.cpp -o sleepingBarbers -lpthread`

usage: `./sleepingBarbers numBarbers numChairs numCustomers serviceTime`

example: `./sleepingBarbers 5 20 100 1000`

There are 5 barbers in the shop, with 20 waiting chairs, 100 customers to be served and a service time of 1000.

The status and progress of the barbers, customers and chairs available are printed to stdout.
