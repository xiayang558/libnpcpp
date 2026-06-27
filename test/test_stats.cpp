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

// ── Sum ──────────────────────────────────────────────────────

TEST(sum_all) {
    auto a = npc::ones({3, 3});
    auto r = npc::sum(a);
    ASSERT_NEAR(r.scalar<double>(), 9.0, 1e-12);
}

TEST(sum_1d) {
    auto a = npc::arange(0.0, 6.0, 1.0); // [0,1,2,3,4,5], sum = 15
    auto r = npc::sum(a);
    ASSERT_NEAR(r.scalar<double>(), 15.0, 1e-12);
}

// ── Mean ─────────────────────────────────────────────────────

TEST(mean_test) {
    auto a = npc::arange(1.0, 6.0, 1.0); // [1,2,3,4,5]
    auto r = npc::mean(a);
    ASSERT_NEAR(r.scalar<double>(), 3.0, 1e-12);
}

// ── Min / Max ────────────────────────────────────────────────

TEST(min_test) {
    auto a = npc::arange(5.0, 10.0, 1.0); // [5,6,7,8,9]
    auto r = npc::min(a);
    ASSERT_NEAR(r.scalar<double>(), 5.0, 1e-12);
}

TEST(max_test) {
    auto a = npc::arange(5.0, 10.0, 1.0);
    auto r = npc::max(a);
    ASSERT_NEAR(r.scalar<double>(), 9.0, 1e-12);
}

// ── Stddev ───────────────────────────────────────────────────

TEST(stddev_test) {
    auto a = npc::full({5}, 7.0);
    auto r = npc::stddev(a);
    ASSERT_NEAR(r.scalar<double>(), 0.0, 1e-12);

    auto b = npc::arange(0.0, 5.0, 1.0); // [0,1,2,3,4]; mean=2
    auto r2 = npc::stddev(b);
    // population std = sqrt((4+1+0+1+4)/5) = sqrt(2)
    ASSERT_NEAR(r2.scalar<double>(), std::sqrt(2.0), 1e-6);
}

// ── Variance ─────────────────────────────────────────────────

TEST(var_test) {
    auto a = npc::arange(0.0, 5.0, 1.0); // [0,1,2,3,4]
    auto r = npc::var(a);
    ASSERT_NEAR(r.scalar<double>(), 2.0, 1e-6);
}

// ── Prod ─────────────────────────────────────────────────────

TEST(prod_test) {
    auto a = npc::arange(1.0, 6.0, 1.0); // [1,2,3,4,5]
    auto r = npc::prod(a);
    ASSERT_NEAR(r.scalar<double>(), 120.0, 1e-12);
}

// ── Cumsum ───────────────────────────────────────────────────

TEST(cumsum_test) {
    auto a = npc::ones({5});
    auto r = npc::cumsum(a);
    // cumsum returns same shape — 1D with 5 elements
    ASSERT_NEAR(r.at<double>({0}), 1.0, 1e-12);
    ASSERT_NEAR(r.at<double>({4}), 5.0, 1e-12);
}

// ── Median ───────────────────────────────────────────────────

TEST(median_test) {
    auto a = npc::arange(1.0, 6.0, 1.0); // [1,2,3,4,5]
    auto r = npc::median(a);
    ASSERT_NEAR(r.scalar<double>(), 3.0, 1e-12);
}

// ── Argmax / Argmin ──────────────────────────────────────────

TEST(argmax_test) {
    auto a = npc::arange(10.0, 20.0, 1.0);
    auto r = npc::argmax(a);
    // argmax returns INT64 index
    ASSERT_NEAR((double)r.scalar<int64_t>(), 9.0, 1e-12);
}

TEST(argmin_test) {
    auto a = npc::arange(10.0, 20.0, 1.0);
    auto r = npc::argmin(a);
    ASSERT_NEAR((double)r.scalar<int64_t>(), 0.0, 1e-12);
}

// ── Count nonzero ────────────────────────────────────────────

TEST(count_nonzero_test) {
    auto a = npc::ones({10});
    auto r = npc::count_nonzero(a);
    ASSERT_NEAR((double)r.scalar<int64_t>(), 10.0, 1e-12);
}

// ── Ptp ──────────────────────────────────────────────────────

TEST(ptp_test) {
    auto a = npc::arange(5.0, 20.0, 1.0); // [5..19], ptp = 14
    auto r = npc::ptp(a);
    ASSERT_NEAR(r.scalar<double>(), 14.0, 1e-12);
}

// ── All / Any ────────────────────────────────────────────────

TEST(all_test) {
    auto a = npc::greater(npc::full({3}, 1.0), npc::full({3}, 0.0));
    // a = [true,true,true]
    auto r = npc::all(a);
    ASSERT_EQ(r.scalar<bool>(), true);

    a.at<bool>({1}) = false;
    auto r2 = npc::all(a);
    ASSERT_EQ(r2.scalar<bool>(), false);
}

TEST(any_test) {
    auto a = npc::greater(npc::full({3}, 0.0), npc::full({3}, 1.0));
    // a = [false,false,false]
    auto r = npc::any(a);
    ASSERT_EQ(r.scalar<bool>(), false);

    a.at<bool>({1}) = true;
    auto r2 = npc::any(a);
    ASSERT_EQ(r2.scalar<bool>(), true);
}

// ── Main ─────────────────────────────────────────────────────

int main() {
    printf("\n=== libnpc++ Stats Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
