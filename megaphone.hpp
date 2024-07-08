#pragma once
#include <string>
#include <iostream>

//-_mm256_cmpge_epi8(data, lower_a) compares each character in data with 'a' and returns a mask where 
//  	the bits are 1 if the character is greater than or equal to 'a' same dumb CPU wisper with z.
//-_mm256_and_si256 is used to combine each mask, which gives a mask where the bits
//	    are 1 only for lowercase letters.
//-_mm256_and_si256(data, upper_mask) applies the upper_mask (0xDF) to data characters, 
//   	which converts lowercase letters to uppercase.
//-_mm256_and_si256(data, upper_mask) applies the upper_mask (0xDF) to data characters,
//		which converts lowercase letters to uppercase.
//-_mm256_andnot_si256(mask, data) selects data characters that are not lowercase letters.
//-_mm256_or_si256 combines the results of the previous two operations, converting lowercase letters 
//		to uppercase and leaving other characters unchanged.


	#include <immintrin.h>

#if defined (__AVX2__)
#	define SIZE						32
#	define ALIGN					__attribute__((aligned(SIZE)))
 	
#	define Loadreg(ptr)				_mm256_loadu_si256((__m256i*)(ptr))
#	define Storereg(ptr, data)		_mm256_storeu_si256((__m256i*)(ptr), data)
#	define Andnot(mask, data)		_mm256_andnot_si256(mask, data)
#	define And(data, mask)			_mm256_and_si256(data, mask)
#	define Or(data1, data2)			_mm256_or_si256(data1, data2)
#	define Sub(data1, data2)		_mm256_sub_epi8(data1, data2)
#	define Setreg(value)			_mm256_set1_epi8(value)
#	define Prefetch(ptr)			_mm_prefetch((const char *)(ptr), _MM_HINT_T0)
#	define Cmp(data1, data2)		_mm256_cmpgt_epi8(data1, data2)
#	define Aligned					ALIGN __m256i
#elif (__AVX512__)
#	define SIZE						64
#	define ALIGN64					__attribute__((aligned(SIZE)))
 	
#	define Loadreg(ptr)				_mm512_loadu_si512((__m512i*)(ptr))
#	define Storereg(ptr, data)		_mm512_storeu_si512((__m512i*)(ptr), data)
#	define Andnot(mask, data)		_mm512_andnot_si512(mask, data)
#	define And(data, mask)			_mm512_and_si512(data, mask)
#	define Or(data1, data2)			_mm512_or_si512(data1, data2)
#	define Sub(data1, data2)		_mm512_sub_epi8(data1, data2)
#	define Setreg(value)			_mm512_set1_epi8(value)
#	define Prefetch(ptr)			_mm_prefetch((const char *)(ptr), _MM_HINT_T0)
#	define Cmp(data1, data2)		_mm512_cmpgt_epi8_mask(data1, data2)
#	define Aligned					__attribute__((aligned(SIZE))) __m512i
#endif
	

#if defined(__AVX2__) || defined(__AVX512__)

#	define PROCESS_CHARS_AVX(str, len, lower_z, lower_a, upper_mask)			  \
 	 for (int i = 0; i <= len - SIZE; i += SIZE) {                                \
 	   const Aligned data = Loadreg(str.data() + i);                              \
 	   const Aligned mask =                                                       \
 	       Andnot(Cmp(data, lower_z), Cmp(data, Sub(lower_a, Setreg(1))));        \
 	   const Aligned result =                                                     \
 	       Or(And(mask, And(data, upper_mask)), Andnot(mask, data));              \
 	   Prefetch(str.data() + i + SIZE);                                           \
 	   Storereg((__m256i *)(str.data() + i), result);                             \
 	 }
#endif
