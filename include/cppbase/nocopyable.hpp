#ifndef CPPBASE_NONCOPYABLE_HPP
#define CPPBASE_NONCOPYABLE_HPP

namespace cppbase {

/**
 * @brief A base class to make derived classes non-copyable and non-assignable
 * @note Core design strategy for C++ object-oriented programming
 * 
 * @details This class uses C++11 delete semantics to disable copy constructor 
 *          and copy assignment operator at compile time. Any class that inherits 
 *          from NonCopyable will automatically lose the ability to be copied or assigned.
 */
class NoCopyable
{
public:
    NoCopyable() = default;
    NoCopyable(const NoCopyable &) = delete;
    NoCopyable &operator=(const NoCopyable &) = delete;
}; 

} // namespace cppbase

#endif // CPPBASE_NONCOPYABLE_HPP