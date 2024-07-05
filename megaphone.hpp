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

#if defined (__AVX2__)

	#include <immintrin.h>

	#define ALIGN32			__attribute__((aligned(32)))

	#define Loadreg_32(ptr)		_mm256_loadu_si256((__m256i*)(ptr))
	#define Storereg_32(ptr, data)	_mm256_storeu_si256((__m256i*)(ptr), data)
	#define Andnot_32(mask, data)	_mm256_andnot_si256(mask, data)
	#define And_32(data, mask)	_mm256_and_si256(data, mask)
	#define Or_32(data1, data2)	_mm256_or_si256(data1, data2)
	#define Sub_32(data1, data2)	_mm256_sub_epi8(data1, data2)
	#define Setreg_32(value)	_mm256_set1_epi8(value)
	#define Prefetch_32(ptr)	_mm_prefetch((const char *)(ptr), _MM_HINT_T0)
	#define Cmp_32(data1, data2)	_mm256_cmpgt_epi8(data1, data2)
	#define Aligned_32		__attribute__((aligned(32))) __m256i
	


	#define PROCESS_32_CHARS_AVX2(str, i, len, lower_z, lower_a, upper_mask)						\
		for (int i = 0; i <= len - 32; i += 32) {									\
			const Aligned_32 data  = Loadreg_32(str.data() + i);							\
	        	const Aligned_32  mask = Andnot_32(Cmp_32(data, lower_z),Cmp_32(data, Sub_32(lower_a, Setreg_32(1))));  \
			const Aligned_32  result = Or_32(And_32(mask, And_32(data, upper_mask)),Andnot_32(mask, data));         \
			Prefetch_32(str.data() + i + 32);									\
			Storereg_32((__m256i*)(str.data() + i), result);							\
		}

#elif defined (__AVX512)
	
	//AVX512f implementation
	// same as the AVX2 implementation, but with 512-bit registers and instructions.
	//if CPU does not support AVX512f, the code will use the AVX2 implementation.

	#include <immintrin.h>

	#define ALIGN64			__attribute__((aligned(64)))

	#define Loadreg_64(ptr)		_mm512_loadu_si512((__m512i*)(ptr))
	#define Storereg_64(ptr, data)	_mm512_storeu_si512((__m512i*)(ptr), data)
	#define Andnot_64(mask, data)	_mm512_andnot_si512(mask, data)
	#define And_64(data, mask)	_mm512_and_si512(data, mask)
	#define Or_64(data1, data2)	_mm512_or_si512(data1, data2)
	#define Sub_64(data1, data2)	_mm512_sub_epi8(data1, data2)
	#define Setreg_64(value)	_mm512_set1_epi8(value)
	#define Prefetch_64(ptr)	_mm_prefetch((const char *)(ptr), _MM_HINT_T0)
	#define Cmp_64(data1, data2)	_mm512_cmpgt_epi8_mask(data1, data2)
	#define Aligned_64		__attribute__((aligned(64))) __m512i



	#define PROCESS_64_CHARS_AVX512(str, i, len, lower_z, lower_a, upper_mask)						\
		for (int i = 0; i <= len - 64; i += 64) {									\
			Aligned_64 data  = Loadreg_64(str.data() + i);								\
			Aligned_64 mask  = Andnot_64(Cmp_64(data, lower_z), Cmp_64(data, Sub_64(lower_a, Setreg_64(1)));        \
			Aligned_64 result = Or_64(And_64(mask, And_64(data, upper_mask)), Andnot_64(mask, data));		\
			Prefetch_64(str.data() + i + 64);									\
			Storereg_64((__m512i*)(str.data() + i), result);							\
		}
#endif

