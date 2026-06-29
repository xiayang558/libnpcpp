# libnpc++

[English](README.md) | [中文](README_zh.md) | [📘 API Documentation](https://xiayang558.github.io/libnpcpp)

C++17 RAII wrapper for [libnpc](https://github.com/xiayang558/libnpc) — a NumPy-compatible numerical computing library written in pure C with **zero Python dependency**.

[![Version](https://img.shields.io/badge/version-1.0.1-blue)](https://github.com/xiayang558/libnpcpp)
[![C++17](https://img.shields.io/badge/C++-17-blue)](https://en.cppreference.com/w/cpp/17)
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE)

libnpc++ provides modern C++ idioms on top of libnpc's ~335 C functions:

- **RAII** — automatic memory management, no manual `free_array()` calls
- **Operator overloading** — `a + b`, `a * 2.0`, `-a`, `2.0 / a`, `a += b`
- **Convenience indexing** — `a(i, j)`, `a[3]` bracket access, typed `a.at<T>({i, j})`
- **Stream output** — `std::cout << a` prints array metadata
- **Move semantics** — efficient ownership transfer
- **Type safety** — compile-time `dtype_of<T>` mapping, typed `begin<T>()`/`end<T>()` iterators
- **NumPy-compatible defaults** — `axis=-1` for all elements, `keepdims=false`

## Build & Install

### Prerequisites

- **C++17 compiler** (GCC 8+, Clang 7+, Apple Clang 12+)
- **CMake 3.16+**
- **libzip** (for NPZ file I/O):
  - macOS: `brew install libzip`
  - Linux: `apt install libzip-dev`

### Build

```bash
git clone --recurse-submodules https://github.com/xiayang558/libnpcpp.git
cd libnpcpp
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Run Tests

```bash
cd build
ctest --output-on-failure
```

### Use in Your CMake Project

```cmake
add_subdirectory(path/to/libnpcpp)
target_link_libraries(my_app PRIVATE libnpc::npcpp)
```

## Data Types

libnpc++ maps each libnpc C `DataType` to a scoped C++ `DType` enum.

| DType | C++ Type | Size | Description |
|---|---|---|---|
| `Bool` | `bool` | 1 | Boolean |
| `Int8` | `int8_t` | 1 | Signed 8-bit integer |
| `Int16` | `int16_t` | 2 | Signed 16-bit integer |
| `Int32` | `int32_t` | 4 | Signed 32-bit integer |
| `Int64` | `int64_t` | 8 | Signed 64-bit integer |
| `UInt8` | `uint8_t` | 1 | Unsigned 8-bit integer |
| `UInt16` | `uint16_t` | 2 | Unsigned 16-bit integer |
| `UInt32` | `uint32_t` | 4 | Unsigned 32-bit integer |
| `UInt64` | `uint64_t` | 8 | Unsigned 64-bit integer |
| `Float32` | `float` | 4 | 32-bit floating point |
| `Float64` | `double` | 8 | 64-bit floating point **(default)** |
| `Float128` | `long double` | 16 | 128-bit floating point |
| `Complex64` | `std::complex<float>` | 8 | 64-bit complex |
| `Complex128` | `std::complex<double>` | 16 | 128-bit complex |
| `Complex256` | `std::complex<long double>` | 32 | 256-bit complex |
| `DateTime64` | `DateTime64` | 8 | Date/time value |
| `String` | — | varies | String type |
| `Unknown` | — | — | Unknown/unsupported |

### Compile-time type mapping

```cpp
npc::dtype_of<double>::value          // → npc::DType::Float64
npc::dtype_of<int32_t>::value         // → npc::DType::Int32
npc::dtype_of<std::complex<float>>::value  // → npc::DType::Complex64
```

## Quick Start

```cpp
#include <npc/npc.hpp>

int main() {
    // ── Array creation ──────────────────────────────
    auto a = npc::ones({3, 3});              // 3×3 matrix of 1.0
    auto b = npc::full({3, 3}, 2.0);         // 3×3 matrix of 2.0
    auto r = npc::arange(0.0, 10.0, 2.0);    // [0, 2, 4, 6, 8]
    auto I = npc::eye(3);                    // 3×3 identity

    // ── Operator overloading ───────────────────────
    auto c = a + b;       // element-wise addition
    auto d = c * 3.0;     // scalar multiplication
    auto e = 2.0 / a;     // scalar division
    e += 1.0;             // compound assignment

    // ── Element access ────────────────────────────
    std::cout << a(0, 1) << std::endl;   // → 1.0
    a[0] = 99.0;                          // bracket indexing (1-D)
    double val = a.at<double>({1, 2});   // typed access

    // ── Math & statistics ──────────────────────────
    auto s = npc::sqrt(b);                // element-wise sqrt
    auto m = npc::mean(r);                // mean of all elements
    auto t = npc::sum(a, 0);              // sum along axis 0

    // ── Linear algebra ────────────────────────────
    auto [U, sv, Vt] = npc::svd(I);
    auto inv_a = npc::inv(a);

    // ── Printing ──────────────────────────────────
    c.print();                             // print array contents
    a.info();                              // print metadata

    return 0;
    // All arrays automatically freed via RAII — no manual cleanup needed
}
```

## Module Overview

| Module | Key Functions | Count |
|---|---|---|
| **Array Creation** | `zeros`, `ones`, `full`, `empty`, `arange`, `linspace`, `eye`, `identity`, `array<T>({...})` | 12 |
| **Arithmetic** | `operator+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=` (Array↔Array, Array↔scalar) | 14 |
| **Math (Unary)** | `abs`, `sqrt`, `exp`, `log`, `log10`, `log2`, `sin`, `cos`, `tan`, `floor`, `ceil`, `round`, `sign`, `reciprocal`, `cbrt`, `square`, etc. | 41 |
| **Math (Binary)** | `power`, `hypot`, `logaddexp`, `arctan2`, `max`, `min`, `mod`, `fmod`, `clip` | 12 |
| **Math (Extra)** | `gcd`, `lcm`, `ldexp`, `left_shift`, `right_shift`, `bincount` | 6 |
| **Trigonometry** | `sin`, `cos`, `tan`, `asin`, `acos`, `atan`, hyperbolic variants, angle conversion | 22 |
| **Comparison** | `equal`, `not_equal`, `greater`, `greater_equal`, `less`, `less_equal` | 6 |
| **Logic & Bitwise** | `logical_and`, `logical_or`, `logical_not`, `bitwise_and`, `bitwise_or`, `bitwise_xor` | 9 |
| **Floating Point** | `isclose`, `fabs`, `copysign`, `nextafter`, `spacing`, `frexp`, `modf` | 7 |
| **Statistics** | `sum`, `mean`, `min`, `max`, `stddev`, `var`, `median`, `percentile`, `cumsum`, `cov`, `corrcoef`, `argmax`, `ptp`, `all`, `any` | 31 |
| **NaN Statistics** | `nanmax`, `nanmin`, `nanmean`, `nanstd`, `nansum`, `nanprod`, `nan_to_num`, `isnan`, `isinf`, `isfinite` | 12 |
| **Linear Algebra** | `matmul`, `dot`, `inv`, `pinv`, `det`, `trace`, `svd`, `qr`, `lu`, `cholesky`, `eig`, `cross`, `outer`, `kron` | 19 |
| **FFT** | `fft`, `ifft`, `rfft`, `irfft`, `fft2`, `fftn`, `fftshift`, `fftfreq` | 18 |
| **Random** | `rand`, `randn`, `randint`, `uniform`, `normal`, `exponential`, `gamma`, `beta`, `poisson`, `binomial`, `choice`, `shuffle`, `permutation` | 48 |
| **Polynomials** | `poly`, `roots`, `polyfit`, `polyval`, `polyadd`, `polysub`, `polymul`, `polydiv`, `polyder`, `polyint` | 10 |
| **Shape & Manipulation** | `reshape`, `transpose`, `flatten`, `squeeze`, `stack`, `split`, `concatenate`, `tile`, `flip`, `roll`, `broadcast_to` | 45 |
| **Search & Set** | `unique`, `intersect1d`, `setdiff1d`, `searchsorted`, `digitize`, `argwhere` | 6 |
| **Interpolation** | `interp`, `diff`, `ediff1d`, `gradient` | 4 |
| **Sorting** | `sort`, `argsort`, `argpartition`, `partition`, `piecewise`, `unwrap` | 6 |
| **I/O** | `readtxt`, `savetxt`, `genfromtxt`, `load_npy`, `save_npy`, `savez` | 9 |
| **Window Functions** | `bartlett`, `blackman`, `hanning`, `hamming`, `kaiser` | 5 |
| **Histograms** | `histogram`, `histogram2d`, `histogramdd` | 3 |
| **Einsum** | `einsum`, `einsum_path` | 2 |
| **Datetime** | `DateTime64`, `now()`, `from_string()`, `to_string()`, `add_days()`, `is_busday` | 12 |
| **Mutation** | `place`, `put`, `putmask`, `fill_diagonal`, `insert`, `array_delete` | 6 |
| **Misc** | `copy`, `astype`, `logspace`, `geomspace`, `convolve`, `trim_zeros`, `frombuffer`, `ix_` | 11 |
| **Complex** | `conj_transpose`, `real`, `imag`, `angle` | 4 |
| **NaN Utils** | `nanargmax`, `nanargmin` | 2 |

## API Reference

### Core Types

```cpp
// ── npc::Array ────────────────────────────────────────────
namespace npc {
class Array {
public:
    Array();                                          // null array
    explicit Array(::Array* ptr, bool owned = true);  // from C pointer
    Array(const Array& other);                        // deep copy
    Array(Array&& other) noexcept;                    // move
    ~Array();

    // Accessors
    int ndim() const;
    int size() const;
    const int* shape() const;
    const int* strides() const;
    DType dtype() const;
    bool is_view() const;
    bool is_null() const;
    std::vector<int> shape_vec() const;

    // Element access
    template<typename T> T&  at(std::initializer_list<int> indices);
    template<typename T> const T& at(std::initializer_list<int> indices) const;
    template<typename T> T scalar() const;            // first element as T
    template<typename... Idx> double operator()(Idx... idx) const;  // a(i), a(i,j,k)
    double  operator[](int idx) const;                // 1-D bracket access
    double& operator[](int idx);                      // 1-D mutable access

    // Data pointers
    template<typename T> T* data();
    template<typename T> const T* data() const;

    // Iterators
    template<typename T> T* begin();
    template<typename T> T* end();
    template<typename T> const T* begin() const;
    template<typename T> const T* end() const;

    // I/O
    void print() const;
    void info() const;
    friend std::ostream& operator<<(std::ostream&, const Array&);

    // Raw access
    ::Array* get() const;
    ::Array* release() noexcept;
    void reset(::Array* ptr = nullptr);
};
}

// ── npc::DType ────────────────────────────────────────────
enum class DType : int { Bool, Int8, ..., Float64, ..., Unknown };

// ── npc::dtype_of<T> ──────────────────────────────────────
template<typename T> struct dtype_of;
```

### Array Creation

```cpp
// Factory functions (in namespace npc)
Array zeros(std::initializer_list<int> shape, DType dtype = Float64);
Array ones(std::initializer_list<int> shape, DType dtype = Float64);
Array full(std::initializer_list<int> shape, double value, DType dtype = Float64);
Array empty(std::initializer_list<int> shape, DType dtype = Float64);
Array arange(double start, double stop, double step = 1.0, DType dtype = Float64);
Array linspace(double start, double stop, int num, bool endpoint = true);
Array eye(int N, int M = -1, int k = 0, DType dtype = Float64);
Array identity(int N, DType dtype = Float64);
Array logspace(double start, double stop, int num, double base = 10.0, bool endpoint = true);
Array geomspace(double start, double stop, int num, bool endpoint = true);

// From initializer_list
template<typename T> Array array(std::initializer_list<T> data);                   // 1-D
template<typename T> Array array(std::initializer_list<std::initializer_list<T>>); // 2-D

// Copy & cast
Array copy(const Array& arr);
Array astype(const Array& arr, DType new_dtype);
```

### Arithmetic Operators

```cpp
// Array ↔ Array
Array operator+(const Array& a, const Array& b);
Array operator-(const Array& a, const Array& b);
Array operator*(const Array& a, const Array& b);
Array operator/(const Array& a, const Array& b);

// Unary negation
Array operator-(const Array& a);

// Array ↔ Scalar (double)
Array operator+(const Array& a, double s);    Array operator+(double s, const Array& a);
Array operator-(const Array& a, double s);    Array operator-(double s, const Array& a);
Array operator*(const Array& a, double s);    Array operator*(double s, const Array& a);
Array operator/(const Array& a, double s);    Array operator/(double s, const Array& a);

// Compound assignment
Array& operator+=(Array& a, const Array& b);  Array& operator+=(Array& a, double s);
Array& operator-=(Array& a, const Array& b);  Array& operator-=(Array& a, double s);
Array& operator*=(Array& a, const Array& b);  Array& operator*=(Array& a, double s);
Array& operator/=(Array& a, const Array& b);  Array& operator/=(Array& a, double s);
```

### Element-wise Math Functions

```cpp
// Unary (each returns Array)
Array abs(const Array& a);       Array sqrt(const Array& a);
Array exp(const Array& a);       Array log(const Array& a);
Array log10(const Array& a);     Array log2(const Array& a);
Array sin(const Array& a);       Array cos(const Array& a);
Array tan(const Array& a);       Array sinh(const Array& a);
Array cosh(const Array& a);      Array tanh(const Array& a);
Array arcsin(const Array& a);    Array arccos(const Array& a);
Array arctan(const Array& a);    Array arcsinh(const Array& a);
Array arccosh(const Array& a);   Array arctanh(const Array& a);
Array deg2rad(const Array& a);   Array rad2deg(const Array& a);
Array floor(const Array& a);     Array ceil(const Array& a);
Array rint(const Array& a);      Array trunc(const Array& a);
Array fix(const Array& a);       Array around(const Array& a, int decimals = 0);
Array sign(const Array& a);      Array sinc(const Array& a);
Array fabs(const Array& a);      Array absolute(const Array& a);   // abs alias (NumPy naming)
Array reciprocal(const Array& x);                                 // 1/x
Array positive(const Array& x);  Array negative(const Array& x);  // +x, -x
Array cbrt(const Array& x);      Array square(const Array& x);    // ∛x, x²
Array heaviside(const Array& x, double h = 0.5);
Array i0(const Array& a);
Array real(const Array& a);      Array imag(const Array& a);
Array angle(const Array& z, bool deg = false);
Array spacing(const Array& a);
int ndim(const Array& a);                                        // number of dimensions

// Binary (each takes two Arrays and returns Array)
Array power(const Array& a, const Array& b);
Array hypot(const Array& x, const Array& y);
Array logaddexp(const Array& a, const Array& b);
Array logaddexp2(const Array& a, const Array& b);
Array arctan2(const Array& y, const Array& x);
Array copysign(const Array& x1, const Array& x2);
Array nextafter(const Array& x1, const Array& x2);
Array mod(const Array& a, const Array& b);
Array fmod(const Array& x1, const Array& x2);
Array maximum(const Array& x1, const Array& x2);
Array minimum(const Array& x1, const Array& x2);
Array clip(const Array& arr, const Array& lo, const Array& hi);

// Extra math
Array gcd(const Array& x1, const Array& x2);
Array lcm(const Array& x1, const Array& x2);
Array ldexp(const Array& x1, const Array& x2);
Array left_shift(const Array& x1, const Array& x2);
Array right_shift(const Array& x1, const Array& x2);
Array bincount(const Array& a, int minlength = 0);
Array bincount(const Array& a, const Array& weights, int minlength);
```

### Comparison & Logic

```cpp
// Element-wise comparison (each returns bool Array)
Array equal(const Array& a, const Array& b);
Array not_equal(const Array& a, const Array& b);
Array greater(const Array& a, const Array& b);
Array greater_equal(const Array& a, const Array& b);
Array less(const Array& a, const Array& b);
Array less_equal(const Array& a, const Array& b);
Array isclose(const Array& a, const Array& b, double rtol = 1e-5, double atol = 1e-8, bool equal_nan = false);

// Logical operations
Array logical_and(const Array& a, const Array& b);
Array logical_or(const Array& a, const Array& b);
Array logical_xor(const Array& a, const Array& b);
Array logical_not(const Array& a);

// Bitwise operations
Array bitwise_and(const Array& a, const Array& b);
Array bitwise_or(const Array& a, const Array& b);
Array bitwise_xor(const Array& a, const Array& b);
Array bitwise_not(const Array& a);
Array invert(const Array& a);
```

### Statistics

```cpp
// Basic aggregations (axis=-1 = all elements, keepdims=false)
Array sum(const Array& a, int axis = -1, bool keepdims = false);
Array mean(const Array& a, int axis = -1, bool keepdims = false);
Array min(const Array& a, int axis = -1, bool keepdims = false);
Array max(const Array& a, int axis = -1, bool keepdims = false);
Array prod(const Array& a, int axis = -1, bool keepdims = false);

// Variance & standard deviation
Array stddev(const Array& a, int axis = -1, bool keepdims = false);
Array var(const Array& a, int axis = -1, int ddof = 0, bool keepdims = false);

// Cumulative
Array cumsum(const Array& a, int axis = -1);
Array cumprod(const Array& a, int axis = -1);

// Order statistics
Array median(const Array& a, int axis = -1, bool keepdims = false);
Array percentile(const Array& a, const Array& q, int axis = -1);
Array quantile(const Array& a, const Array& q, int axis = -1);

// Index operations
Array argmax(const Array& a, int axis = -1, bool keepdims = false);
Array argmin(const Array& a, int axis = -1, bool keepdims = false);
Array count_nonzero(const Array& a, int axis = -1);

// Other
Array ptp(const Array& a, int axis = -1, bool keepdims = false);
Array all(const Array& a, int axis = -1, bool keepdims = false);
Array any(const Array& a, int axis = -1, bool keepdims = false);
Array where(const Array& condition, const Array& x, const Array& y);

// Weighted & correlation
Array average(const Array& a, int axis = -1, bool keepdims = false);
Array average(const Array& a, const Array& weights, int axis = -1, bool keepdims = false);
Array cov(const Array& m, bool rowvar = true, int ddof = 0, DType dtype = Float64);
Array corrcoef(const Array& m, bool rowvar = true);
Array correlate(const Array& a, const Array& v, const char* mode = "valid");

// Integration
Array trapz(const Array& y, double dx = 1.0, int axis = -1);
Array trapz(const Array& y, const Array& x, double dx = 1.0, int axis = -1);

// NaN-aware aggregations
Array nanmax(const Array& a, int axis = -1);
Array nanmin(const Array& a, int axis = -1);
Array nanmean(const Array& a, int axis = -1);
Array nanmedian(const Array& a, int axis = -1);
Array nanpercentile(const Array& a, const Array& q, int axis = -1);
Array nanquantile(const Array& a, const Array& q, int axis = -1);
Array nancumsum(const Array& a, int axis = -1);
Array nancumprod(const Array& a, int axis = -1);
Array nansum(const Array& a, int axis = -1);
Array nanprod(const Array& a, int axis = -1);
Array nanstd(const Array& a, int axis = -1, bool keepdims = false);
Array nanvar(const Array& a, int axis = -1, int ddof = 0, bool keepdims = false);
```

### Linear Algebra

```cpp
// ── Result structs ─────────────────────────────────────────
struct SVDResult { Array U, s, Vt; };
struct QRResult  { Array Q, R; };
struct LUResult  { Array P, L, U; };
struct EigResult { Array eigenvalues, eigenvectors; };

// Matrix products
Array dot(const Array& a, const Array& b);
Array matmul(const Array& a, const Array& b);
Array vdot(const Array& a, const Array& b);
Array inner(const Array& a, const Array& b);
Array outer(const Array& a, const Array& b);
Array cross(const Array& a, const Array& b);
Array kron(const Array& a, const Array& b);
Array tensordot(const Array& a, const Array& b, const Array& axes_a, const Array& axes_b);
Array tensordot(const Array& a, const Array& b, int axes);

// Matrix properties
Array inv(const Array& a);
Array pinv(const Array& a, double rcond = 1e-15);
Array det(const Array& a);
Array trace(const Array& a);

// Decompositions
Array cholesky(const Array& a);
SVDResult svd(const Array& a, bool full_matrices = true);
QRResult qr(const Array& a);
LUResult lu(const Array& a);
EigResult eig(const Array& a);

// In-place
void swap_rows(Array& arr, int row1, int row2);
void swap_cols(Array& arr, int col1, int col2);
```

### Shape Manipulation

```cpp
// Reshaping
Array reshape(const Array& a, std::initializer_list<int> new_shape);
Array resize(const Array& a, std::initializer_list<int> new_shape);
Array transpose(const Array& a);
Array flatten(const Array& a);
Array ravel(const Array& a, const char* order = "C");
Array squeeze(const Array& a, int axis = -1);
Array expand_dims(const Array& a, int axis);

// Stacking
Array stack(const std::vector<Array>& arrays, int axis = 0);
Array hstack(const std::vector<Array>& arrays);
Array vstack(const std::vector<Array>& arrays);
Array dstack(const std::vector<Array>& arrays);
Array column_stack(const std::vector<Array>& arrays);
Array concatenate(const Array& a, const Array& b, int axis = 0);
Array append(const Array& a, const Array& values, int axis);

// Splitting (returns std::vector<Array>)
std::vector<Array> split(const Array& a, const Array& indices, int axis = 0);
std::vector<Array> hsplit(const Array& a, const Array& indices);
std::vector<Array> vsplit(const Array& a, const Array& indices);
std::vector<Array> dsplit(const Array& a, const Array& indices);

// Repeating & tiling
Array tile(const Array& a, std::initializer_list<int> reps);
Array repeat(const Array& a, const Array& repeats, int axis);

// Flipping & rolling
Array flip(const Array& a, int axis = -1);
Array fliplr(const Array& a);
Array flipud(const Array& a);
Array roll(const Array& a, int shift, int axis = -1);
Array rot90(const Array& m, int k = 1, int axis1 = 0, int axis2 = 1);

// Axis operations
Array moveaxis(const Array& a, int source, int destination);
Array rollaxis(const Array& a, int axis, int start);
Array swapaxes(const Array& a, int axis1, int axis2);

// Broadcasting
Array broadcast_to(const Array& a, std::initializer_list<int> shape);
std::vector<Array> broadcast_arrays(const std::vector<Array>& arrays);

// Diagonal
Array diag(const Array& v, int k = 0);
Array diagflat(const Array& a, int k = 0);
Array diagonal(const Array& a, int offset = 0, int axis1 = 0, int axis2 = 1);

// Other
Array atleast_1d(const Array& a);
Array atleast_2d(const Array& a);
Array atleast_3d(const Array& a);
Array take(const Array& a, const Array& indices, int axis);
Array compress(const Array& condition, const Array& a, int axis);
Array choose(const Array& a, const std::vector<Array>& choices, int mode);
Array pad(const Array& a, const Array& pad_width, const char* mode = "constant", double cv = 0.0);
Array flatnonzero(const Array& a);
Array block(const std::vector<Array>& blocks, int rows, int cols);
Array bmat(const std::vector<Array>& blocks, int rows, int cols);
std::vector<Array> meshgrid(const std::vector<Array>& arrays, const char* indexing = "xy");
```

### FFT

```cpp
Array fft(const Array& a);
Array ifft(const Array& a);
Array rfft(const Array& a);
Array rfft2(const Array& a);
Array rfftn(const Array& a);
Array hfft(const Array& a, int n);
Array ihfft(const Array& a, int n);
Array irfft(const Array& a, int n = -1);
Array irfft2(const Array& a, std::array<int, 2> s = {});
Array irfftn(const Array& a, std::initializer_list<int> s = {});
Array fft2(const Array& a);
Array ifft2(const Array& a);
Array fftn(const Array& a);
Array ifftn(const Array& a);
Array fftshift(const Array& a);
Array ifftshift(const Array& a);
Array fftfreq(int n, double d = 1.0);
Array rfftfreq(int n, double d = 1.0);
```

### Random Number Generation

```cpp
// RNG control
void random_seed(uint64_t seed);
uint64_t random_uint64();
double random_double();

// Uniform (shape = {} for scalar output)
Array random_sample(std::initializer_list<int> shape = {});
Array rand(std::initializer_list<int> shape = {});
Array randint(int low, int high, std::initializer_list<int> shape = {}, DType dtype = Int64);
Array uniform(double low, double high, std::initializer_list<int> shape = {});

// Normal
double standard_normal_val();
Array standard_normal(std::initializer_list<int> shape = {});
Array randn(std::initializer_list<int> shape = {});
Array normal(double loc, double scale, std::initializer_list<int> shape = {});

// Exponential
double standard_exponential_val();
Array standard_exponential(std::initializer_list<int> shape = {});
Array exponential(double scale, std::initializer_list<int> shape = {});

// Gamma family
double standard_gamma_val(double shape);
Array standard_gamma(double shape, std::initializer_list<int> out_shape = {});
Array gamma(double shape, double scale, std::initializer_list<int> out_shape = {});
Array chisquare(double df, std::initializer_list<int> shape = {});
Array beta(double a, double b, std::initializer_list<int> shape = {});
Array standard_t(double df, std::initializer_list<int> shape = {});
Array f(double dfn, double dfd, std::initializer_list<int> shape = {});

// Continuous distributions
Array laplace(double loc, double scale, std::initializer_list<int> shape = {});
Array logistic(double loc, double scale, std::initializer_list<int> shape = {});
Array lognormal(double mean, double sigma, std::initializer_list<int> shape = {});
Array pareto(double a, std::initializer_list<int> shape = {});
Array power(double a, std::initializer_list<int> shape = {});
Array rayleigh(double scale, std::initializer_list<int> shape = {});
Array weibull(double a, std::initializer_list<int> shape = {});
Array standard_cauchy(std::initializer_list<int> shape = {});
Array gumbel(double loc, double scale, std::initializer_list<int> shape = {});
Array triangular(double left, double mode, double right, std::initializer_list<int> shape = {});
Array vonmises(double mu, double kappa, std::initializer_list<int> shape = {});
Array wald(double mean, double scale, std::initializer_list<int> shape = {});

// Discrete distributions
Array geometric(double p, std::initializer_list<int> shape = {});
Array poisson(double lam, std::initializer_list<int> shape = {});
Array binomial(int n, double p, std::initializer_list<int> shape = {});
Array negative_binomial(int n, double p, std::initializer_list<int> shape = {});
Array hypergeometric(int ngood, int nbad, int nsample, std::initializer_list<int> shape = {});
Array zipf(double a, std::initializer_list<int> shape = {});
Array logseries(double p, std::initializer_list<int> shape = {});

// Multivariate & utilities
Array permutation(const Array& x);
void shuffle(Array& x);
Array choice(const Array& a, int size, bool replace = true, const Array& p = Array());
Array multinomial(int n, const Array& pvals, std::initializer_list<int> shape = {});
Array multivariate_normal(const Array& mean, const Array& cov, std::initializer_list<int> shape = {});
Array dirichlet(const Array& alpha, std::initializer_list<int> shape = {});
Array random_bytes(int n);
```

### I/O

```cpp
// Text
Array readtxt(const char* filename, const char* delimiter = " ", ...);
Array genfromtxt(const char* filename, const char* delimiter = ",", int skip_header = 0);
int savetxt(const char* filename, const Array& arr, const char* fmt = "%g", ...);

// Binary NPY
int save_npy(const char* filename, const Array& arr);
Array load_npy(const char* filename);

// NPZ
int savez(const char* filename, const std::vector<Array>& arrays);

// std::string overloads
int savetxt(const std::string& filename, const Array& arr, ...);
int save_npy(const std::string& filename, const Array& arr);
Array load_npy(const std::string& filename);
Array readtxt(const std::string& filename, const char* delimiter = " ");
```

### Polynomials

```cpp
Array poly(const Array& roots);
Array roots(const Array& p);
Array polyadd(const Array& a, const Array& b);
Array polysub(const Array& a, const Array& b);
Array polymul(const Array& a, const Array& b);
std::pair<Array, Array> polydiv(const Array& u, const Array& v);  // {quotient, remainder}
Array polyder(const Array& p, int m = 1);
Array polyint(const Array& p, int m = 1);
Array polyval(const Array& p, const Array& x);
Array polyfit(const Array& x, const Array& y, int deg);
```

### Search & Set Operations

```cpp
struct UniqueResult { Array values, indices, inverse, counts; };

UniqueResult unique(const Array& arr, bool return_index = false,
                    bool return_inverse = false, bool return_counts = false);
Array intersect1d(const Array& a, const Array& b, bool assume_unique = false);
Array setdiff1d(const Array& a, const Array& b);
Array searchsorted(const Array& a, const Array& v, const char* side = "left");
Array digitize(const Array& arr, const Array& bins, bool right = false);
Array argwhere(const Array& arr);
```

### Window Functions

```cpp
Array bartlett(int M);
Array blackman(int M);
Array kaiser(int M, double beta);
Array hanning(int M);
Array hamming(int M);
```

### Histograms

```cpp
struct HistogramResult   { Array hist, bin_edges; };
struct Histogram2DResult { Array H, x_edges, y_edges; };
struct HistogramDDResult { Array hist; std::vector<Array> edges; };

HistogramResult   histogram(const Array& x, int bins, bool density = false);
Histogram2DResult histogram2d(const Array& x, const Array& y, int bins_x, int bins_y,
                              const Array& rx = Array(), const Array& ry = Array(),
                              bool density = false);
HistogramDDResult histogramdd(const Array& sample, const Array& bins,
                              const Array& range = Array(), bool density = false);
```

### Interpolation & Difference

```cpp
Array interp(const Array& arr, const Array& xp, const Array& fp,
             const Array& left = Array(), const Array& right = Array());
Array diff(const Array& arr, int n = 1, int axis = -1);
Array ediff1d(const Array& y, const Array& to_begin = Array(), const Array& to_end = Array());
std::vector<Array> gradient(const Array& arr, const Array& spacing = Array());
```

### Sorting & Special

```cpp
Array sort(const Array& a);
Array argsort(const Array& arr, int axis = -1, const char* kind = "quicksort");
Array argpartition(const Array& arr, int kth, int axis = -1);
Array partition(const Array& arr, int kth, int axis = -1);
Array piecewise(const Array& x, const std::vector<Array>& condlist,
                const std::vector<double(*)(double)>& funclist);
Array unwrap(const Array& arr, double discont = M_PI, int axis = -1);
```

### In-place Mutation

```cpp
int place(Array& arr, const Array& mask, const Array& vals);
int put(Array& arr, const Array& indices, const Array& values, const char* mode = "raise");
void putmask(Array& arr, const Array& mask, const Array& values);
void fill_diagonal(Array& arr, double val);
```

### Miscellaneous

```cpp
Array convolve(const Array& a, const Array& v);
Array extract(const Array& condition, const Array& arr);
Array trim_zeros(const Array& arr, const char* trim = "fb");
std::pair<Array, Array> frexp(const Array& arr);     // {mantissa, exponent}
std::pair<Array, Array> modf(const Array& arr);      // {fractional, integral}
Array frombuffer(const void* buffer, DType dtype, int count, size_t offset = 0);
Array fromfunction(double(*func)(int*, int), std::initializer_list<int> shape, DType dtype = Float64);
Array insert(const Array& arr, const Array& obj, const Array& values, int axis);
Array array_delete(const Array& arr, const Array& obj, int axis);
std::vector<Array> ix_(const std::vector<Array>& arrays);
Array linspace_float(float start, float stop, int num);
```

### Datetime64

```cpp
enum class DateTimeUnit { NS, US, MS, S, M, H, D, W, Month, Year };

class DateTime64 {
public:
    static DateTime64 now();
    static DateTime64 from_string(const char* str, DateTimeUnit unit = DateTimeUnit::NS);
    std::string to_string() const;
    DateTime64 add_days(int64_t days) const;
    DateTime64 sub_days(int64_t days) const;
    int64_t to_ns() const;
    // Full comparison: ==, !=, <, <=, >, >=
};

Array is_busday(const Array& dates, const char* weekmask = "1111100",
                const Array& holidays = Array());
```

### Einsum

```cpp
Array einsum(const char* subscripts, const std::vector<Array>& operands,
             const Array& out = Array(), const char* optimize = "greedy");
std::pair<std::vector<std::vector<int>>, std::string>
    einsum_path(const char* subscripts, const std::vector<Array>& operands,
                const char* optimize = "greedy");
```

## Architecture

### RAII Ownership

Every `npc::Array` owns its underlying `::Array*` via `std::unique_ptr<::Array, ArrayDeleter>`. When the C++ object goes out of scope, `free_array()` is called automatically. There is no manual memory management.

### Copy vs Move

- **Copy** (`Array(const Array&)`): deep-copies the underlying C array via `copy_array()`. The copy is independent.
- **Move** (`Array(Array&&)`): transfers ownership. The source becomes a null array.
- **Copy assignment** (`operator=`): deep-copies, leaving the source unchanged.
- **Move assignment**: transfers ownership.

### Operator Overloading

All arithmetic operators (`+`, `-`, `*`, `/`) return new `npc::Array` objects that own their data. Compound assignment (`+=`, `-=`, etc.) modifies the left-hand side in place.

Comparison operators (`equal`, `greater`, etc.) return boolean arrays (element-wise results). C++ `operator==` is intentionally **not** overloaded — NumPy-style element-wise comparison is done via named functions.

### C++ vs libnpc Naming

| libnpc C | libnpc++ C++ | Reason |
|---|---|---|
| `free_array(ptr)` | RAII destructor | Automatic cleanup |
| `get_item(arr, idx)` | `a.at<T>({i,j})`, `a(i,j)`, `a[3]` | Operator overloading |
| `std(arr, axis, keepdims)` | `stddev(arr, axis, keepdims)` | `std` is a C++ reserved namespace |
| `delete(arr, obj, axis)` | `array_delete(arr, obj, axis)` | `delete` is a C++ keyword |
| `DataType` enum | `DType` scoped enum | C++ enum class |
| `int keepdims` (0/1) | `bool keepdims` | Idiomatic C++ |
| `int* shape, int ndim` | `std::initializer_list<int>` | Modern C++ syntax |

## License

MIT — see LICENSE for details.

libnpc (in `third_party/libnpc/`) is also MIT-licensed by xiayang558.
