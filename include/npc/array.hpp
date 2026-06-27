#pragma once

#include <memory>
#include <vector>
#include <initializer_list>
#include <iostream>
#include <cassert>

#include "detail/deleter.hpp"
#include "dtype.hpp"

namespace npc {

/// RAII C++ wrapper around libnpc's ::Array.
///
/// Provides:
///   - Automatic memory management (no manual free_array calls)
///   - Move semantics for efficient transfer
///   - Deep copy on copy construction/assignment
///   - Typed element access via templates
///   - Operator overloads for arithmetic and comparison
class Array {
public:
    // ── Constructors ──────────────────────────────────────────

    /// Default: creates a null array (wraps nullptr).
    Array() : ptr_(nullptr) {}

    /// Take ownership of a raw ::Array pointer.
    /// @param ptr  Raw C array pointer (may be nullptr).
    /// @param owned  If true, the Array takes ownership and will free it.
    explicit Array(::Array* ptr, bool owned = true) : ptr_(owned ? ptr : nullptr) {
        if (!owned && ptr) {
            // Non-owning view: deep-copy instead.
            ptr_.reset(copy_array(ptr));
        }
    }

    /// Deep copy constructor.
    Array(const Array& other) : ptr_(nullptr) {
        if (other.ptr_) {
            ptr_.reset(copy_array(other.ptr_.get()));
        }
    }

    /// Move constructor — transfers ownership.
    Array(Array&& other) noexcept = default;

    /// Destructor — automatically frees the C array.
    ~Array() = default;

    // ── Assignment ────────────────────────────────────────────

    /// Deep copy assignment.
    Array& operator=(const Array& other) {
        if (this != &other) {
            ptr_.reset(other.ptr_ ? copy_array(other.ptr_.get()) : nullptr);
        }
        return *this;
    }

    /// Move assignment — transfers ownership.
    Array& operator=(Array&& other) noexcept = default;

    // ── Accessors ────────────────────────────────────────────

    /// Number of dimensions.
    int ndim() const { return ptr_ ? ptr_->ndim : 0; }

    /// Total number of elements.
    int size() const { return ptr_ ? ptr_->size : 0; }

    /// Pointer to shape array (length = ndim()).
    const int* shape() const { return ptr_ ? ptr_->shape : nullptr; }

    /// Pointer to strides array (length = ndim()).
    const int* strides() const { return ptr_ ? ptr_->strides : nullptr; }

    /// Data type of the array elements.
    DType dtype() const { return ptr_ ? static_cast<DType>(ptr_->dtype) : DType::Unknown; }

    /// True if this array is a view into another array's data.
    bool is_view() const { return ptr_ ? ptr_->is_view : false; }

    /// Shape as a std::vector for convenience.
    std::vector<int> shape_vec() const {
        if (!ptr_ || !ptr_->shape) return {};
        return std::vector<int>(ptr_->shape, ptr_->shape + ptr_->ndim);
    }

    /// Check if this array is null (wraps no data).
    bool is_null() const { return ptr_ == nullptr; }
    explicit operator bool() const { return !is_null(); }

    // ── Typed element access ──────────────────────────────────

    /// Access a single element by indices. Returns a reference.
    /// Usage: arr.at<double>({i, j})
    template <typename T>
    T& at(std::initializer_list<int> indices) {
        assert(ptr_ && "Array is null");
        assert(static_cast<int>(indices.size()) == ptr_->ndim && "Index count must match ndim");
        void* p = get_item(ptr_.get(), const_cast<int*>(indices.begin()));
        return *static_cast<T*>(p);
    }

    /// Const version of at<T>().
    template <typename T>
    const T& at(std::initializer_list<int> indices) const {
        assert(ptr_ && "Array is null");
        assert(static_cast<int>(indices.size()) == ptr_->ndim && "Index count must match ndim");
        void* p = get_item(ptr_.get(), const_cast<int*>(indices.begin()));
        return *static_cast<const T*>(p);
    }

    /// Get first element as a scalar value.
    /// Works for arrays of any dimensionality (0-d scalar, 1-d, 2-d, etc.).
    template <typename T>
    T scalar() const {
        assert(ptr_ && "Array is null");
        assert(ptr_->size > 0 && "Array has no elements");
        return *static_cast<const T*>(ptr_->data);
    }

    // ── Convenience indexing ──────────────────────────────────

    /// Multi-dimensional indexing via variadic template.
    /// Usage: a(0), a(1, 2), a(0, 1, 2)
    template <typename... Idx>
    double operator()(Idx... idx) const {
        return at<double>({static_cast<int>(idx)...});
    }

