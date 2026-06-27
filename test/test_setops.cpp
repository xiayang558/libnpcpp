#include <npc/npc.hpp>
#include <cstdio>
#include <cmath>

static int tests_run = 0, tests_passed = 0, tests_failed = 0;
#define TEST(name) void test_##name(); struct R_##name { R_##name() { \
    tests_run++; printf("  RUN %s\n", #name); test_##name(); \
    tests_passed++; printf("  PASS %s\n", #name); }} r_##name; void test_##name()
#define ASSERT_EQ(a,b) do{if((a)!=(b)){printf("  FAIL %s:%d\n",__FILE__,__LINE__); \
    tests_failed++;tests_passed--;return;}}while(0)
#define ASSERT_TRUE(e) do{if(!(e)){printf("  FAIL %s:%d\n",__FILE__,__LINE__); \
    tests_failed++;tests_passed--;return;}}while(0)

TEST(unique_test) {
    auto a = npc::array({1.0, 2.0, 2.0, 3.0, 1.0});
    auto r = npc::unique(a, false, false, true);
    ASSERT_EQ(r.values.size(), 3);
}

TEST(intersect1d_test) {
    auto a = npc::array({1.0, 2.0, 3.0, 4.0});
    auto b = npc::array({3.0, 4.0, 5.0, 6.0});
    auto r = npc::intersect1d(a, b);
    ASSERT_EQ(r.size(), 2);
}

TEST(setdiff1d_test) {
    auto a = npc::array({1.0, 2.0, 3.0, 4.0});
    auto b = npc::array({3.0, 4.0});
    auto r = npc::setdiff1d(a, b);
    ASSERT_EQ(r.size(), 2);
}

TEST(searchsorted_test) {
    auto a = npc::arange(0.0, 10.0, 1.0);
    auto v = npc::array({2.5, 7.5});
    auto r = npc::searchsorted(a, v);
    ASSERT_EQ(r.size(), 2);
}

TEST(digitize_test) {
    auto bins = npc::array({0.0, 5.0, 10.0});
    auto x = npc::array({2.0, 7.0, 12.0});
    auto r = npc::digitize(x, bins);
    ASSERT_EQ(r.size(), 3);
}

int main() {
    printf("\n=== libnpc++ SetOps Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
