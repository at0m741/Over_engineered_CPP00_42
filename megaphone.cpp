/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouzali <ltouzali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 17:02:24 by ltouzali          #+#    #+#             */
/*   Updated: 2024/07/08 20:26:32 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

#if defined(__AVX512__) || defined(__AVX2__)

#	include "megaphone.hpp"	
#	include <immintrin.h>
					  //
	__attribute__((__always_inline__, __nodebug__)) 
	inline void to_upper_avx(std::string& str) {
		__attribute__((aligned(SIZE))) int			len = str.length();
		const Aligned								lower_a = Setreg('a');
		const Aligned		 						lower_z = Setreg('z');
		const Aligned		 						upper_mask = Setreg(0xDF); // 0xDF -> 11011111

		Prefetch(str.data());	
			Prefetch(str.data() + SIZE);
			PROCESS_CHARS_AVX(str, len, lower_z, lower_a, upper_mask);
			for (int i = 0; i < len; ++i) {
			if (str[i] >= 'a' && str[i] <= 'z') {
				str[i] &= 0xDF; // STR AND= 11011111
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
        std::string str = *av;
#if defined(__AVX2__) || defined(__AVX512__)
        to_upper_avx(str);
#else
		for (int i = 0; i < str.length(); ++i) {
			if (str[i] >= 'a' && str[i] <= 'z') {
				str[i] &= 0xDF; // STR AND= 11011111
			}
		}
#endif
        std::cout << str;
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
