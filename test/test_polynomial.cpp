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

TEST(poly_roots) {
    // (x-1)(x-2)(x-3) = x^3 - 6x^2 + 11x - 6
    auto r = npc::array({1.0, 2.0, 3.0});
    auto p = npc::poly(r);
    ASSERT_EQ(p.size(), 4);
}

TEST(roots_poly) {
    auto r = npc::array({1.0, 2.0, 3.0});
    auto p = npc::poly(r);
    auto r2 = npc::roots(p);
    ASSERT_EQ(r2.size(), 3);
}

TEST(polyadd_test) {
    auto a = npc::array({1.0, 2.0, 3.0});
    auto b = npc::array({4.0, 5.0, 6.0});
    auto c = npc::polyadd(a, b);
    ASSERT_EQ(c.size(), 3);
}

TEST(polyval_test) {
    auto p = npc::array({1.0, 2.0, 3.0});
    auto x = npc::array({0.0, 1.0, 2.0});
    auto y = npc::polyval(p, x);
    ASSERT_EQ(y.size(), 3);
}

TEST(polyfit_test) {
    auto x = npc::arange(0.0, 5.0, 1.0);
    auto y = x * 2.0 + 1.0;
    auto p = npc::polyfit(x, y, 1);
    ASSERT_NEAR(p.at<double>({0}), 2.0, 1e-6);
    ASSERT_NEAR(p.at<double>({1}), 1.0, 1e-6);
}

TEST(polyder_test) {
    // derivative of x^2: [1,0,0] -> [2,0]
    auto p = npc::array({1.0, 0.0, 0.0});
    auto dp = npc::polyder(p);
    ASSERT_NEAR(dp.at<double>({0}), 2.0, 1e-6);
}

int main() {
    printf("\n=== libnpc++ Polynomial Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
