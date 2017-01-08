#pragma once
#include <functional>

/**
 * @brief      OR logic for only one conditional statement function
 *
 * @param[in]  head  a conditional statement function
 *
 * @tparam     Head  std::function<bool()> binded conditional function
 *
 * @return     binded result of true if any conditional return true
 */
template<typename Head>
std::function<bool()> if_any(Head& head)
{
    return head;
}

/**
 * @brief      OR logic for only multiple conditional statement functions
 *
 * @param[in]  head  next conditional to check
 * @param[in]  tail  other conditonal to be checked recursively
 *
 * @tparam     Head  A std::function<bool()> to be checked first
 * @tparam     Tail  Other std::function<bool()> conditional checked next
 *
 * @return     binded result of true if any conditional return true
 */
template<typename Head, typename...Tail>
std::function<bool(void)> if_any(Head& head, Tail&...tail)
{
    return [&head, &tail...]() -> bool
    {
        return head() || if_any(tail...)();
    };
}
