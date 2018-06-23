/**
 * driver1.cpp
 * Driver for multiple sleeping barbers monitor class, 
 * customer threads and barber threads.
 * Program version 1 (max does not default to 3 if nChairs is 0)
 * 
 * To compile this program:
 * ./make1
 * executable is named 'sleepingBarbers1'
 * 
 * Thurman Gillespy
 * 5/8/18
 */

#include <iostream>     // cout
#include <sys/time.h>   // gettimeofday
#include <unistd.h>     // usleep
#include "Shop1.h"

// forward declarations
void *barber( void * );
void *customer( void * );

using namespace std;

/**
 * Driver for barber shop
 */
int main( int argc, char *argv[] )
{
   int nBarbers =    atoi(argv[1]); // # barbers in shop
   int nChairs =     atoi(argv[2]); // # waiting chairs
   int nCustomers =  atoi(argv[3]); // # customers
   int serviceTime = atoi(argv[4]); // time to cut hair (usec)

   pthread_t barber_thread[nBarbers];
   pthread_t customer_threads[nCustomers];
   Shop shop( nBarbers, nChairs );
   shop.init(nCustomers, nBarbers);

   cout << "sleepingBarbers " << nBarbers << " " << nChairs << " ";
   cout << nCustomers << " " << serviceTime << endl;
   cout << "program version: " << shop.getVersion() << endl;

   // barbers begin at id 0
   for (int i = 0; i < nBarbers; i++)
   {
      int id = i;
      ThreadParam* tParam = new ThreadParam(&shop, id, serviceTime);
      pthread_create( &barber_thread[i], NULL, barber, tParam );
      usleep(100);
   }

   // customers begin at id 1
   for ( int i = 0; i < nCustomers; i++ )
   {
      usleep( (rand() % 1000) + 100);
      int id = i + 1;
      ThreadParam* tParam = new ThreadParam(&shop, id, 0);
      pthread_create( &customer_threads[i], NULL, customer, tParam );
   }

   // wait for all the customer threads to finish
   for ( int i = 0; i < nCustomers; i++ )
      pthread_join( customer_threads[i], NULL );

   shop.cleanupBarberQueue(nCustomers);
   usleep(500);
    
   // kill all the barber threads
   for (int i = 0; i < nBarbers; i++)
      pthread_cancel( barber_thread[i] );
   
   usleep(100); // let stdout settle
   cout << flush; // flush the stdout buffer
   cout << "# customers who didn't receive a service = " << shop.nNotServed
       << endl;

  return EXIT_SUCCESS;
}

/**
 * barber thread
 * cuts customer hair
 */
void *barber( void *arg )
{

   ThreadParam &tParam = *(ThreadParam *)arg;
   Shop &shop = *(tParam.shop);
   int barbID = tParam.id;
   int serviceTime = tParam.serviceTime;

   delete (ThreadParam *)arg; 

   while (true)
   {
      shop.waitList(barbID);
      int custID = shop.helloCustomer(barbID);
      usleep(serviceTime);
      if (custID > 0) // customers beging at id 1
         shop.byeCustomer(barbID, custID);
   }

  return nullptr;
}

/**
 * customer thread
 * customer enters barber shop to get a haircut
 */
void *customer( void *arg )
{
   ThreadParam& tParam = *(ThreadParam *)arg;
   Shop& shop = *(tParam.shop);
   int custID = tParam.id;
   
   delete (ThreadParam *)arg; // BUG - causes crash
   
   int barbID = shop.visitShop(custID);
   if ( barbID >= 0 ) // barbers begin at id 0
      shop.leaveShop (custID, barbID);

   return nullptr;
}
