/**
 * Shop1.cpp
 * Class definitions for multiple sleeping barbers Shop monitor.
 * 
 * Emulates a barber shop, where customers enter a barber shop and either 
 * receive a haircut or sit in a waiting char if there are other customers
 * waiting. If the waiting chars are full, the customer leaves without
 * a haircut.
 * Program version 1 (max does not default to 3 if nChairs is 0)
 * 
 * Thurman Gillespy
 * 5/8/18
 */

#include <iostream> // cout
#include <sstream>  // stringstream
#include <string>   // string
#include "Shop1.h"

/**
 * destructor - delete all memory we allocated
 */
Shop::~Shop()
{
   delete [] in_service;
   delete [] money_paid;
   delete [] cond_customer_served;
   delete [] cond_barber_paid;
   delete [] cond_barber_waiting;
}

/**
 * init
 * Allocate memory for arrays.
 * Initialize all cond_t condition variables
 */
void Shop::init( int nCustomers, int nBarbers )
{
   // customers begin at id 1, barbers at id 0
   in_service = new bool[nCustomers + 1];
   money_paid = new bool[nBarbers];

   // initialize the map so all barbID keys are mapped to -1 (no customer)
   for (int i = 0; i < nBarbers; i++) // assumes barbID starts at 1
   {
      barberMap.insert(make_pair(i, -1));
   }

   pthread_mutex_init( &mutex, NULL );
   pthread_cond_init(&cond_customers_waiting, NULL);

   // allocate memory for cond arrays, then initialize
   // customers begin at id 1
   cond_customer_served = new pthread_cond_t[nCustomers + 1];
   for (int i = 0; i < nCustomers + 1; i++ )
   {
      pthread_cond_init( &cond_customer_served[i], NULL );
   }

   cond_barber_paid = new pthread_cond_t[nBarbers];
   cond_barber_waiting = new pthread_cond_t[nBarbers];
   
   // barbers begin at id 0
   for (int i = 0; i < nBarbers; i++)
   {
      pthread_cond_init( &cond_barber_paid[i], NULL );
      pthread_cond_init( &cond_barber_waiting[i], NULL);
   }
}

/**
 * customer visits the shop to get a haircut
 * return -1 if no service, else return the barber ID
 */
int Shop::visitShop( int custID )
{
   pthread_mutex_lock( &mutex );   // lock

   if (max > 0) // version 1, max always > 0 
   {
      if (waiting_chairs.size() == max)
      {
         if (barber_queue.empty())
         {
            print(custID, "leaves the shop because of no available waiting chairs.", false);
            ++nNotServed;
            pthread_mutex_unlock(&mutex);
            return -1; // leave the shop
         }
         else
         {
            // although the waiting chairs are full, there is a barber available
            // get the next customer out of his chair
            pthread_cond_signal(&cond_customers_waiting);
         }
      }

      if (service_chair != 0 || !waiting_chairs.empty())
      {
         // someone is being served or transitting from a waiting to a service chair
         waiting_chairs.push(custID); // have a waiting chair

         print(custID, "takes a waiting chair. # waiting seats available = " + int2string(max - waiting_chairs.size()), false);
         pthread_cond_wait(&cond_customers_waiting, &mutex);
         waiting_chairs.pop(); // stand up
      }
   } // end if max > 0
   else // max == 0, ie, no waiting chairs at all - version 2
   {
      // leave if no barber ready and the service chair is occupied
      if (barber_queue.empty() && service_chair != 0)
      {
         print(custID, "leaves the shop because of no waiting chairs or available barbers.", false);
         ++nNotServed;
         pthread_mutex_unlock(&mutex);
         return -1;
      }
   } // end else
   
   // get the next barber
   int barbID = -1;

   if (!barber_queue.empty())
   {
      BarberParam b = barber_queue.front();
      barbID = b.id;
      assert(barbID != -1);
      barber_queue.pop();
   }
   else
   {
      // rarely, due to timing issues, a barber is not avaiable
      // leave the shop without a haircut
      #ifndef NDEBUG
      cout << "*** NO BARBER IN QUEUE ***" << endl;
      #endif
      print(custID, "leaves the shop because of no available waiting chairs.", false);
      ++nNotServed;
      pthread_mutex_unlock(&mutex);
      return -1; // no barber ready
   }

   // associate barber and customer ids
   barberMap[barbID] = custID; // <barbID, custID>
   print( custID, "moves to the service chair. # waiting seats available = " 
	   + int2string( max - waiting_chairs.size( ) ), false );
   
   in_service[custID] = true;
   service_chair = custID;
   // wake up the barber in the barber_queue
   pthread_cond_signal(&cond_barber_waiting[barbID]);
   pthread_mutex_unlock( &mutex ); // unlock
   return barbID;
}

