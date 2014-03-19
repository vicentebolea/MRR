#include <omp.h>
#include <orthrus.hh>
#include <UnitTest++.h>
//
// --------UNIT TEST AUTOMATICALLY GENERATED------------
//
struct fix_cache {
 Orthrus* victim;

 fix_cache () {
  victim = new Orthrus ();
 }
 ~fix_cache () {
  delete victim;
 }
};
//
// --------UNIT TEST AUTOMATICALLY GENERATED------------
//
SUITE (CACHE_TEST) {
 // ----------------------------------------------------
 TEST_FIXTURE (fix_cache, ctor_dtor) { }

 // ----------------------------------------------------
 TEST_FIXTURE (fix_cache, normal) { 
  (*victim).set_size (4) 
   .set_policy (CACHE_LRU) 
   .set_port (9000)
   .set_iface ("eth0")
   .set_host ("localhost")
   .set_network ({"127.0.0.1", "127.0.0.2"})
   .bind() .run ();

  victim->insert ("0", "Hello");
  victim->insert ("1", "Hola");
  victim->insert ("2", "Allo");
  victim->insert ("3", "Annyeong");
  victim->insert ("4", "Seno");

  CHECK_EQUAL("Hello",    std::get<0> (victim->lookup ("0")));
  CHECK_EQUAL("Hola",     std::get<0> (victim->lookup ("1")));
  CHECK_EQUAL("Allo",     std::get<0> (victim->lookup ("2")));
  CHECK_EQUAL("Annyeong", std::get<0> (victim->lookup ("3")));
  CHECK_EQUAL("Seno",     std::get<0> (victim->lookup ("4")));
   
  victim->close();
 }

 // ----------------------------------------------------
 TEST_FIXTURE (fix_cache, lru) { 
  (*victim)
   .set_size (3) 
   .set_policy (CACHE_LRU) 
   .set_port (9000)
   .set_iface ("eth0")
   .set_host ("localhost")
   .set_network ({"127.0.0.1", "127.0.0.2"})
   .bind() .run ();

  victim->insert ("0", "Hello");
  victim->insert ("1", "Hola");
  victim->insert ("2", "Allo");
  victim->insert ("3", "Annyeong");
  CHECK_THROW (victim->lookup ("0"), std::out_of_range);

  victim->insert ("4", "Seno");

  CHECK_EQUAL ("Hello",    std::get<0> (victim->lookup ("0")));
  CHECK_EQUAL ("Hola",     std::get<0> (victim->lookup ("1")));
  CHECK_EQUAL ("Allo",     std::get<0> (victim->lookup ("2")));
  CHECK_EQUAL ("Annyeong", std::get<0> (victim->lookup ("3")));
  CHECK_EQUAL ("Seno",     std::get<0> (victim->lookup ("4")));
 }

 // ----------------------------------------------------
 TEST(parallel) { 
  omp_lock_t lock, lock2;
  omp_init_lock (&lock);
  omp_init_lock (&lock2);
  omp_set_lock (&lock);         //! Set Lock 1 to 1
  omp_set_lock (&lock2);        //! Set lock 2 to 1

#pragma omp parallel sections
  {
#pragma omp section
   {
    //First Node
    Cache cache = Cache ()
     .set_size (4) 
     .set_policy (CACHE_LRU) 
     .set_port (9000)
     .set_iface ("eth0")
     .set_host ("localhost")
     .set_network ({"127.0.0.1", "127.0.0.2"})
     .bind () .run ();

    omp_unset_lock (&lock2);       //! Wait till the other thread finish

    cache.insert ("10", "test1");
    cache.insert ("20", "test2");
    cache.insert ("30", "test3");

    cache.close ();
   }
#pragma omp section
   {
    //First Node
    Cache cache = Cache ()
     .set_size (4) 
     .set_policy (CACHE_LRU) 
     .set_port (9000) // 9001 9002 9003
     .set_iface ("eth0")
     .set_host ("localhost")
     .set_network ({"127.0.0.1", "127.0.0.2"})
     .bind () .run ();

    omp_set_lock (&lock2);       //! Wait till the other thread finish
    CHECK (cache.insert ("10", "test1") == true);  //! It test if the dp was migrated

    cache.close ();
   }
  }
 }
}
// -----------------------------------------------------