    /// Single-element access for 1-D arrays (or flat indexing).
    /// Returns a double for convenience; use at<T>() for typed access.
    double operator[](int idx) const {
        assert(ptr_ && "Array is null");
        if (ptr_->ndim == 0) return *static_cast<double*>(ptr_->data);
        assert(ptr_->ndim == 1 && "operator[] requires 1-D array; use operator() for multi-dim");
        assert(idx >= 0 && idx < ptr_->size && "Index out of bounds");
        return *static_cast<double*>(get_item(ptr_.get(), &idx));
    }

    double& operator[](int idx) {
        assert(ptr_ && "Array is null");
        if (ptr_->ndim == 0) return *static_cast<double*>(ptr_->data);
        assert(ptr_->ndim == 1 && "operator[] requires 1-D array; use operator() for multi-dim");
        assert(idx >= 0 && idx < ptr_->size && "Index out of bounds");
        return *static_cast<double*>(get_item(ptr_.get(), &idx));
    }

    /// Get typed pointer to raw data.
    template <typename T>
    T* data() {
        return static_cast<T*>(ptr_ ? ptr_->data : nullptr);
    }

    /// Const typed pointer to raw data.
    template <typename T>
    const T* data() const {
        return static_cast<const T*>(ptr_ ? ptr_->data : nullptr);
    }

    // ── Printing ──────────────────────────────────────────────

    /// Print array contents to stdout (delegates to libnpc's print_array).
    void print() const {
        if (ptr_) print_array(ptr_.get());
    }

    /// Print array metadata (dtype, shape, strides).
    void info() const {
        if (ptr_) array_info(ptr_.get());
    }

    // ── Stream output ─────────────────────────────────────────

    friend std::ostream& operator<<(std::ostream& os, const Array& arr) {
        if (arr.is_null()) { os << "Array(null)"; return os; }
        os << "Array(dtype=" << static_cast<int>(arr.dtype())
           << ", ndim=" << arr.ndim() << ", shape=[";
        for (int i = 0; i < arr.ndim(); ++i) {
            if (i > 0) os << ",";
            os << arr.shape()[i];
        }
        os << "])";
        return os;
    }

    /// Transpose view (reverses all axes). Like numpy .T attribute.
    Array T() const { return Array(::transpose(get())); }

    // ── NumPy-style built-in methods ──────────────────────────

    /// Element-wise absolute value.
    Array abs() const { return Array(::abs_array(get())); }
    /// Element-wise square root.
    Array sqrt() const { return Array(::sqrt_array(get())); }
    /// Element-wise exponential (e^x).
    Array exp() const { return Array(::exp_array(get())); }
    /// Element-wise natural logarithm.
    Array log() const { return Array(::log_array(get())); }
    /// Element-wise base-10 logarithm.
    Array log10() const { return Array(::log10_array(get())); }
    /// Element-wise base-2 logarithm.
    Array log2() const { return Array(::log2_array(get())); }
    /// Element-wise sine.
    Array sin() const { return Array(::sin_array(get())); }
    /// Element-wise cosine.
    Array cos() const { return Array(::cos_array(get())); }
    /// Element-wise tangent.
    Array tan() const { return Array(::tan_array(get())); }
    /// Element-wise floor (round down).
    Array floor() const { return Array(::floor_array(get())); }
    /// Element-wise ceil (round up).
    Array ceil() const { return Array(::ceil_array(get())); }
    /// Round to nearest integer, ties to even.
    Array rint() const { return Array(::arint(get())); }
    /// Truncate toward zero.
    Array trunc() const { return Array(::atrunc(get())); }
    /// Round toward zero.
    Array fix() const { return Array(::fix(get())); }
    /// Round to given decimals.
    Array round(int decimals = 0) const { return Array(::around(get(), decimals)); }
    /// Element-wise sign (-1, 0, +1).
    Array sign() const { return Array(::sign(get())); }
    /// Element-wise power (exponentiation). self^exponent.
    Array power(const Array& exponent) const { return Array(::power(get(), exponent.get())); }

    // ── Aggregation methods ───────────────────────────────────

