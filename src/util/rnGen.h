/****************************************************************************
  FileName     [ rnGen.h ]
  PackageName  [ util ]
  Synopsis     [ Random number generator ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#ifndef RN_GEN_H
#define RN_GEN_H

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>  
#include <limits.h>
#include <cmath>

#define my_srandom  srandom
#define my_random   random
typedef std::vector<size_t> random_vec_t;

class RandomNumGen
{
   public:
      RandomNumGen() { my_srandom(getpid()); }
      RandomNumGen(unsigned seed) { my_srandom(seed); }
      const int operator() (const int range) const {
         return int(range * (double(my_random()) / INT_MAX) * pow(-1, my_random()));
      }
};

class RandomNumGen2
{
   public:
      RandomNumGen2() { my_srandom(getpid()); }
      RandomNumGen2(unsigned seed) { my_srandom(seed); }
      const random_vec_t operator () (const size_t length) const {
         random_vec_t random_vec;
         for(size_t i = 0; i < length; ++i)
         {
            random_vec.push_back(size_t(my_random() * (RAND_MAX + 1) + my_random()));
         }
         return random_vec;
      }
};

#endif // RN_GEN_H

