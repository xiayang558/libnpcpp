# libnpc++

[English](README.md) | [中文](README_zh.md) | [📘 API 文档](https://xiayang558.github.io/libnpcpp)

[libnpc](https://github.com/xiayang558/libnpc) 的 C++17 RAII 封装层。libnpc 是一个纯 C 数值计算库，重新实现了 NumPy 核心功能（300+ 函数，21 个模块），**零 Python 依赖**。

[![Version](https://img.shields.io/badge/version-1.0.0-blue)](https://github.com/xiayang558/libnpcpp)
[![C++17](https://img.shields.io/badge/C++-17-blue)](https://en.cppreference.com/w/cpp/17)
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE)

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
| **数学 (一元)** | `abs`, `sqrt`, `exp`, `log`, `log10`, `log2`, `sin`, `cos`, `tan`, `floor`, `ceil`, `round`, `sign` 等 | 35 |
| **数学 (二元)** | `power`, `hypot`, `logaddexp`, `arctan2`, `max`, `min`, `mod`, `clip` | 11 |
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
