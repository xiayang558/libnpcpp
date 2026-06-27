#include <npc/npc.hpp>
#include <cstdio>
#include <cmath>

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) \
    void test_##name(); \
    struct Register_##name { \
        Register_##name() { \
            tests_run++; \
            printf("  RUN   %s\n", #name); \
            test_##name(); \
            tests_passed++; \
            printf("  PASS  %s\n", #name); \
        } \
    } reg_##name; \
    void test_##name()

#define ASSERT_TRUE(expr) do { \
    if (!(expr)) { \
        printf("  FAIL  %s:%d: ASSERT_TRUE(%s)\n", __FILE__, __LINE__, #expr); \
        tests_failed++; tests_passed--; return; \
    } \
} while(0)

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        printf("  FAIL  %s:%d: ASSERT_EQ(%s, %s)\n", __FILE__, __LINE__, #a, #b); \
        tests_failed++; tests_passed--; return; \
    } \
} while(0)

#define ASSERT_NEAR(a, b, eps) do { \
    if (std::fabs((a) - (b)) > (eps)) { \
        printf("  FAIL  %s:%d: ASSERT_NEAR(%s, %s, %s) — values: %f vs %f\n", \
               __FILE__, __LINE__, #a, #b, #eps, (double)(a), (double)(b)); \
        tests_failed++; tests_passed--; return; \
    } \
} while(0)

// ── Null array test ───────────────────────────────────────────

TEST(null_array) {
    npc::Array a;
    ASSERT_TRUE(a.is_null());
    ASSERT_EQ(a.ndim(), 0);
    ASSERT_EQ(a.size(), 0);
}

// ── zeros tests ───────────────────────────────────────────────

TEST(zeros_1d) {
    auto a = npc::zeros({5});
    ASSERT_FALSE(a.is_null());
    ASSERT_EQ(a.ndim(), 1);
    ASSERT_EQ(a.size(), 5);
    ASSERT_EQ(a.shape()[0], 5);
    ASSERT_EQ(a.dtype(), npc::DType::Float64);

    for (int i = 0; i < 5; i++) {
        ASSERT_NEAR(a.at<double>({i}), 0.0, 1e-12);
    }
}

TEST(zeros_2d) {
    auto a = npc::zeros({3, 4});
    ASSERT_FALSE(a.is_null());
    ASSERT_EQ(a.ndim(), 2);
    ASSERT_EQ(a.size(), 12);
    ASSERT_EQ(a.shape()[0], 3);
    ASSERT_EQ(a.shape()[1], 4);
}

// ── ones tests ────────────────────────────────────────────────

TEST(ones_test) {
    auto a = npc::ones({2, 3});
    ASSERT_EQ(a.ndim(), 2);
    ASSERT_EQ(a.size(), 6);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_NEAR(a.at<double>({i, j}), 1.0, 1e-12);
        }
    }
}

// ── full test ─────────────────────────────────────────────────

TEST(full_test) {
    auto a = npc::full({3}, 42.0);
    ASSERT_EQ(a.size(), 3);
    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(a.at<double>({i}), 42.0, 1e-12);
    }
}

// ── arange test ───────────────────────────────────────────────

TEST(arange_test) {
    auto a = npc::arange(0.0, 5.0, 1.0);
    ASSERT_EQ(a.size(), 5);
    for (int i = 0; i < 5; i++) {
        ASSERT_NEAR(a.at<double>({i}), (double)i, 1e-12);
    }
}

TEST(arange_float_step) {
    auto a = npc::arange(0.0, 1.0, 0.25);
    ASSERT_EQ(a.size(), 4);
    ASSERT_NEAR(a.at<double>({0}), 0.0, 1e-12);
    ASSERT_NEAR(a.at<double>({1}), 0.25, 1e-12);
    ASSERT_NEAR(a.at<double>({2}), 0.50, 1e-12);
    ASSERT_NEAR(a.at<double>({3}), 0.75, 1e-12);
}

// ── eye test ──────────────────────────────────────────────────

TEST(eye_test) {
    auto a = npc::eye(3);
    ASSERT_EQ(a.ndim(), 2);
    ASSERT_EQ(a.size(), 9);

    // Diagonal should be 1, off-diagonal 0
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double expected = (i == j) ? 1.0 : 0.0;
            ASSERT_NEAR(a.at<double>({i, j}), expected, 1e-12);
        }
    }
}

// ── Copy tests ────────────────────────────────────────────────

TEST(copy_test) {
    auto a = npc::ones({3});
    auto b = npc::copy(a);

    ASSERT_EQ(b.ndim(), 1);
    ASSERT_EQ(b.size(), 3);
    ASSERT_NEAR(b.at<double>({0}), 1.0, 1e-12);

    // Modifying copy should not affect original
    b.at<double>({0}) = 99.0;
    ASSERT_NEAR(b.at<double>({0}), 99.0, 1e-12);
    ASSERT_NEAR(a.at<double>({0}), 1.0, 1e-12);
}

