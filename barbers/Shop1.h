
/**
 * Shop1.h
 * Class declarations for multiple sleeping barbers Shop monitor.
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

#pragma once

#define NDEBUG // disable for DEBUG mode

#include <pthread.h>
#include <queue>
#include <map>
#include <cassert>

using namespace std;

#define DEFAULT_CHAIRS 3

class Shop; // forward declaration

/**
 * data structure passed to customer and barber threads
 */
struct ThreadParam
{
   Shop* shop = nullptr;
   int id = 0; // thread id
   int serviceTime = 0; // service time in usec (baber), but 0 for cust
   ThreadParam (Shop* _shop, int _id, int _serviceTime ) : 
         shop(_shop), id (_id), serviceTime( _serviceTime) {};
};

/**
 * data structure stored in barber_queue
 * contains barber ID and boolean if sleeping
 */
struct BarberParam{
   int id = -1;
   bool sleeping = false;
   BarberParam(int bID, bool sleep) : id(bID), sleeping(sleep) {}
};

/**
 * Monitor class for emulated barber shop
 */
class Shop
{
public:
   // version 1: 0 nChairs actually means 0 nChairs (max == 0) 
   Shop(int nBarb, int nChairs) : max (nChairs), version(1) {}; // v 1
   // verson 2: 0 nChairs defaults to 3 chairs
   // Shop(int nBarb, int nChairs) : 
   //    max( ( nChairs > 0 ) ? nChairs : DEFAULT_CHAIRS ), version(2) {};// v 2

   Shop( int nChairs ) : 
      max( ( nChairs > 0 ) ? nChairs : DEFAULT_CHAIRS ) {};

   Shop( ) : max( DEFAULT_CHAIRS ) {};

   /**
    * Destructor
    * Free all allocated memory
    */
   ~Shop();

   /**
    * accessor to get the program version
    */
   int getVersion() { return version; }

   /**
    * init
    * Allocate memory for arrays.
    * Initialize all cond_t condition variables
    */
   void init(int nCustomers, int nBarbers);

   /**
    * customer visits the shop to get a haircut
    * return -1 if no service, else return the barber ID
    */
   int visitShop( int custID ); 

   /**
    * customer waits for the haircut, than leaves the shop
    */
   void leaveShop( int custID, int barbID );
   
   /**
    * barber enters the shop to find which customer needs a haircut
    */
   int helloCustomer(int barbID);
   
   /**
    * After giving the haircut, barber waits to be paid, then
    * says goodbye to the customer
    */
   void byeCustomer(int barbID, int custID);
   
   /**
    * barber is placed in the barber_queue, ready to service a customer
    */
   void waitList(int barberID);

   /**
    * Removes all barbers from the barber_queue. Any barbers still
    * awake are put to sleep.
    */
   void cleanupBarberQueue(int nCustomers);
   
   int nNotServed = 0; // the number of customers not served
   int nServed = 0; // number of customers served - sanity check

private:
   const unsigned int max; // the max number of customer threads that can wait
   bool* in_service;            
   bool* money_paid;
   int service_chair = 0;
   int version = -1; // program version
   queue<int> waiting_chairs;  // includes the ids of all waiting threads
   queue<BarberParam> barber_queue;
   map<int, int> barberMap; // customer associated with a barber, <barbID, custID>

   pthread_mutex_t mutex;
   pthread_cond_t  cond_customers_waiting; // single, all reamining are arrays
   pthread_cond_t*  cond_customer_served;
   pthread_cond_t*  cond_barber_paid;
   pthread_cond_t*  cond_barber_waiting;
   
   /**
    * Utility method to convert an int into a string
    */
   string int2string( int i );
   
   /**
    * print a message for either the barber or the customer
    */
   void print( int person, string message, bool isBarber );
};

