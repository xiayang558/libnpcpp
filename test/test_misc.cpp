#include <npc/npc.hpp>
#include <cstdio>
#include <cmath>

static int tests_run = 0, tests_passed = 0, tests_failed = 0;
#define TEST(name) void test_##name(); struct R_##name { R_##name() { \
    tests_run++; printf("  RUN %s\n", #name); test_##name(); \
    tests_passed++; printf("  PASS %s\n", #name); }} r_##name; void test_##name()
#define ASSERT_NEAR(a,b,eps) do { if(std::fabs((a)-(b))>(eps)){ \
    printf("  FAIL %s:%d: %f vs %f\n",__FILE__,__LINE__,(double)(a),(double)(b)); \
    tests_failed++;tests_passed--;return;}}while(0)
#define ASSERT_EQ(a,b) do{if((a)!=(b)){printf("  FAIL %s:%d\n",__FILE__,__LINE__); \
    tests_failed++;tests_passed--;return;}}while(0)
#define ASSERT_TRUE(e) do{if(!(e)){printf("  FAIL %s:%d\n",__FILE__,__LINE__); \
    tests_failed++;tests_passed--;return;}}while(0)

// ── logspace / geomspace ────────────────────────────────────

TEST(logspace_test) {
    auto r = npc::logspace(0.0, 2.0, 3);
    ASSERT_EQ(r.size(), 3);
    ASSERT_NEAR(r.at<double>({0}), 1.0, 1e-6);
    ASSERT_NEAR(r.at<double>({2}), 100.0, 1e-6);
}

TEST(geomspace_test) {
    auto r = npc::geomspace(1.0, 8.0, 4);
    ASSERT_EQ(r.size(), 4);
}

// ── sort / argsort ──────────────────────────────────────────

TEST(msort_test) {
    auto a = npc::array({3.0, 1.0, 2.0});
    auto r = npc::msort(a);
    ASSERT_NEAR(r.at<double>({0}), 1.0, 1e-12);
    ASSERT_NEAR(r.at<double>({2}), 3.0, 1e-12);
}

TEST(argsort_test) {
    auto a = npc::array({3.0, 1.0, 2.0});
    auto r = npc::argsort(a);
    ASSERT_EQ(r.scalar<int64_t>(), 1); // index of smallest
}

// ── trim_zeros ──────────────────────────────────────────────

TEST(trim_zeros_test) {
    auto a = npc::array({0.0, 0.0, 1.0, 2.0, 0.0});
    auto r = npc::trim_zeros(a);
    ASSERT_EQ(r.size(), 2);
}

// ── nan utils ────────────────────────────────────────────────

TEST(isnan_test) {
    double nan_val = NAN;
    auto a = npc::full({3}, nan_val);
    auto r = npc::isnan(a);
    ASSERT_EQ(r.at<bool>({0}), true);
}

TEST(isinf_test) {
    double inf_val = INFINITY;
    auto a = npc::full({3}, inf_val);
    auto r = npc::isinf(a);
    ASSERT_EQ(r.at<bool>({0}), true);
}

TEST(isfinite_test) {
    auto a = npc::full({3}, 1.0);
    auto r = npc::isfinite(a);
    ASSERT_EQ(r.at<bool>({0}), true);
}

// ── unwrap ──────────────────────────────────────────────────

TEST(unwrap_test) {
    auto a = npc::array({0.0, M_PI, -M_PI + 0.1, M_PI});
    auto r = npc::unwrap(a);
    ASSERT_EQ(r.size(), 4);
}

int main() {
    printf("\n=== libnpc++ Misc Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
