#ifndef _ISTL_FUNCTIONAL_H_
#define _ISTL_FUNCTIONAL_H_

namespace istl
{

    template <typename T>
    struct less
    {
        bool operator()(const T& lhs, const T& rhs) const {
            return lhs < rhs;
        }
    };

} // namespace istl

#endif