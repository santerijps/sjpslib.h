/*
   A C-program for MT19937-64 (2004/9/29 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using rand_init(seed)
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
     ACM Transactions on Modeling and
     Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
     ``Mersenne Twister: a 623-dimensionally equidistributed
       uniform pseudorandom number generator''
     ACM Transactions on Modeling and
     Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/e_random_mt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/

#ifndef SJPSLIB_RANDOM_H

  #define SJPSLIB_RANDOM_H

  #define _RANDOM_NN 312
  #define _RANDOM_MM 156
  #define _RANDOM_UM 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
  #define _RANDOM_LM 0x7FFFFFFFULL /* Least significant 31 bits */

  unsigned long long ts_now(void);

  /* The array for the state vector */
  static unsigned long long _random_mt[_RANDOM_NN];
  /* _random_mti==_RANDOM_NN+1 means _random_mt[_RANDOM_NN] is not initialized */
  static int _random_mti=_RANDOM_NN+1;

  /* initializes _random_mt[_RANDOM_NN] with a seed */
  void rand_init(unsigned long long seed)
  {
    _random_mt[0] = seed;
    for (_random_mti=1; _random_mti<_RANDOM_NN; _random_mti++)
      _random_mt[_random_mti] = (6364136223846793005ULL * (_random_mt[_random_mti-1] ^ (_random_mt[_random_mti-1] >> 62)) + _random_mti);
  }

  /* Returns a random number on [0, 2^64-1]-interval */
  unsigned long long rand_u64(void)
  {
    int i;
    unsigned long long x;
    static unsigned long long mag01[2]={0ULL, 0xB5026F5AA96619E9ULL};

    if (_random_mti >= _RANDOM_NN) { /* generate _RANDOM_NN words at one time */

        /* if rand_init() has not been called, */
        /* a default initial seed is used     */
        if (_random_mti == _RANDOM_NN+1)
            rand_init(ts_now());

        for (i=0;i<_RANDOM_NN-_RANDOM_MM;i++) {
            x = (_random_mt[i]&_RANDOM_UM)|(_random_mt[i+1]&_RANDOM_LM);
            _random_mt[i] = _random_mt[i+_RANDOM_MM] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
        }
        for (;i<_RANDOM_NN-1;i++) {
            x = (_random_mt[i]&_RANDOM_UM)|(_random_mt[i+1]&_RANDOM_LM);
            _random_mt[i] = _random_mt[i+(_RANDOM_MM-_RANDOM_NN)] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
        }
        x = (_random_mt[_RANDOM_NN-1]&_RANDOM_UM)|(_random_mt[0]&_RANDOM_LM);
        _random_mt[_RANDOM_NN-1] = _random_mt[_RANDOM_MM-1] ^ (x>>1) ^ mag01[(int)(x&1ULL)];

        _random_mti = 0;
    }

    x = _random_mt[_random_mti++];

    x ^= (x >> 29) & 0x5555555555555555ULL;
    x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
    x ^= (x << 37) & 0xFFF7EEE000000000ULL;
    x ^= (x >> 43);

    return x;
  }

  /* Returns a random number between _min and _max, inclusive. */
  unsigned long long rand_range(unsigned long long _min, unsigned long long _max)
  {
    unsigned long long r = rand_u64();
    return r % (_max + 1 - _min) + _min;
  }

  /* generates a random number on [0, 2^63-1]-interval */
  long long rand_i63(void)
  {
    return (long long)(rand_u64() >> 1);
  }

  /* generates a random number on [0,1]-real-interval */
  double rand_real1(void)
  {
    return (rand_u64() >> 11) * (1.0/9007199254740991.0);
  }

  /* generates a random number on [0,1)-real-interval */
  double rand_real2(void)
  {
    return (rand_u64() >> 11) * (1.0/9007199254740992.0);
  }

  /* generates a random number on (0,1)-real-interval */
  double rand_real3(void)
  {
    return ((rand_u64() >> 12) + 0.5) * (1.0/4503599627370496.0);
  }

#endif