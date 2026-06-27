#pragma once

// Include via deleter.hpp which handles the C++ `delete` keyword conflict
// in libnpc's array.h.  Do NOT include <array.h> directly.
#include "detail/deleter.hpp"
#include <complex>

namespace npc {

/// C++ enum mirroring libnpc's DataType.
/// Values are directly castable to/from ::DataType.
enum class DType : int {
    Bool      = BOOL,
    Int8      = INT8,
    Int16     = INT16,
    Int32     = INT32,
    Int64     = INT64,
    UInt8     = UINT8,
    UInt16    = UINT16,
    UInt32    = UINT32,
    UInt64    = UINT64,
    Float32   = FLOAT32,
    Float64   = FLOAT64,
    Float128  = FLOAT128,
    Complex64  = COMPLEX64,
    Complex128 = COMPLEX128,
    Complex256 = COMPLEX256,
    DateTime64 = DATETIME64,
    String    = STRING,
    Unknown   = UNKNOWN,
};

/// Compile-time mapping from C++ type to DType.
/// Specializations for each supported type.
template <typename T> struct dtype_of;

template <> struct dtype_of<bool>            { static constexpr DType value = DType::Bool; };
template <> struct dtype_of<int8_t>          { static constexpr DType value = DType::Int8; };
template <> struct dtype_of<int16_t>         { static constexpr DType value = DType::Int16; };
template <> struct dtype_of<int32_t>         { static constexpr DType value = DType::Int32; };
template <> struct dtype_of<int64_t>         { static constexpr DType value = DType::Int64; };
template <> struct dtype_of<uint8_t>         { static constexpr DType value = DType::UInt8; };
template <> struct dtype_of<uint16_t>        { static constexpr DType value = DType::UInt16; };
template <> struct dtype_of<uint32_t>        { static constexpr DType value = DType::UInt32; };
template <> struct dtype_of<uint64_t>        { static constexpr DType value = DType::UInt64; };
template <> struct dtype_of<float>           { static constexpr DType value = DType::Float32; };
template <> struct dtype_of<double>          { static constexpr DType value = DType::Float64; };
template <> struct dtype_of<long double>     { static constexpr DType value = DType::Float128; };
template <> struct dtype_of<std::complex<float>>   { static constexpr DType value = DType::Complex64; };
template <> struct dtype_of<std::complex<double>>  { static constexpr DType value = DType::Complex128; };
template <> struct dtype_of<std::complex<long double>> { static constexpr DType value = DType::Complex256; };

/// Convenience: get the ::DataType for a C++ type.
template <typename T>
inline constexpr DataType to_c_dtype() {
    return static_cast<DataType>(dtype_of<T>::value);
}

} // namespace npc
