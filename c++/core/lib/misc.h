#ifndef MISC_H
#define MISC_H
#include <vector>
#include <fstream>
#include <cstring>
#include <array>

template<typename T1, typename T2> 
class Robin_Hash {
    public:
        Robin_Hash(T1 link, T2 value) {

        }

        T2 at(T1 link) {
            
        }
};

template <typename T, std::size_t S>
std::array<T, S> operator+=(std::array<T, S>& a, std::array<T,S>& b)
{
    for (size_t i{}; i < S; i++) {
        a[i] += b[i];
    }
}
#endif