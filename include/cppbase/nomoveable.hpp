#ifndef CPPBASE_NONMOVABLE_HPP
#define CPPBASE_NONMOVABLE_HPP

namespace cppbase {

/**
 * @brief Base class for non-movable objects, disable move construction and move assignment
 * @note Empty base optimization (EBO), zero memory and performance overhead
 * @note Inherit this class for core components that should not be moved
 */
class NoMoveable
{
public:
    NoMoveable() = default;
    NoMoveable(NoMoveable &&) noexcept = delete;
    NoMoveable &operator=(NoMoveable &&) = delete;
};

} // namespace cppbase

#endif // CPPBASE_NONMOVABLE_HPP