// ── Addition tests ────────────────────────────────────────────

TEST(add_arrays) {
    auto a = npc::full({3}, 10.0);
    auto b = npc::full({3}, 20.0);
    auto c = a + b;

    ASSERT_EQ(c.size(), 3);
    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(c.at<double>({i}), 30.0, 1e-12);
    }
}

TEST(add_scalar) {
    auto a = npc::full({3}, 10.0);
    auto c = a + 5.0;

    ASSERT_EQ(c.size(), 3);
    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(c.at<double>({i}), 15.0, 1e-12);
    }
}

TEST(add_scalar_left) {
    auto a = npc::full({3}, 10.0);
    auto c = 5.0 + a;

    ASSERT_EQ(c.size(), 3);
    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(c.at<double>({i}), 15.0, 1e-12);
    }
}

// ── Subtraction tests ────────────────────────────────────────

TEST(sub_arrays) {
    auto a = npc::full({3}, 30.0);
    auto b = npc::full({3}, 10.0);
    auto c = a - b;

    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(c.at<double>({i}), 20.0, 1e-12);
    }
}

// ── Multiplication tests ─────────────────────────────────────

TEST(mul_arrays) {
    auto a = npc::full({3}, 6.0);
    auto b = npc::full({3}, 7.0);
    auto c = a * b;

    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(c.at<double>({i}), 42.0, 1e-12);
    }
}

TEST(mul_scalar) {
    auto a = npc::full({3}, 6.0);
    auto c = a * 7.0;

    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(c.at<double>({i}), 42.0, 1e-12);
    }
}

// ── Division tests ───────────────────────────────────────────

TEST(div_arrays) {
    auto a = npc::full({3}, 100.0);
    auto b = npc::full({3}, 4.0);
    auto c = a / b;

    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(c.at<double>({i}), 25.0, 1e-12);
    }
}

TEST(div_scalar) {
    auto a = npc::full({3}, 100.0);
    auto c = a / 4.0;

    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(c.at<double>({i}), 25.0, 1e-12);
    }
}

// ── Negation test ────────────────────────────────────────────

TEST(negate) {
    auto a = npc::full({3}, 5.0);
    auto c = -a;

    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(c.at<double>({i}), -5.0, 1e-12);
    }
}

// ── Compound assignment tests ────────────────────────────────

TEST(compound_add) {
    auto a = npc::full({3}, 10.0);
    auto b = npc::full({3}, 5.0);
    a += b;

    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(a.at<double>({i}), 15.0, 1e-12);
    }
}

// ── Comparison tests ─────────────────────────────────────────

TEST(equal_test) {
    auto a = npc::full({3}, 5.0);
    auto b = npc::full({3}, 5.0);
    auto c = npc::equal(a, b);

    ASSERT_EQ(c.dtype(), npc::DType::Bool);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(c.at<bool>({i}), true);
    }
}

TEST(greater_test) {
    auto a = npc::full({3}, 10.0);
    auto b = npc::full({3}, 5.0);
    auto c = npc::greater(a, b);

    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(c.at<bool>({i}), true);
    }
}

// ── Move semantics test ──────────────────────────────────────

TEST(move_semantics) {
    auto a = npc::ones({5});
    ASSERT_FALSE(a.is_null());

    npc::Array b = std::move(a);
    ASSERT_TRUE(a.is_null());  // a was moved from
    ASSERT_FALSE(b.is_null()); // b owns the data
    ASSERT_EQ(b.size(), 5);
}

// ── Copy assignment test ─────────────────────────────────────

TEST(copy_assignment) {
    auto a = npc::ones({3});
    npc::Array b;
    b = a;  // deep copy

    ASSERT_FALSE(a.is_null());
    ASSERT_FALSE(b.is_null());
    ASSERT_EQ(b.size(), 3);

    b.at<double>({0}) = 99.0;
    ASSERT_NEAR(a.at<double>({0}), 1.0, 1e-12); // original unchanged
}

// ── astype test ──────────────────────────────────────────────

TEST(astype_test) {
    auto a = npc::full({3}, 3.14);
    auto b = npc::astype(a, npc::DType::Int32);

    ASSERT_EQ(b.dtype(), npc::DType::Int32);
    ASSERT_EQ(b.size(), 3);
}

// ── convenience array creation test ──────────────────────────

TEST(array_from_values) {
    auto a = npc::array({1.0, 2.0, 3.0, 4.0});
    ASSERT_EQ(a.ndim(), 1);
    ASSERT_EQ(a.size(), 4);
    ASSERT_NEAR(a.at<double>({0}), 1.0, 1e-12);
    ASSERT_NEAR(a.at<double>({3}), 4.0, 1e-12);
}

// ── Main ─────────────────────────────────────────────────────

int main() {
    printf("\n=== libnpc++ Test Suite ===\n\n");

    // Global register objects run their tests during static init.
    // We just report final results here.

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
