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

#define ASSERT_NEAR(a, b, eps) do { \
    if (std::fabs((a) - (b)) > (eps)) { \
        printf("  FAIL  %s:%d: ASSERT_NEAR(%s, %s, %s) — values: %f vs %f\n", \
               __FILE__, __LINE__, #a, #b, #eps, (double)(a), (double)(b)); \
        tests_failed++; tests_passed--; return; \
    } \
} while(0)

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        printf("  FAIL  %s:%d: ASSERT_EQ(%s, %s)\n", __FILE__, __LINE__, #a, #b); \
        tests_failed++; tests_passed--; return; \
    } \
} while(0)

// ── Unary math tests ─────────────────────────────────────────

TEST(sqrt_test) {
    auto a = npc::full({3}, 4.0);
    auto r = npc::sqrt(a);
    for (int i = 0; i < 3; i++) ASSERT_NEAR(r.at<double>({i}), 2.0, 1e-12);
}

TEST(exp_test) {
    auto a = npc::full({3}, 1.0);
    auto r = npc::exp(a);
    for (int i = 0; i < 3; i++) ASSERT_NEAR(r.at<double>({i}), M_E, 1e-12);
}

TEST(log_test) {
    auto a = npc::full({3}, M_E);
    auto r = npc::log(a);
    for (int i = 0; i < 3; i++) ASSERT_NEAR(r.at<double>({i}), 1.0, 1e-12);
}

TEST(log10_test) {
    auto a = npc::full({3}, 100.0);
    auto r = npc::log10(a);
    for (int i = 0; i < 3; i++) ASSERT_NEAR(r.at<double>({i}), 2.0, 1e-12);
}

TEST(log2_test) {
    auto a = npc::full({3}, 8.0);
    auto r = npc::log2(a);
    for (int i = 0; i < 3; i++) ASSERT_NEAR(r.at<double>({i}), 3.0, 1e-12);
}

TEST(abs_test) {
    auto a = npc::full({3}, -5.0);
    auto r = npc::abs(a);
    for (int i = 0; i < 3; i++) ASSERT_NEAR(r.at<double>({i}), 5.0, 1e-12);
}

TEST(sin_test) {
    auto a = npc::arange(0.0, 3.0, 1.0);  // [0, 1, 2]
    auto r = npc::sin(a);
    ASSERT_NEAR(r.at<double>({0}), 0.0, 1e-12);
    ASSERT_NEAR(r.at<double>({1}), std::sin(1.0), 1e-12);
}

TEST(cos_test) {
    auto a = npc::arange(0.0, 3.0, 1.0);
    auto r = npc::cos(a);
    ASSERT_NEAR(r.at<double>({0}), 1.0, 1e-12);
    ASSERT_NEAR(r.at<double>({1}), std::cos(1.0), 1e-12);
}

TEST(tan_test) {
    auto a = npc::arange(0.0, 3.0, 1.0);
    auto r = npc::tan(a);
    ASSERT_NEAR(r.at<double>({0}), 0.0, 1e-12);
}

TEST(floor_test) {
    auto a = npc::arange(1.5, 4.5, 1.0);
    auto r = npc::floor(a);
    ASSERT_NEAR(r.at<double>({0}), 1.0, 1e-12);
    ASSERT_NEAR(r.at<double>({1}), 2.0, 1e-12);
    ASSERT_NEAR(r.at<double>({2}), 3.0, 1e-12);
}

TEST(ceil_test) {
    auto a = npc::arange(1.5, 4.5, 1.0);
    auto r = npc::ceil(a);
    ASSERT_NEAR(r.at<double>({0}), 2.0, 1e-12);
    ASSERT_NEAR(r.at<double>({1}), 3.0, 1e-12);
    ASSERT_NEAR(r.at<double>({2}), 4.0, 1e-12);
}

TEST(sign_test) {
    auto a = npc::arange(-2.0, 3.0, 1.0);
    auto r = npc::sign(a);
    ASSERT_NEAR(r.at<double>({0}), -1.0, 1e-12);
    ASSERT_NEAR(r.at<double>({1}), -1.0, 1e-12);
    ASSERT_NEAR(r.at<double>({2}),  0.0, 1e-12);
    ASSERT_NEAR(r.at<double>({3}),  1.0, 1e-12);
    ASSERT_NEAR(r.at<double>({4}),  1.0, 1e-12);
}

