#include "CalculatorApp.hpp"

/*
#include <iostream>
#include <iomanip>
template <typename T>
inline void dump(const T& obj)
{
    const uint8_t * pObj = reinterpret_cast<const uint8_t*>(&obj);
    std::cout << std::hex << std::uppercase << std::setfill('0');

    for (unsigned int i = 0; i < sizeof(obj); ++i)
        std::cout << std::setw(2) << static_cast<unsigned>(pObj[i]) << ' ';

    std::cout << '\n';
}
*/

int main(void)
{
    CalculatorApp calc;
    calc.run();
    return EXIT_SUCCESS;
}