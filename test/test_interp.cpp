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

TEST(interp_test) {
    auto xp = npc::arange(0.0, 5.0, 1.0);
    auto fp = xp * 2.0;
    auto x = npc::array({1.5, 3.5});
    auto y = npc::interp(x, xp, fp);
    ASSERT_EQ(y.size(), 2);
    ASSERT_NEAR(y.at<double>({0}), 3.0, 1e-6);
    ASSERT_NEAR(y.at<double>({1}), 7.0, 1e-6);
}

TEST(diff_test) {
    auto a = npc::arange(0.0, 5.0, 1.0);
    auto d = npc::diff(a);
    ASSERT_EQ(d.size(), 4);
    ASSERT_NEAR(d.at<double>({0}), 1.0, 1e-12);
}

TEST(ediff1d_test) {
    auto y = npc::arange(1.0, 5.0, 1.0);
    auto d = npc::ediff1d(y);
    ASSERT_EQ(d.size(), 3);
    ASSERT_NEAR(d.at<double>({0}), 1.0, 1e-12);
}

TEST(convolve_test) {
    auto a = npc::ones({3});
    auto v = npc::ones({3});
    auto c = npc::convolve(a, v);
    ASSERT_EQ(c.size(), 5);
}

int main() {
    printf("\n=== libnpc++ Interp Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
