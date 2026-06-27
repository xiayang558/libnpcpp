#include <npc/npc.hpp>
#include <cstdio>
#include <cmath>
#include <sstream>

static int tests_run = 0, tests_passed = 0, tests_failed = 0;

#define TEST(name) \
    void test_##name(); \
    struct R_##name { R_##name() { \
        tests_run++; printf("  RUN %s\n", #name); test_##name(); \
        tests_passed++; printf("  PASS %s\n", #name); \
    }} r_##name; \
    void test_##name()

#define ASSERT_NEAR(a,b,eps) do { if(std::fabs((a)-(b))>(eps)){ \
    printf("  FAIL %s:%d: %f vs %f\n",__FILE__,__LINE__,(double)(a),(double)(b)); \
    tests_failed++;tests_passed--;return;}}while(0)
#define ASSERT_EQ(a,b) do{if((a)!=(b)){printf("  FAIL %s:%d\n",__FILE__,__LINE__); \
    tests_failed++;tests_passed--;return;}}while(0)
#define ASSERT_TRUE(e) do{if(!(e)){printf("  FAIL %s:%d\n",__FILE__,__LINE__); \
    tests_failed++;tests_passed--;return;}}while(0)

// ── operator() multi-indexing ──────────────────────────────

TEST(operator_paren_1d) {
    auto a = npc::arange(0.0, 5.0, 1.0);
    ASSERT_NEAR(a(2), 2.0, 1e-12);
}

TEST(operator_paren_2d) {
    auto a = npc::array({{1.0, 2.0}, {3.0, 4.0}});
    ASSERT_NEAR(a(0, 0), 1.0, 1e-12);
    ASSERT_NEAR(a(1, 1), 4.0, 1e-12);
}

// ── operator[] single-indexing ─────────────────────────────

TEST(operator_bracket) {
    auto a = npc::arange(0.0, 5.0, 1.0);
    ASSERT_NEAR(a[0], 0.0, 1e-12);
    ASSERT_NEAR(a[4], 4.0, 1e-12);
    a[0] = 99.0;
    ASSERT_NEAR(a[0], 99.0, 1e-12);
}

// ── operator<< stream output ────────────────────────────────

TEST(stream_output) {
    auto a = npc::ones({3, 3});
    std::ostringstream oss;
    oss << a;
    std::string s = oss.str();
    ASSERT_TRUE(s.find("Array") != std::string::npos);
    ASSERT_TRUE(s.find("ndim=2") != std::string::npos);
}

// ── Typed iterators ────────────────────────────────────────

TEST(iterators) {
    auto a = npc::arange(0.0, 5.0, 1.0);
    double sum = 0.0;
    for (const double* p = a.begin<double>(); p != a.end<double>(); ++p) {
        sum += *p;
    }
    ASSERT_NEAR(sum, 10.0, 1e-12);
}

// ── bool keepdims (stats.hpp) ──────────────────────────────

TEST(keepdims_bool) {
    auto a = npc::ones({3, 3});
    auto r = npc::sum(a, -1, true);   // bool, not int
    ASSERT_EQ(r.scalar<double>(), 9.0);
}

// ── operator/(scalar, Array) ───────────────────────────────

TEST(div_scalar_array) {
    auto a = npc::full({3}, 10.0);
    auto r = 100.0 / a;
    ASSERT_NEAR(r[0], 10.0, 1e-12);
}

// ── sort() alias ───────────────────────────────────────────

TEST(sort_alias) {
    auto a = npc::array({3.0, 1.0, 2.0});
    auto r = npc::sort(a);
    ASSERT_NEAR(r[0], 1.0, 1e-12);
    ASSERT_NEAR(r[2], 3.0, 1e-12);
}

// ── nan stats (nan_utils.hpp) ──────────────────────────────

TEST(nansum_test) {
    auto a = npc::full({5}, 1.0);
    auto r = npc::nansum(a);
    ASSERT_NEAR(r.scalar<double>(), 5.0, 1e-12);
}

TEST(nanprod_test) {
    auto a = npc::full({3}, 2.0);
    auto r = npc::nanprod(a);
    ASSERT_NEAR(r.scalar<double>(), 8.0, 1e-12);
}

// ── swapaxes ───────────────────────────────────────────────

TEST(swapaxes_test) {
    auto a = npc::array({{1.0, 2.0}, {3.0, 4.0}});
    auto r = npc::swapaxes(a, 0, 1);
    ASSERT_EQ(r.shape()[0], 2);
    ASSERT_EQ(r.shape()[1], 2);
    ASSERT_NEAR(r(0, 1), 3.0, 1e-12);
}

// ── Main ────────────────────────────────────────────────────

int main() {
    printf("\n=== libnpc++ Cleanup Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