/**
 * customer waits for the haircut, than leaves the shop
 */
void Shop::leaveShop(int custID, int barbID)
{
   pthread_mutex_lock( &mutex );   // lock

   string s = "wait for barber[" + int2string(barbID) + "] to be done with haircut";
   print(custID, s, false);

   while ( in_service[custID] == true )                           // while being served
      pthread_cond_wait( &cond_customer_served[custID], &mutex );  // just sit.

   money_paid[barbID] = true;

   print( custID, "says good-bye to barber[" + int2string(barbID) + "]", false );

   pthread_cond_signal(&cond_barber_paid[barbID]);
   pthread_mutex_unlock( &mutex ); // unlock
}

/**
 * barber enters the shop to find which customer needs a haircut
 */
int Shop::helloCustomer(int barbID)
{
   pthread_mutex_lock( &mutex );   // lock

   int custID = -1;
   // get the ID of my customer
   custID = barberMap[barbID];
   
   if (custID < 0)
   {
      assert(custID >= 0);
      pthread_mutex_unlock(&mutex);
      return -1;
   }

   print( barbID, 
	   "starts a hair-cut service for customer[" + int2string(custID) + "]", true );

   pthread_mutex_unlock( &mutex );  // unlock
   return custID;
}

/**
 * After giving the haircut, barber waits to be paid, then
 * says goodbye to the customer
 */
void Shop::byeCustomer( int barbID, int custID)
{
   pthread_mutex_lock( &mutex );    // lock

   in_service[custID] = false;
   print( barbID, "says he's done with a hair-cut service for customer[" + 
	   int2string( custID ) + "]", true );

   money_paid[barbID] = false; 
   pthread_cond_signal( &cond_customer_served[custID] );   // tell the customer "done"

   while ( money_paid[barbID] == false )
      pthread_cond_wait( &cond_barber_paid[barbID], &mutex );

   // reset the map entry for this barber
   barberMap[barbID] = -1;
   service_chair = 0;
   ++nServed;
   print( barbID, "calls in another customer", true );
   
   pthread_mutex_unlock( &mutex );  // unlock
}

/**
 * barber is placed in the barber_queue, ready to service a customer
 */
void Shop::waitList(int barbID)
{
   pthread_mutex_lock(&mutex);
   bool sleeping = false;
   
   if (waiting_chairs.empty()) // no waiting customers
   {
      print(barbID, "sleeps because of no customers.", true);
      sleeping = true;
   }
   
   BarberParam b(barbID, sleeping);
   barber_queue.push(b);
   pthread_cond_signal(&cond_customers_waiting); // MOVING THIS TO HERE MAJOR BREAK
   pthread_cond_wait(&cond_barber_waiting[barbID], &mutex);
   pthread_mutex_unlock(&mutex);
}

/**
 * Removes all barbers from the barber_queue. Any barbers still
 * awake are put to sleep.
 */
void Shop::cleanupBarberQueue(int nCustomers)
{
   // pop all the barbers off the queue, and put to sleep any that are awake
   int size = barber_queue.size();   
   
   cout << flush; // flush any pending thread print output
   for (int i = 0; i < size; i++)
   {
      BarberParam b = barber_queue.front();
      int barbID = b.id;
      bool sleeping = b.sleeping;
      
      barber_queue.pop();
      if (!sleeping)
         print(barbID, "sleeps because of no customers", true);
   }
   // sanity checks
   assert(waiting_chairs.empty()); // should be no customers in a wating chair
   assert(nServed + nNotServed == nCustomers); // make sure we didn't loose a customer
}

/**
 * Utility method to convert an int into a string
 */
string Shop::int2string(int i)
{
   stringstream out;
   out << i;
   return out.str();
}

/**
 * print a message for either the barber or the customer
 */
void Shop::print(int person, string message, bool isBarber)
{
   cout << ((isBarber == true) ? "barber  [" : "customer[")
        << person << "]: " << message << endl;
}