TEST(deg2rad_test) {
    auto a = npc::full({2}, 180.0);
    auto r = npc::deg2rad(a);
    ASSERT_NEAR(r.at<double>({0}), M_PI, 1e-12);
}

TEST(rad2deg_test) {
    auto a = npc::full({2}, M_PI);
    auto r = npc::rad2deg(a);
    ASSERT_NEAR(r.at<double>({0}), 180.0, 1e-12);
}

// ── Binary math tests ────────────────────────────────────────

TEST(power_test) {
    auto a = npc::full({3}, 2.0);
    auto b = npc::full({3}, 3.0);
    auto r = npc::power(a, b);
    for (int i = 0; i < 3; i++) ASSERT_NEAR(r.at<double>({i}), 8.0, 1e-12);
}

TEST(maximum_test) {
    auto a = npc::arange(0.0, 5.0, 1.0);
    auto b = npc::arange(5.0, 0.0, -1.0);
    auto r = npc::maximum(a, b);
    ASSERT_NEAR(r.at<double>({0}), 5.0, 1e-12);
    ASSERT_NEAR(r.at<double>({1}), 4.0, 1e-12);
    ASSERT_NEAR(r.at<double>({2}), 3.0, 1e-12); // max(2, 3)
}

TEST(minimum_test) {
    auto a = npc::arange(0.0, 5.0, 1.0);
    auto b = npc::arange(5.0, 0.0, -1.0);
    auto r = npc::minimum(a, b);
    ASSERT_NEAR(r.at<double>({0}), 0.0, 1e-12);
    ASSERT_NEAR(r.at<double>({4}), 1.0, 1e-12); // min(4, 1)
}

TEST(mod_test) {
    auto a = npc::full({3}, 10.0);
    auto b = npc::full({3}, 3.0);
    auto r = npc::mod(a, b);
    for (int i = 0; i < 3; i++) ASSERT_NEAR(r.at<double>({i}), 1.0, 1e-12);
}

TEST(hypot_test) {
    auto a = npc::full({3}, 3.0);
    auto b = npc::full({3}, 4.0);
    auto r = npc::hypot(a, b);
    for (int i = 0; i < 3; i++) ASSERT_NEAR(r.at<double>({i}), 5.0, 1e-12);
}

// ── Logical tests ────────────────────────────────────────────

TEST(logical_and_test) {
    auto a = npc::greater(npc::full({3}, 1.0), npc::full({3}, 0.0));
    auto b = npc::greater(npc::full({3}, 0.0), npc::full({3}, 1.0));
    // a = [true,true,true], b = [false,false,false]
    auto r = npc::logical_and(a, b);
    ASSERT_EQ(r.at<bool>({0}), false);
}

TEST(logical_or_test) {
    auto a = npc::greater(npc::full({3}, 1.0), npc::full({3}, 0.0));
    auto b = npc::greater(npc::full({3}, 0.0), npc::full({3}, 1.0));
    auto r = npc::logical_or(a, b);
    ASSERT_EQ(r.at<bool>({0}), true);
}

TEST(logical_not_test) {
    auto a = npc::greater(npc::full({3}, 1.0), npc::full({3}, 0.0));
    auto r = npc::logical_not(a);
    ASSERT_EQ(r.at<bool>({0}), false);
}

// ── isclose test ─────────────────────────────────────────────

TEST(isclose_test) {
    auto a = npc::full({3}, 1.0);
    auto b = npc::full({3}, 1.0000001);
    auto r = npc::isclose(a, b);
    ASSERT_EQ(r.at<bool>({0}), true);

    auto c = npc::full({3}, 2.0);
    auto r2 = npc::isclose(a, c);
    ASSERT_EQ(r2.at<bool>({0}), false);
}

// ── Main ─────────────────────────────────────────────────────

int main() {
    printf("\n=== libnpc++ Math Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
