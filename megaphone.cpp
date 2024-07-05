/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouzali <ltouzali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 17:02:24 by ltouzali          #+#    #+#             */
/*   Updated: 2024/07/06 01:08:34 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

#if defined(__AVX2__)
	#include "megaphone.hpp"	
    	#include <immintrin.h>
	#include <cstring>
	#include <cstdio> // use for puts if you want to prove that the code is working
					  //
	__attribute__((__always_inline__, __nodebug__)) 
	inline void to_upper_avx2(std::string& str) {
		__attribute__((aligned(32))) int len = strlen(str.c_str());
	   	const Aligned_32 lower_a = Setreg_32('a');
	    	const Aligned_32 lower_z = Setreg_32('z');
	    	const Aligned_32 upper_mask = Setreg_32(0xDF); // 0xDF -> 11011111
	   	Prefetch_32(str.data());	
	    	Prefetch_32(str.data() + 32);
	    	PROCESS_32_CHARS_AVX2(str, i, len, lower_z, lower_a, upper_mask);
	    	for (int i = 0; i < len; ++i) {
			if (str[i] >= 'a' && str[i] <= 'z') {
				str[i] &= 0xDF; // STR AND= 11011111
			}
		}
	}

#elif defined(__AVX512__)
	#include "megaphone.hpp"
	#include <immintrin.h>
	#include <cstring>
	
	__attribute__((__always_inline__, __nodebug__))
	inline void to_upper_avx512(std::string& str) {
		__attribute__((aligned(64))) int len = strlen(str.c_str());

		const ALIGN64 __m512i lower_a = _mm512_set1_epi8('a');
        	const ALIGN64 __m512i lower_z = _mm512_set1_epi8('z');
        	const ALIGN64 __m512i upper_mask = _mm512_set1_epi8(0xDF);
        	PREFETCH64(str.data());
		PREFETCH64(str.data() + 32);
		PREFETCH64(str.data() + 64);		
		PROCESS_64_CHARS_AVX512(str, i, len, lower_z, lower_a, upper_mask);
        	for (int i = 0; i < len; ++i) {
            		if (str[i] >= 'a' && str[i] <= 'z') {
                		str[i] &= 0xDF;
            		}
        	}
    	}
#endif

int megaphone(char **av)
{
    av++;
    if (!*av)
    {
        std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
        return (1);
    }
    for (; *av; av++)
    {
#if defined(__AVX2__)
        std::string str = *av;
        to_upper_avx2(str);
        std::cout << str;
#elif defined(AVX512f)
        std::string str = *av;
        to_upper_avx512(str);
        std::cout << str;
#else
        std::string str = *av;
        std::transform(str.begin(), str.end(), str.begin(),  (int(*)(int)) std::toupper);
        std::cout << str;
#endif
        if (*(av + 1))
            std::cout << " ";
    }
    std::cout << std::endl;
    return (0);
}

int main(__attribute__((unused)) int ac, char **av)
{
    megaphone(av);
}