    /// Sum of array elements.
    Array sum(int axis = -1, bool keepdims = false) const { return Array(::sum(get(), axis, keepdims ? 1 : 0)); }
    /// Arithmetic mean.
    Array mean(int axis = -1, bool keepdims = false) const { return Array(::mean(get(), axis, keepdims ? 1 : 0)); }
    /// Minimum value.
    Array min(int axis = -1, bool keepdims = false) const { return Array(::min(get(), axis, keepdims ? 1 : 0)); }
    /// Maximum value.
    Array max(int axis = -1, bool keepdims = false) const { return Array(::max(get(), axis, keepdims ? 1 : 0)); }
    /// Product of elements.
    Array prod(int axis = -1, bool keepdims = false) const { return Array(::prod(get(), axis, keepdims ? 1 : 0)); }
    /// Standard deviation (population).
    Array stddev(int axis = -1, bool keepdims = false) const { return Array(::npc_std(get(), axis, keepdims ? 1 : 0)); }
    /// Variance with optional delta DOF.
    Array var(int axis = -1, int ddof = 0, bool keepdims = false) const { return Array(::var(get(), axis, ddof, keepdims ? 1 : 0)); }
    /// Median value.
    Array median(int axis = -1, bool keepdims = false) const { return Array(::median(get(), axis, keepdims ? 1 : 0)); }
    /// Indices of maximum values.
    Array argmax(int axis = -1, bool keepdims = false) const { return Array(::argmax(get(), axis, keepdims ? 1 : 0)); }
    /// Indices of minimum values.
    Array argmin(int axis = -1, bool keepdims = false) const { return Array(::argmin(get(), axis, keepdims ? 1 : 0)); }
    /// Test if all elements are true.
    Array all(int axis = -1, bool keepdims = false) const { return Array(::all(get(), axis, keepdims ? 1 : 0)); }
    /// Test if any element is true.
    Array any(int axis = -1, bool keepdims = false) const { return Array(::any(get(), axis, keepdims ? 1 : 0)); }
    /// Count non-zero elements.
    Array count_nonzero(int axis = -1) const { return Array(::count_nonzero(get(), axis)); }
    /// Peak-to-peak (max - min).
    Array ptp(int axis = -1, bool keepdims = false) const { return Array(::ptp(get(), axis, keepdims ? 1 : 0)); }

    // ── Cumulative ────────────────────────────────────────────

    /// Cumulative sum along axis.
    Array cumsum(int axis = -1) const { return Array(::cumsum(get(), axis)); }
    /// Cumulative product along axis.
    Array cumprod(int axis = -1) const { return Array(::cumprod(get(), axis)); }

    // ── Shape manipulation ────────────────────────────────────

    /// Reshape to new dimensions.
    Array reshape(std::initializer_list<int> shape) const {
        std::vector<int> sh(shape);
        return Array(::reshape(get(), sh.data(), static_cast<int>(sh.size())));
    }
    /// Full transpose (reverse all axes).
    Array transpose() const { return Array(::transpose(get())); }
    /// Flatten to 1-D (C-order).
    Array flatten() const { return Array(::flatten(get())); }
    /// Flatten with specified order.
    Array ravel(const char* order = "C") const { return Array(::ravel(get(), order)); }
    /// Remove dimensions of length 1.
    Array squeeze(int axis = -1) const { return Array(::squeeze(get(), axis)); }
    /// Add a new axis at the given position.
    Array expand_dims(int axis) const { return Array(::expand_dims(get(), axis)); }
    /// Reverse elements along axis.
    Array flip(int axis = -1) const { return Array(::flip(get(), axis)); }
    /// Roll elements along axis.
    Array roll(int shift, int axis = -1) const { return Array(::roll(get(), shift, axis)); }
    /// Broadcast to target shape.
    Array broadcast_to(std::initializer_list<int> shape) const {
        std::vector<int> sh(shape);
        return Array(::broadcast_to(get(), sh.data(), static_cast<int>(sh.size())));
    }
    /// Tile by given repetition factors.
    Array tile(std::initializer_list<int> reps) const {
        std::vector<int> r(reps);
        return Array(::tile(get(), r.data(), static_cast<int>(r.size())));
    }
    /// Pad the array.
    Array pad(const Array& pad_width, const char* mode = "constant", double cv = 0.0) const {
        return Array(::pad(get(), pad_width.get(), mode, cv));
    }
    /// Swap two axes.
    Array swapaxes(int axis1, int axis2) const {
        if (axis1 == axis2) return Array(::copy_array(get()));
        // Swap by moving axis1→axis2, then moving the (adjusted) axis2→axis1
        auto tmp = Array(::moveaxis(get(), axis1, axis2));
        int adj = (axis1 < axis2) ? axis2 - 1 : axis2;
        return Array(::moveaxis(tmp.get(), axis2 < axis1 ? axis2 + 1 : axis2, axis1));
    }

    // ── Type conversion / copy ────────────────────────────────

