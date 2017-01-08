#pragma once
#include <functional>

/**
 * @brief      NOT logic for only one conditional statement function
 *
 * @param[in]  head  a conditional statement function
 *
 * @tparam     Head  std::function<bool()> binded conditional function
 *
 * @return     binded result of true if any conditional return true
 */
template<typename Head>
std::function<bool()> if_not(Head& head)
{
    return [&head]() -> bool
    {
        return !head();
    };
}
