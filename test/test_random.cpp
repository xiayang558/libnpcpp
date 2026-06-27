#include <npc/npc.hpp>
#include <cstdio>
#include <cmath>

static int tests_run = 0, tests_passed = 0, tests_failed = 0;

#define TEST(name) \
    void test_##name(); \
    struct R_##name { R_##name() { \
        tests_run++; printf("  RUN   %s\n", #name); test_##name(); \
        tests_passed++; printf("  PASS  %s\n", #name); \
    }} r_##name; \
    void test_##name()

#define ASSERT_NEAR(a, b, eps) do { \
    if (std::fabs((a) - (b)) > (eps)) { \
        printf("  FAIL  %s:%d: %f vs %f\n", __FILE__, __LINE__, (double)(a), (double)(b)); \
        tests_failed++; tests_passed--; return; \
    } \
} while(0)

#define ASSERT_EQ(a, b) do { if ((a) != (b)) { \
    printf("  FAIL  %s:%d: ASSERT_EQ(%s, %s)\n", __FILE__, __LINE__, #a, #b); \
    tests_failed++; tests_passed--; return; \
}} while(0)

#define ASSERT_TRUE(e) do { if (!(e)) { \
    printf("  FAIL  %s:%d: ASSERT_TRUE(%s)\n", __FILE__, __LINE__, #e); \
    tests_failed++; tests_passed--; return; \
}} while(0)

// ── RNG engine ──────────────────────────────────────────────

TEST(seed_reproducibility) {
    npc::random_seed(42);
    auto a = npc::rand({5});
    npc::random_seed(42);
    auto b = npc::rand({5});
    for (int i = 0; i < 5; i++) {
        ASSERT_NEAR(a.at<double>({i}), b.at<double>({i}), 1e-12);
    }
}

// ── Uniform ─────────────────────────────────────────────────

TEST(rand_sample) {
    auto r = npc::random_sample({5});
    ASSERT_EQ(r.size(), 5);
    for (int i = 0; i < 5; i++) {
        double v = r.at<double>({i});
        ASSERT_TRUE(v >= 0.0 && v < 1.0);
    }
}

TEST(randn_test) {
    auto r = npc::randn({10});
    ASSERT_EQ(r.size(), 10);
}

TEST(randint_test) {
    auto r = npc::randint(0, 100, {10}, npc::DType::Int64);
    ASSERT_EQ(r.size(), 10);
}

TEST(uniform_test) {
    auto r = npc::uniform(10.0, 20.0, {100});
    ASSERT_EQ(r.size(), 100);
    // Check bounds
    for (int i = 0; i < 100; i++) {
        double v = r.at<double>({i});
        ASSERT_TRUE(v >= 10.0 && v < 20.0);
    }
}

// ── Normal ──────────────────────────────────────────────────

TEST(normal_test) {
    auto r = npc::normal(0.0, 1.0, {100});
    ASSERT_EQ(r.size(), 100);
}

// ── Exponential ─────────────────────────────────────────────

TEST(exponential_test) {
    auto r = npc::exponential(1.0, {50});
    ASSERT_EQ(r.size(), 50);
}

// ── Standard normal val (scalar) ────────────────────────────

TEST(standard_normal_val_test) {
    double v = npc::standard_normal_val();
    (void)v; // just check it doesn't crash
    ASSERT_TRUE(true);
}

// ── Permutation / shuffle ───────────────────────────────────

TEST(permutation_test) {
    auto a = npc::arange(0.0, 10.0, 1.0);
    auto r = npc::permutation(a);
    ASSERT_EQ(r.size(), 10);
}

TEST(shuffle_test) {
    auto a = npc::arange(0.0, 10.0, 1.0);
    npc::shuffle(a);
    ASSERT_EQ(a.size(), 10);
}

// ── Choice ──────────────────────────────────────────────────

TEST(choice_test) {
    auto a = npc::arange(0.0, 10.0, 1.0);
    auto r = npc::choice(a, 5);
    ASSERT_EQ(r.size(), 5);
}

// ── Beta / Gamma ────────────────────────────────────────────

TEST(beta_test) {
    auto r = npc::beta(2.0, 5.0, {10});
    ASSERT_EQ(r.size(), 10);
}

// ── Poisson ─────────────────────────────────────────────────

TEST(poisson_test) {
    npc::random_seed(1234);
    auto r = npc::poisson(5.0, {20});
    ASSERT_EQ(r.size(), 20);
}

// ── Dirichlet ───────────────────────────────────────────────

TEST(dirichlet_test) {
    auto alpha = npc::full({3}, 1.0);
    auto r = npc::dirichlet(alpha, {5});
    ASSERT_EQ(r.ndim(), 2);
    ASSERT_EQ(r.shape()[0], 5);
    ASSERT_EQ(r.shape()[1], 3);
}

// ── Random bytes ────────────────────────────────────────────

TEST(random_bytes_test) {
    auto r = npc::random_bytes(16);
    ASSERT_EQ(r.size(), 16);
}

// ── Main ────────────────────────────────────────────────────

int main() {
    printf("\n=== libnpc++ Random Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
