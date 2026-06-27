#include <npc/npc.hpp>
#include <cstdio>
#include <cmath>
#include <cstdlib>

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
    printf("  FAIL  %s:%d: ASSERT_EQ\n", __FILE__, __LINE__); \
    tests_failed++; tests_passed--; return; \
}} while(0)

// ── Helper: get element from possibly-2D array ──────────────

static double get_elem(const npc::Array& a, int i) {
    if (a.ndim() == 1) return a.at<double>({i});
    if (a.ndim() == 2) return a.at<double>({i, 0});
    return a.scalar<double>();
}

// ── NPY save/load round-trip ────────────────────────────────

TEST(npy_save_load) {
    const char* fname = "/tmp/npc_test_io.npy";
    auto a = npc::arange(0.0, 10.0, 1.0);
    int ok = npc::save_npy(fname, a);
    ASSERT_EQ(ok, 0);

    auto b = npc::load_npy(fname);
    ASSERT_EQ(b.size(), a.size());
    for (int i = 0; i < a.size(); i++) {
        ASSERT_NEAR(get_elem(b, i), a.at<double>({i}), 1e-12);
    }
    std::remove(fname);
}

// ── savetxt/readtxt round-trip ──────────────────────────────

TEST(txt_save_load) {
    const char* fname = "/tmp/npc_test_io.txt";
    auto a = npc::arange(0.0, 5.0, 1.0);
    int ok = npc::savetxt(fname, a);
    ASSERT_EQ(ok, 0);

    auto b = npc::readtxt(fname);
    ASSERT_EQ(b.size(), a.size());
    // readtxt returns 2D column vector: shape=[n, 1]
    for (int i = 0; i < a.size(); i++) {
        ASSERT_NEAR(b.at<double>({i, 0}), a.at<double>({i}), 1e-12);
    }
    std::remove(fname);
}

// ── genfromtxt ──────────────────────────────────────────────

TEST(genfromtxt_test) {
    const char* fname = "/tmp/npc_test_gft.txt";
    // Write 2D data with comma delimiter so genfromtxt can parse it
    auto a = npc::ones({2, 2});
    a.at<double>({0, 1}) = 2.0;
    a.at<double>({1, 0}) = 3.0;
    a.at<double>({1, 1}) = 4.0;
    npc::savetxt(fname, a, "%g", ",");
    auto b = npc::genfromtxt(fname);
    ASSERT_EQ(b.size(), 4);
    ASSERT_EQ(b.ndim(), 2);
    std::remove(fname);
}

// ── Window functions ────────────────────────────────────────

TEST(bartlett_test) {
    auto w = npc::bartlett(5);
    ASSERT_EQ(w.size(), 5);
}

TEST(hanning_test) {
    auto w = npc::hanning(5);
    ASSERT_EQ(w.size(), 5);
}

TEST(hamming_test) {
    auto w = npc::hamming(5);
    ASSERT_EQ(w.size(), 5);
}

TEST(blackman_test) {
    auto w = npc::blackman(5);
    ASSERT_EQ(w.size(), 5);
}

// ── Main ────────────────────────────────────────────────────

int main() {
    printf("\n=== libnpc++ I/O & Window Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
