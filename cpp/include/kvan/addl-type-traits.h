// -*- c++ -*-
#ifndef __KVAN_ADDL_TYPE_TRAITS__HH__
#define __KVAN_ADDL_TYPE_TRAITS__HH__

#include <type_traits>
#include <vector>

// from https://stackoverflow.com/a/57812868/1181482
template<typename T>
struct is_string
        : public std::disjunction<
                std::is_same<char *, typename std::decay<T>::type>,
                std::is_same<const char *, typename std::decay<T>::type>,
                std::is_same<std::string, typename std::decay<T>::type>
        > {
};

// https://stackoverflow.com/a/31105859/1181482
template <typename T> struct is_vector: std::false_type {};
template <typename... Args> struct is_vector<std::vector<Args...>> : std::true_type{};


#endif