    /// Deep copy of this array.
    Array copy() const { return Array(::copy_array(get())); }
    /// Cast to a different data type.
    Array astype(DType new_dtype) const { return Array(::astype(get(), static_cast<DataType>(new_dtype))); }

    // ── Linear algebra ────────────────────────────────────────

    /// Dot product with another array.
    Array dot(const Array& other) const { return Array(::dot(get(), other.get())); }
    /// Matrix multiplication.
    Array matmul(const Array& other) const { return Array(::matmul(get(), other.get())); }
    /// Trace (sum of diagonal elements).
    Array trace() const { return Array(::trace(get())); }
    /// Determinant.
    Array det() const { return Array(::det(get())); }
    /// Matrix inverse (2-D square).
    Array inv() const { return Array(::inv(get())); }
    /// Conjugate (Hermitian) transpose.
    Array conj_transpose() const { return Array(::conj_transpose(get())); }

    // ── Comparison / logic ────────────────────────────────────

    /// Element-wise equality test. Returns BOOL array.
    Array equal(const Array& other) const { return Array(::equal(get(), other.get())); }
    /// Element-wise inequality test.
    Array not_equal(const Array& other) const { return Array(::logical_not(::equal(get(), other.get()))); }
    /// Element-wise greater-than.
    Array greater(const Array& other) const { return Array(::greater(get(), other.get())); }
    /// Element-wise greater-equal.
    Array greater_equal(const Array& other) const { return Array(::greater_equal(get(), other.get())); }
    /// Element-wise less-than.
    Array less(const Array& other) const { return Array(::greater(other.get(), get())); }
    /// Element-wise less-equal.
    Array less_equal(const Array& other) const { return Array(::greater_equal(other.get(), get())); }

    // ── NaN utilities ─────────────────────────────────────────

    /// Test each element for NaN.
    Array isnan() const { return Array(::isnan_array(get())); }
    /// Test each element for infinity.
    Array isinf() const { return Array(::isinf_array(get())); }
    /// Test each element for finiteness.
    Array isfinite() const { return Array(::isfinite_array(get())); }
    /// Replace NaN and infinity with finite values.
    Array nan_to_num() const { return Array(::nan_to_num(get())); }

    // ── Sorting ───────────────────────────────────────────────

    /// Sort along first axis.
    Array sort() const { return Array(::msort(get())); }
    /// Indices that would sort the array.
    Array argsort(int axis = -1, const char* kind = "quicksort") const { return Array(::argsort(get(), axis, kind)); }

    // ── Misc ──────────────────────────────────────────────────

    /// Extract diagonal or construct diagonal matrix.
    Array diag(int k = 0) const { return Array(::diag(get(), k)); }
    /// Clip values to [lo, hi] range.
    Array clip(const Array& lo, const Array& hi) const { return Array(::clip(get(), lo.get(), hi.get())); }
    /// In-place fill diagonal with a value.
    void fill_diagonal(double val) { ::fill_diagonal(get(), &val); }
    /// Element-wise clip between two values.
    Array clip(double lo, double hi) const {
        double lo_v = lo, hi_v = hi;
        Array lo_arr(::create_scalar_array(&lo_v, FLOAT64));
        Array hi_arr(::create_scalar_array(&hi_v, FLOAT64));
        return Array(::clip(get(), lo_arr.get(), hi_arr.get()));
    }
    /// Trim leading/trailing zeros.
    Array trim_zeros(const char* trim = "fb") const { return Array(::trim_zeros(get(), trim)); }

    // ── Typed iterators ──────────────────────────────────────

    template <typename T> T* begin() { return static_cast<T*>(ptr_ ? ptr_->data : nullptr); }
    template <typename T> T* end()   { return begin<T>() + (ptr_ ? ptr_->size : 0); }
    template <typename T> const T* begin() const { return static_cast<const T*>(ptr_ ? ptr_->data : nullptr); }
    template <typename T> const T* end() const   { return begin<T>() + (ptr_ ? ptr_->size : 0); }

    // ── Raw C pointer access (escape hatch) ───────────────────

    /// Get the underlying ::Array pointer without releasing ownership.
    ::Array* get() const { return ptr_.get(); }

    /// Release ownership of the ::Array pointer. Caller must free it manually.
    ::Array* release() noexcept { return ptr_.release(); }

    /// Reset with a new raw pointer, freeing the old one.
    void reset(::Array* ptr = nullptr) { ptr_.reset(ptr); }

private:
    std::unique_ptr<::Array, detail::ArrayDeleter> ptr_;
};

} // namespace npc
