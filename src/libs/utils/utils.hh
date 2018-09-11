#ifndef _UTILS_HH
#define _UTILS_HH

namespace yapet {
template <class T>
inline void deleteAndZero(T **ptr) {
    if (ptr == nullptr || *ptr == nullptr) return;

    delete (*ptr);
    *ptr = nullptr;
}
}  // namespace yapet

#endif