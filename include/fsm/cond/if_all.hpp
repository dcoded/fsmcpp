#pragma once
#include <functional>

/**
 * @brief      AND logic for only one conditional statement function
 *
 * @param[in]  head  The head
 *
 * @tparam     Head  std::function<bool()> binded conditional function
 *
 * @return     binded result of true iff conditional return true
 */
template<typename Head>
std::function<bool()> if_all(Head& head)
{
    return head;
}

/**
 * @brief      AND logic for multiple conditional statement functions
 *
 * @param[in]  head  Next conditional to be check
 * @param[in]  tail  Other conditonal to be checked recursively
 *
 * @tparam     Head  A std::function<bool()> to be checked first
 * @tparam     Tail  Other std::function<bool()> conditional checked next
 *
 * @return     binded result of true iff all conditionals return true
 */
template<typename Head, typename...Tail>
std::function<bool()> if_all(Head& head, Tail&...tail)
{
    return [&head, &tail...]() -> bool
    {
        return head() && if_all(tail...)();
    };
}
