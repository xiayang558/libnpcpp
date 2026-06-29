# libnpc++

[English](README.md) | [中文](README_zh.md) | [📘 API 文档](https://xiayang558.github.io/libnpcpp)

[![Version](https://img.shields.io/badge/version-1.0.0-blue)](https://github.com/xiayang558/libnpcpp)
[![C++17](https://img.shields.io/badge/C++-17-blue)](https://en.cppreference.com/w/cpp/17)
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE)

[libnpc](https://github.com/xiayang558/libnpc) 的 C++17 RAII 封装层。libnpc 是一个纯 C 数值计算库，重新实现了 NumPy 核心功能（300+ 函数，21 个模块），**零 Python 依赖**。

libnpc++ 在 libnpc 的 ~335 个 C 函数之上提供现代 C++ 惯用法：

- **RAII** — 自动内存管理，无需手动调用 `free_array()`
- **运算符重载** — `a + b`, `a * 2.0`, `-a`, `2.0 / a`, `a += b`
- **便捷索引** — `a(i, j)`, `a[3]`, `a.at<T>({i, j})`
- **流输出** — `std::cout << a` 打印数组元数据
- **移动语义** — 高效的所有权转移
- **类型安全** — 编译期 `dtype_of<T>` 映射，带类型的 `begin<T>()`/`end<T>()` 迭代器
- **NumPy 兼容默认值** — `axis=-1` 表示所有元素，`keepdims=false`

## 构建与安装

### 前置依赖

- **C++17 编译器** (GCC 8+, Clang 7+, Apple Clang 12+)
- **CMake 3.16+**
- **libzip** (用于 NPZ 文件 I/O):
  - macOS: `brew install libzip`
  - Linux: `apt install libzip-dev`

### 构建

```bash
git clone --recurse-submodules https://github.com/xiayang558/libnpcpp.git
cd libnpcpp
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### 运行测试

```bash
cd build
ctest --output-on-failure
```

### 在 CMake 项目中使用

```cmake
add_subdirectory(path/to/libnpcpp)
target_link_libraries(my_app PRIVATE libnpc::npcpp)
```

## 数据类型

libnpc++ 将 libnpc 的 C 语言 `DataType` 枚举映射为有作用域的 C++ `DType` 枚举。

| DType | C++ 类型 | 大小 | 说明 |
|---|---|---|---|
| `Bool` | `bool` | 1 | 布尔型 |
| `Int8` | `int8_t` | 1 | 有符号 8 位整数 |
| `Int16` | `int16_t` | 2 | 有符号 16 位整数 |
| `Int32` | `int32_t` | 4 | 有符号 32 位整数 |
| `Int64` | `int64_t` | 8 | 有符号 64 位整数 |
| `UInt8` | `uint8_t` | 1 | 无符号 8 位整数 |
| `UInt16` | `uint16_t` | 2 | 无符号 16 位整数 |
| `UInt32` | `uint32_t` | 4 | 无符号 32 位整数 |
| `UInt64` | `uint64_t` | 8 | 无符号 64 位整数 |
| `Float32` | `float` | 4 | 32 位浮点 |
| `Float64` | `double` | 8 | 64 位浮点 **(默认)** |
| `Float128` | `long double` | 16 | 128 位浮点 |
| `Complex64` | `std::complex<float>` | 8 | 64 位复数 |
| `Complex128` | `std::complex<double>` | 16 | 128 位复数 |
| `Complex256` | `std::complex<long double>` | 32 | 256 位复数 |
| `DateTime64` | `DateTime64` | 8 | 日期时间 |
| `String` | — | 不定 | 字符串 |
| `Unknown` | — | — | 未知类型 |

### 编译期类型映射

```cpp
npc::dtype_of<double>::value          // → npc::DType::Float64
npc::dtype_of<int32_t>::value         // → npc::DType::Int32
npc::dtype_of<std::complex<float>>::value  // → npc::DType::Complex64
```

## 快速开始

```cpp
#include <npc/npc.hpp>

int main() {
    // ── 数组创建 ────────────────────────────────
    auto a = npc::ones({3, 3});              // 3×3 全 1 矩阵
    auto b = npc::full({3, 3}, 2.0);         // 3×3 全 2.0 矩阵
    auto r = npc::arange(0.0, 10.0, 2.0);    // [0, 2, 4, 6, 8]
    auto I = npc::eye(3);                    // 3×3 单位矩阵

    // ── 运算符重载 ─────────────────────────────
    auto c = a + b;       // 逐元素加法
    auto d = c * 3.0;     // 标量乘法
    auto e = 2.0 / a;     // 标量除法
    e += 1.0;             // 复合赋值

    // ── 元素访问 ──────────────────────────────
    std::cout << a(0, 1) << std::endl;   // → 1.0
    a[0] = 99.0;                          // 方括号索引 (1-D)
    double val = a.at<double>({1, 2});   // 类型化访问

    // ── 数学与统计 ────────────────────────────
    auto s = npc::sqrt(b);                // 逐元素开方
    auto m = npc::mean(r);                // 所有元素的均值
    auto t = npc::sum(a, 0);              // 沿 axis 0 求和

    // ── 线性代数 ──────────────────────────────
    auto [U, sv, Vt] = npc::svd(I);
    auto inv_a = npc::inv(a);

    // ── 打印 ──────────────────────────────────
    c.print();                             // 打印数组内容
    a.info();                              // 打印元数据

    return 0;
    // 所有数组通过 RAII 自动释放 — 无需手动清理
}
```

## 模块概览

| 模块 | 关键函数 | 数量 |
|---|---|---|
| **数组创建** | `zeros`, `ones`, `full`, `empty`, `arange`, `linspace`, `eye`, `identity`, `array<T>({...})` | 12 |
| **算术** | `operator+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`（数组↔数组，数组↔标量） | 14 |
| **数学 (一元)** | `abs`, `sqrt`, `exp`, `log`, `log10`, `log2`, `sin`, `cos`, `tan`, `floor`, `ceil`, `round`, `sign`, `reciprocal`, `cbrt`, `square` 等 | 41 |
| **数学 (二元)** | `power`, `hypot`, `logaddexp`, `arctan2`, `max`, `min`, `mod`, `fmod`, `clip` | 12 |
| **数学 (扩展)** | `gcd`, `lcm`, `ldexp`, `left_shift`, `right_shift`, `bincount` | 6 |
| **三角** | `sin`, `cos`, `tan`, `asin`, `acos`, `atan`, 双曲变体, 角度转换 | 22 |
| **比较** | `equal`, `not_equal`, `greater`, `greater_equal`, `less`, `less_equal` | 6 |
| **逻辑与位运算** | `logical_and`, `logical_or`, `logical_not`, `bitwise_and`, `bitwise_or`, `bitwise_xor` | 9 |
| **浮点** | `isclose`, `fabs`, `copysign`, `nextafter`, `spacing`, `frexp`, `modf` | 7 |
| **统计** | `sum`, `mean`, `min`, `max`, `stddev`, `var`, `median`, `percentile`, `cumsum`, `cov`, `corrcoef`, `argmax`, `ptp`, `all`, `any` | 31 |
| **NaN 统计** | `nanmax`, `nanmin`, `nanmean`, `nanstd`, `nansum`, `nanprod`, `nan_to_num`, `isnan`, `isinf`, `isfinite` | 12 |
| **线性代数** | `matmul`, `dot`, `inv`, `pinv`, `det`, `trace`, `svd`, `qr`, `lu`, `cholesky`, `eig`, `cross`, `outer`, `kron` | 19 |
| **FFT** | `fft`, `ifft`, `rfft`, `irfft`, `fft2`, `fftn`, `fftshift`, `fftfreq` | 18 |
| **随机数** | `rand`, `randn`, `randint`, `uniform`, `normal`, `exponential`, `gamma`, `beta`, `poisson`, `binomial`, `choice`, `shuffle` | 48 |
| **多项式** | `poly`, `roots`, `polyfit`, `polyval`, `polyadd`, `polysub`, `polymul`, `polydiv`, `polyder`, `polyint` | 10 |
| **形状操作** | `reshape`, `transpose`, `flatten`, `squeeze`, `stack`, `split`, `concatenate`, `tile`, `flip`, `roll`, `broadcast_to` | 45 |
| **搜索与集合** | `unique`, `intersect1d`, `setdiff1d`, `searchsorted`, `digitize`, `argwhere` | 6 |
| **插值** | `interp`, `diff`, `ediff1d`, `gradient` | 4 |
| **排序** | `sort`, `argsort`, `argpartition`, `partition`, `piecewise`, `unwrap` | 6 |
| **I/O** | `readtxt`, `savetxt`, `genfromtxt`, `load_npy`, `save_npy`, `savez` | 9 |
| **窗函数** | `bartlett`, `blackman`, `hanning`, `hamming`, `kaiser` | 5 |
| **直方图** | `histogram`, `histogram2d`, `histogramdd` | 3 |
| **Einsum** | `einsum`, `einsum_path` | 2 |
| **日期时间** | `DateTime64`, `now()`, `from_string()`, `to_string()`, `add_days()`, `is_busday` | 12 |
| **原地修改** | `place`, `put`, `putmask`, `fill_diagonal`, `insert`, `array_delete` | 6 |
| **杂项** | `copy`, `astype`, `logspace`, `geomspace`, `convolve`, `trim_zeros`, `frombuffer`, `ix_` | 11 |

## API 参考

### 核心类型

```cpp
// ── npc::Array ────────────────────────────────────────────
namespace npc {
class Array {
public:
    Array();                                          // 空数组
    explicit Array(::Array* ptr, bool owned = true);  // 从 C 指针接管
    Array(const Array& other);                        // 深拷贝
    Array(Array&& other) noexcept;                    // 移动
    ~Array();

    // 访问器
    int ndim() const;                                 // 维度数
    int size() const;                                 // 元素总数
    const int* shape() const;                         // 形状数组
    const int* strides() const;                       // 步长数组
    DType dtype() const;                              // 数据类型
    bool is_view() const;                             // 是否为视图
    bool is_null() const;                             // 是否为空
    std::vector<int> shape_vec() const;               // 形状转为 vector

    // 元素访问
    template<typename T> T&  at(std::initializer_list<int> indices);
    template<typename T> const T& at(std::initializer_list<int> indices) const;
    template<typename T> T scalar() const;             // 首元素转为 T
    template<typename... Idx> double operator()(Idx... idx) const;  // a(i), a(i,j,k)
    double  operator[](int idx) const;                 // 1-D 方括号访问
    double& operator[](int idx);                       // 1-D 可变方括号

    // 数据指针
    template<typename T> T* data();
    template<typename T> const T* data() const;

    // 预定义内建方法（共 74 个，也可通过 npc:: 自由函数调用）
    Array abs() const;   Array sqrt() const;  Array exp() const;
    Array log() const;   Array sin() const;   Array cos() const;
    Array floor() const; Array ceil() const;  Array round(int d=0) const;
    Array sum(int a=-1, bool kd=false) const;    // 求和
    Array mean(int a=-1, bool kd=false) const;   // 均值
    Array min(int a=-1, bool kd=false) const;    // 最小值
    Array max(int a=-1, bool kd=false) const;    // 最大值
    Array stddev(int a=-1, bool kd=false) const; // 标准差
    Array var(int a=-1, int ddof=0, bool kd=false) const;
    Array median(int a=-1, bool kd=false) const; // 中位数
    Array argmax(int a=-1, bool kd=false) const; // 最大值索引
    Array argmin(int a=-1, bool kd=false) const; // 最小值索引
    Array all(int a=-1, bool kd=false) const;    Array any(...) const;
    Array cumsum(int a=-1) const;               Array cumprod(...) const;
    Array reshape(il<int> shape) const;  Array transpose() const;
    Array T() const;                      Array flatten() const;
    Array ravel(const char* o="C") const; Array squeeze(int a=-1) const;
    Array expand_dims(int a) const;      Array flip(int a=-1) const;
    Array roll(int s, int a=-1) const;   Array broadcast_to(il<int> s) const;
    Array tile(il<int> reps) const;      Array pad(...) const;
    Array swapaxes(int a1, int a2) const;
    Array copy() const;                  Array astype(DType d) const;
    Array dot(const Array& o) const;     Array matmul(...) const;
    Array trace() const;                 Array det() const;
    Array inv() const;                   Array conj_transpose() const;
    Array equal(const Array& o) const;   Array not_equal(...) const;
    Array greater(const Array& o) const; Array less(...) const;
    Array isnan() const;                 Array isinf() const;
    Array sort() const;                  Array argsort(...) const;
    Array diag(int k=0) const;           Array clip(const Array& lo, const Array& hi) const;
    Array clip(double lo, double hi) const;
    void fill_diagonal(double val);      Array trim_zeros(const char* t="fb") const;

    // 迭代器
    template<typename T> T* begin();
    template<typename T> T* end();
    template<typename T> const T* begin() const;
    template<typename T> const T* end() const;

    // I/O
    void print() const;
    void info() const;
    friend std::ostream& operator<<(std::ostream&, const Array&);

    // 原始 C 指针（逃生舱口）
    ::Array* get() const;
    ::Array* release() noexcept;
    void reset(::Array* ptr = nullptr);
};
}

// ── npc::DType ────────────────────────────────────────────
enum class DType : int { Bool, Int8, ..., Float64, ..., Unknown };

// ── npc::dtype_of<T> ──────────────────────────────────────
template<typename T> struct dtype_of;  // 编译期类型→DType 映射
```

### 数组创建

```cpp
// 工厂函数（均在 npc 命名空间下）
Array zeros(std::initializer_list<int> shape, DType dtype = Float64);     // 全零数组
Array ones(std::initializer_list<int> shape, DType dtype = Float64);      // 全一数组
Array full(std::initializer_list<int> shape, double value, DType dtype = Float64); // 常量填充
Array empty(std::initializer_list<int> shape, DType dtype = Float64);     // 未初始化数组
Array arange(double start, double stop, double step = 1.0, DType dtype = Float64); // 等间距区间
Array linspace(double start, double stop, int num, bool endpoint = true); // 等间距线性
Array eye(int N, int M = -1, int k = 0, DType dtype = Float64);           // 单位矩阵
Array identity(int N, DType dtype = Float64);                              // N×N 单位矩阵
Array logspace(double start, double stop, int num, double base = 10.0, bool endpoint = true);
Array geomspace(double start, double stop, int num, bool endpoint = true);

// 从初始化列表构建（1-D 和 2-D）
template<typename T> Array array(std::initializer_list<T> data);
template<typename T> Array array(std::initializer_list<std::initializer_list<T>>);

// 拷贝与类型转换
Array copy(const Array& arr);
Array astype(const Array& arr, DType new_dtype);
```

### 算术运算符

```cpp
// 数组 ↔ 数组
Array operator+(const Array& a, const Array& b);   // 逐元素加法
Array operator-(const Array& a, const Array& b);   // 逐元素减法
Array operator*(const Array& a, const Array& b);   // 逐元素乘法
Array operator/(const Array& a, const Array& b);   // 逐元素除法

// 一元取负
Array operator-(const Array& a);

// 数组 ↔ 标量 (double)
Array operator+(const Array& a, double s);    Array operator+(double s, const Array& a);
Array operator-(const Array& a, double s);    Array operator-(double s, const Array& a);
Array operator*(const Array& a, double s);    Array operator*(double s, const Array& a);
Array operator/(const Array& a, double s);    Array operator/(double s, const Array& a);

// 复合赋值
Array& operator+=(Array& a, const Array& b);  Array& operator+=(Array& a, double s);
Array& operator-=(Array& a, const Array& b);  Array& operator-=(Array& a, double s);
Array& operator*=(Array& a, const Array& b);  Array& operator*=(Array& a, double s);
Array& operator/=(Array& a, const Array& b);  Array& operator/=(Array& a, double s);
```

### 逐元素数学函数

```cpp
// 一元函数（返回 Array）
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
Array fabs(const Array& a);      Array absolute(const Array& a);
Array reciprocal(const Array& x); Array positive(const Array& x);
Array negative(const Array& x);  Array cbrt(const Array& x);
Array square(const Array& x);    Array heaviside(const Array& x, double h = 0.5);
Array i0(const Array& a);
Array real(const Array& a);      Array imag(const Array& a);
Array angle(const Array& z, bool deg = false);
Array spacing(const Array& a);
int ndim(const Array& a);

// 二元函数（两个 Array 参数，返回 Array）
Array power(const Array& a, const Array& b);      // 逐元素幂运算 a^b
Array hypot(const Array& x, const Array& y);       // sqrt(x² + y²)
Array logaddexp(const Array& a, const Array& b);   // log(exp(a) + exp(b))
Array logaddexp2(const Array& a, const Array& b);  // log₂(2ᵃ + 2ᵇ)
Array arctan2(const Array& y, const Array& x);     // 双参数反正切
Array copysign(const Array& x1, const Array& x2);  // 复制符号
Array nextafter(const Array& x1, const Array& x2); // 下一个可表示浮点数
Array mod(const Array& a, const Array& b);         // 逐元素取模
Array fmod(const Array& x1, const Array& x2);       // 浮点取模
Array maximum(const Array& x1, const Array& x2);   // 逐元素最大值
Array minimum(const Array& x1, const Array& x2);   // 逐元素最小值
Array clip(const Array& arr, const Array& lo, const Array& hi); // 裁剪

// 扩展数学
Array gcd(const Array& x1, const Array& x2);       // 最大公约数
Array lcm(const Array& x1, const Array& x2);       // 最小公倍数
Array ldexp(const Array& x1, const Array& x2);     // x1 * 2ˣ²
Array left_shift(const Array& x1, const Array& x2); // 左移
Array right_shift(const Array& x1, const Array& x2);// 右移
Array bincount(const Array& a, int minlength = 0);
Array bincount(const Array& a, const Array& weights, int minlength);
```

### 比较与逻辑

```cpp
// 逐元素比较（返回 BOOL 数组）
Array equal(const Array& a, const Array& b);
Array not_equal(const Array& a, const Array& b);
Array greater(const Array& a, const Array& b);
Array greater_equal(const Array& a, const Array& b);
Array less(const Array& a, const Array& b);
Array less_equal(const Array& a, const Array& b);
Array isclose(const Array& a, const Array& b, double rtol = 1e-5, double atol = 1e-8, bool equal_nan = false);

// 逻辑运算
Array logical_and(const Array& a, const Array& b);
Array logical_or(const Array& a, const Array& b);
Array logical_xor(const Array& a, const Array& b);
Array logical_not(const Array& a);

// 位运算
Array bitwise_and(const Array& a, const Array& b);
Array bitwise_or(const Array& a, const Array& b);
Array bitwise_xor(const Array& a, const Array& b);
Array bitwise_not(const Array& a);
Array invert(const Array& a);
```

### 统计

```cpp
// 基本聚合 (axis=-1=全部元素, keepdims=false)
Array sum(const Array& a, int axis = -1, bool keepdims = false);       // 累加和
Array mean(const Array& a, int axis = -1, bool keepdims = false);      // 算术均值
Array min(const Array& a, int axis = -1, bool keepdims = false);       // 最小值
Array max(const Array& a, int axis = -1, bool keepdims = false);       // 最大值
Array prod(const Array& a, int axis = -1, bool keepdims = false);      // 连乘积

// 方差与标准差
Array stddev(const Array& a, int axis = -1, bool keepdims = false);    // 总体标准差（避让 C++ std 命名空间）
Array var(const Array& a, int axis = -1, int ddof = 0, bool keepdims = false); // 方差

// 累积操作
Array cumsum(const Array& a, int axis = -1);                            // 累加和
Array cumprod(const Array& a, int axis = -1);                           // 累乘积

// 顺序统计量
Array median(const Array& a, int axis = -1, bool keepdims = false);    // 中位数
Array percentile(const Array& a, const Array& q, int axis = -1);       // 百分位数
Array quantile(const Array& a, const Array& q, int axis = -1);         // 分位数

// 索引操作
Array argmax(const Array& a, int axis = -1, bool keepdims = false);    // 最大值索引
Array argmin(const Array& a, int axis = -1, bool keepdims = false);    // 最小值索引
Array count_nonzero(const Array& a, int axis = -1);                    // 非零元素计数

// 其他
Array ptp(const Array& a, int axis = -1, bool keepdims = false);       // 峰峰值：max - min
Array all(const Array& a, int axis = -1, bool keepdims = false);       // 全部为真
Array any(const Array& a, int axis = -1, bool keepdims = false);       // 任一为真
Array where(const Array& condition, const Array& x, const Array& y);   // 条件选择

// 加权与相关性
Array average(const Array& a, int axis = -1, bool keepdims = false);   // 普通均值
Array average(const Array& a, const Array& weights, int axis = -1, bool keepdims = false); // 加权均值
Array cov(const Array& m, bool rowvar = true, int ddof = 0, DType dtype = Float64); // 协方差
Array corrcoef(const Array& m, bool rowvar = true);                     // 相关系数矩阵
Array correlate(const Array& a, const Array& v, const char* mode = "valid"); // 互相关

// 积分
Array trapz(const Array& y, double dx = 1.0, int axis = -1);           // 梯形积分
Array trapz(const Array& y, const Array& x, double dx = 1.0, int axis = -1);

// ── NaN 感知版本 ───────────────────────────────────────
Array nanmax(const Array& a, int axis = -1);         Array nanmin(const Array& a, int axis = -1);
Array nanmean(const Array& a, int axis = -1);        Array nanmedian(const Array& a, int axis = -1);
Array nanpercentile(const Array& a, const Array& q, int axis = -1);
Array nanquantile(const Array& a, const Array& q, int axis = -1);
Array nancumsum(const Array& a, int axis = -1);      Array nancumprod(const Array& a, int axis = -1);
Array nansum(const Array& a, int axis = -1);         Array nanprod(const Array& a, int axis = -1);
Array nanstd(const Array& a, int axis = -1, bool keepdims = false);
Array nanvar(const Array& a, int axis = -1, int ddof = 0, bool keepdims = false);
Array nan_to_num(const Array& a);                    // NaN→0, inf→有限值
Array isnan(const Array& a);                         // 检测 NaN
Array isinf(const Array& a);                         // 检测无穷
Array isfinite(const Array& a);                      // 检测有穷
Array nanargmax(const Array& a, int axis = -1);      // 忽略 NaN 的最大值索引
Array nanargmin(const Array& a, int axis = -1);      // 忽略 NaN 的最小值索引
```

### 线性代数

```cpp
// ── 结果结构体 ──────────────────────────────────────────
struct SVDResult { Array U, s, Vt; };
struct QRResult  { Array Q, R; };
struct LUResult  { Array P, L, U; };
struct EigResult { Array eigenvalues, eigenvectors; };

// 矩阵乘积
Array dot(const Array& a, const Array& b);          // 点积
Array matmul(const Array& a, const Array& b);       // 矩阵乘法（支持高维）
Array vdot(const Array& a, const Array& b);         // 共轭内积
Array inner(const Array& a, const Array& b);        // 内积
Array outer(const Array& a, const Array& b);        // 外积
Array cross(const Array& a, const Array& b);        // 叉积
Array kron(const Array& a, const Array& b);         // Kronecker 积
Array tensordot(const Array& a, const Array& b, const Array& axes_a, const Array& axes_b);
Array tensordot(const Array& a, const Array& b, int axes);

// 矩阵属性
Array inv(const Array& a);                           // 矩阵求逆（2-D 方阵）
Array pinv(const Array& a, double rcond = 1e-15);    // Moore-Penrose 伪逆
Array det(const Array& a);                           // 行列式
Array trace(const Array& a);                         // 迹（对角线之和）

// 分解
Array cholesky(const Array& a);                      // Cholesky 分解
SVDResult svd(const Array& a, bool full_matrices = true); // 奇异值分解
QRResult qr(const Array& a);                         // QR 分解
LUResult lu(const Array& a);                         // LU 分解（部分选主元）
EigResult eig(const Array& a);                       // 特征值分解

// 原地操作
void swap_rows(Array& arr, int row1, int row2);
void swap_cols(Array& arr, int col1, int col2);
```

### 形状操作

```cpp
// 重塑
Array reshape(const Array& a, std::initializer_list<int> new_shape);
Array resize(const Array& a, std::initializer_list<int> new_shape);
Array transpose(const Array& a);                     // 完全转置
Array flatten(const Array& a);                       // 展平为 1-D（C 顺序）
Array ravel(const Array& a, const char* order = "C");// 按指定顺序展平
Array squeeze(const Array& a, int axis = -1);        // 移除长度为 1 的维度
Array expand_dims(const Array& a, int axis);         // 插入新轴

// 堆叠
Array stack(const std::vector<Array>& arrays, int axis = 0);  // 沿新轴连接
Array hstack(const std::vector<Array>& arrays);               // 水平堆叠
Array vstack(const std::vector<Array>& arrays);               // 垂直堆叠
Array dstack(const std::vector<Array>& arrays);               // 深度堆叠
Array column_stack(const std::vector<Array>& arrays);         // 列堆叠
Array concatenate(const Array& a, const Array& b, int axis);  // 拼接两个数组
Array append(const Array& a, const Array& values, int axis);  // 追加

// 拆分（返回 std::vector<Array>）
std::vector<Array> split(const Array& a, const Array& indices, int axis = 0);
std::vector<Array> hsplit(const Array& a, const Array& indices);
std::vector<Array> vsplit(const Array& a, const Array& indices);
std::vector<Array> dsplit(const Array& a, const Array& indices);

// 重复与平铺
Array tile(const Array& a, std::initializer_list<int> reps);   // 平铺
Array repeat(const Array& a, const Array& repeats, int axis);  // 重复

// 翻转与滚动
Array flip(const Array& a, int axis = -1);
Array fliplr(const Array& a);
Array flipud(const Array& a);
Array roll(const Array& a, int shift, int axis = -1);
Array rot90(const Array& m, int k = 1, int axis1 = 0, int axis2 = 1);

// 轴操作
Array moveaxis(const Array& a, int source, int destination);  // 移动轴
Array rollaxis(const Array& a, int axis, int start);          // 滚动轴
Array swapaxes(const Array& a, int axis1, int axis2);         // 交换轴

// 广播
Array broadcast_to(const Array& a, std::initializer_list<int> shape);
std::vector<Array> broadcast_arrays(const std::vector<Array>& arrays);

// 对角线
Array diag(const Array& v, int k = 0);
Array diagflat(const Array& a, int k = 0);
Array diagonal(const Array& a, int offset = 0, int axis1 = 0, int axis2 = 1);

// 其他
Array atleast_1d(const Array& a);  Array atleast_2d(const Array& a);
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

### 快速傅里叶变换 (FFT)

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

### 随机数生成

```cpp
// RNG 控制
void random_seed(uint64_t seed);  uint64_t random_uint64();  double random_double();

// 均匀分布
Array random_sample(std::initializer_list<int> shape = {});      Array rand(std::initializer_list<int> shape = {});
Array randint(int low, int high, std::initializer_list<int> shape = {}, DType dtype = Int64);
Array uniform(double low, double high, std::initializer_list<int> shape = {});

// 正态分布
double standard_normal_val();  Array standard_normal(std::initializer_list<int> shape = {});
Array randn(std::initializer_list<int> shape = {});              Array normal(double loc, double scale, std::initializer_list<int> shape = {});

// 指数分布
double standard_exponential_val();  Array standard_exponential(std::initializer_list<int> shape = {});
Array exponential(double scale, std::initializer_list<int> shape = {});

// Gamma 族
double standard_gamma_val(double shape);  Array standard_gamma(double shape, std::initializer_list<int> out_shape = {});
Array gamma(double shape, double scale, std::initializer_list<int> out_shape = {});
Array chisquare(double df, std::initializer_list<int> shape = {});
Array beta(double a, double b, std::initializer_list<int> shape = {});
Array standard_t(double df, std::initializer_list<int> shape = {});
Array f(double dfn, double dfd, std::initializer_list<int> shape = {});

// 连续分布
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

// 离散分布
Array geometric(double p, std::initializer_list<int> shape = {});
Array poisson(double lam, std::initializer_list<int> shape = {});
Array binomial(int n, double p, std::initializer_list<int> shape = {});
Array negative_binomial(int n, double p, std::initializer_list<int> shape = {});
Array hypergeometric(int ngood, int nbad, int nsample, std::initializer_list<int> shape = {});
Array zipf(double a, std::initializer_list<int> shape = {});
Array logseries(double p, std::initializer_list<int> shape = {});

// 多元与工具函数
Array permutation(const Array& x);  void shuffle(Array& x);
Array choice(const Array& a, int size, bool replace = true, const Array& p = Array());
Array multinomial(int n, const Array& pvals, std::initializer_list<int> shape = {});
Array multivariate_normal(const Array& mean, const Array& cov, std::initializer_list<int> shape = {});
Array dirichlet(const Array& alpha, std::initializer_list<int> shape = {});
Array random_bytes(int n);
```

### 文件 I/O

```cpp
// 文本格式
Array readtxt(const char* filename, const char* delimiter = " ", ...);
Array genfromtxt(const char* filename, const char* delimiter = ",", int skip_header = 0);
int savetxt(const char* filename, const Array& arr, const char* fmt = "%g", ...);

// 二进制 NPY
int save_npy(const char* filename, const Array& arr);
Array load_npy(const char* filename);

// NPZ
int savez(const char* filename, const std::vector<Array>& arrays);

// std::string 重载
int savetxt(const std::string& filename, const Array& arr, ...);
int save_npy(const std::string& filename, const Array& arr);
Array load_npy(const std::string& filename);
Array readtxt(const std::string& filename, const char* delimiter = " ");
```

### 多项式

```cpp
Array poly(const Array& roots);                                  // 从根构造多项式
Array roots(const Array& p);                                     // 从系数求根
Array polyadd(const Array& a, const Array& b);                   // 多项式相加
Array polysub(const Array& a, const Array& b);                   // 多项式相减
Array polymul(const Array& a, const Array& b);                   // 多项式相乘
std::pair<Array, Array> polydiv(const Array& u, const Array& v); // 除法 → {商, 余数}
Array polyder(const Array& p, int m = 1);                        // 求导
Array polyint(const Array& p, int m = 1);                        // 积分
Array polyval(const Array& p, const Array& x);                   // 求值
Array polyfit(const Array& x, const Array& y, int deg);          // 最小二乘拟合
```

### 搜索与集合操作

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

### 窗函数

```cpp
Array bartlett(int M);   Array blackman(int M);   Array kaiser(int M, double beta);
Array hanning(int M);    Array hamming(int M);
```

### 直方图

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

### 插值与差分

```cpp
Array interp(const Array& arr, const Array& xp, const Array& fp,
             const Array& left = Array(), const Array& right = Array());
Array diff(const Array& arr, int n = 1, int axis = -1);
Array ediff1d(const Array& y, const Array& to_begin = Array(), const Array& to_end = Array());
std::vector<Array> gradient(const Array& arr, const Array& spacing = Array());
```

### 排序与特殊函数

```cpp
Array sort(const Array& a);
Array argsort(const Array& arr, int axis = -1, const char* kind = "quicksort");
Array argpartition(const Array& arr, int kth, int axis = -1);
Array partition(const Array& arr, int kth, int axis = -1);
Array piecewise(const Array& x, const std::vector<Array>& condlist,
                const std::vector<double(*)(double)>& funclist);
Array unwrap(const Array& arr, double discont = M_PI, int axis = -1);
```

### 原地修改

```cpp
int place(Array& arr, const Array& mask, const Array& vals);
int put(Array& arr, const Array& indices, const Array& values, const char* mode = "raise");
void putmask(Array& arr, const Array& mask, const Array& values);
void fill_diagonal(Array& arr, double val);
```

### 杂项

```cpp
Array convolve(const Array& a, const Array& v);                   // 一维卷积
Array extract(const Array& condition, const Array& arr);          // 条件提取
Array trim_zeros(const Array& arr, const char* trim = "fb");      // 修剪首尾零
std::pair<Array, Array> frexp(const Array& arr);                  // 拆分为尾数和指数
std::pair<Array, Array> modf(const Array& arr);                   // 拆分为小数和整数
Array frombuffer(const void* buffer, DType dtype, int count, size_t offset = 0);
Array fromfunction(double(*func)(int*, int), std::initializer_list<int> shape, DType dtype = Float64);
Array insert(const Array& arr, const Array& obj, const Array& values, int axis);
Array array_delete(const Array& arr, const Array& obj, int axis); // 删除子数组
Array linspace_float(float start, float stop, int num);           // 单精度 linspace
std::vector<Array> ix_(const std::vector<Array>& arrays);         // 开网格索引
```

## 架构设计

### RAII 所有权

每个 `npc::Array` 通过 `std::unique_ptr<::Array, ArrayDeleter>` 拥有其底层 `::Array*`。C++ 对象离开作用域时，自动调用 `free_array()`。完全无需手动内存管理。

### 拷贝与移动

- **拷贝** (`Array(const Array&)`): 通过 `copy_array()` 深拷贝底层 C 数组。拷贝是独立的。
- **移动** (`Array(Array&&)`): 转移所有权。源数组变为空数组。
- **拷贝赋值** (`operator=`): 深拷贝，源数组不变。
- **移动赋值**: 转移所有权。

### 运算符重载

所有算术运算符 (`+`, `-`, `*`, `/`) 返回拥有自身数据的新 `npc::Array` 对象。复合赋值 (`+=`, `-=` 等) 原地修改左操作数。

比较运算符 (`equal`, `greater` 等) 返回布尔数组（逐元素结果）。C++ 的 `operator==` **刻意未重载** — NumPy 风格的逐元素比较通过命名函数进行。

### C++ 与 libnpc 命名对照

| libnpc C | libnpc++ C++ | 原因 |
|---|---|---|
| `free_array(ptr)` | RAII 析构函数 | 自动清理 |
| `get_item(arr, idx)` | `a.at<T>({i,j})`, `a(i,j)`, `a[3]` | 运算符重载 |
| `std(arr, axis, keepdims)` | `stddev(arr, axis, keepdims)` | `std` 是 C++ 保留命名空间 |
| `delete(arr, obj, axis)` | `array_delete(arr, obj, axis)` | `delete` 是 C++ 关键字 |
| `DataType` 枚举 | `DType` 有作用域枚举 | C++ enum class |
| `int keepdims` (0/1) | `bool keepdims` | 惯用 C++ |
| `int* shape, int ndim` | `std::initializer_list<int>` | 现代 C++ 语法 |

## 许可证

MIT — 详见 LICENSE 文件。

libnpc（位于 `third_party/libnpc/`）同样由 xiayang558 以 MIT 许可证发布。
