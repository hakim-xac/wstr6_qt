#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <type_traits>
#include <utility>

namespace WSTR {

template <typename T, typename = void>
struct IsMap : std::false_type {};

template <typename T>
struct IsMap<T, std::enable_if_t<
        std::is_same_v<
        typename T::value_type, std::pair<const typename T::key_type, typename T::mapped_type>
        >>> : std::true_type {};

template <typename T>
static constexpr bool IsMap_v(){
    return IsMap<T>::value;
}

}

#endif // CONCEPTS_H
