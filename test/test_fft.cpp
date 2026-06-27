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
    printf("  FAIL  %s:%d: ASSERT_EQ\n", __FILE__, __LINE__); \
    tests_failed++; tests_passed--; return; \
}} while(0)

// ── FFT/IFFT round-trip ─────────────────────────────────────

TEST(fft_ifft_roundtrip) {
    auto a = npc::arange(0.0, 8.0, 1.0);
    auto freq = npc::fft(a);
    auto back = npc::ifft(freq);
    ASSERT_EQ(a.size(), back.size());
    for (int i = 0; i < a.size(); i++) {
        ASSERT_NEAR(back.at<double>({i}), a.at<double>({i}), 1e-6);
    }
}

TEST(fft_rfft) {
    auto a = npc::arange(0.0, 8.0, 1.0);
    auto r = npc::rfft(a);
    ASSERT_EQ(r.size(), 5); // n/2 + 1 = 5 for n=8
}

TEST(fft_fft2) {
    auto a = npc::array({{1.0, 2.0}, {3.0, 4.0}});
    auto r = npc::fft2(a);
    ASSERT_EQ(r.size(), 4);
}

// ── FFT shift ───────────────────────────────────────────────

TEST(fftshift_test) {
    auto a = npc::arange(0.0, 8.0, 1.0);
    auto r = npc::fftshift(a);
    ASSERT_EQ(r.size(), 8);
    ASSERT_NEAR(r.at<double>({0}), 4.0, 1e-12);
    ASSERT_NEAR(r.at<double>({4}), 0.0, 1e-12);
}

// ── Real FFT variants ───────────────────────────────────────

TEST(fft_rfft2_test) {
    auto a = npc::array({{1.0, 2.0}, {3.0, 4.0}});
    auto r = npc::rfft2(a);
    ASSERT_EQ(r.size(), 4);
}

TEST(fft_rfftn_test) {
    auto a = npc::array({{1.0, 2.0}, {3.0, 4.0}});
    auto r = npc::rfftn(a);
    ASSERT_EQ(r.size(), 4);
}

// ── FFtfreq ─────────────────────────────────────────────────

TEST(fftfreq_test) {
    auto r = npc::fftfreq(8);
    ASSERT_EQ(r.size(), 8);
}

TEST(rfftfreq_test) {
    auto r = npc::rfftfreq(8);
    ASSERT_EQ(r.size(), 5);
}

// ── irfft ───────────────────────────────────────────────────

TEST(irfft_test) {
    auto a = npc::arange(0.0, 8.0, 1.0);
    auto freq = npc::rfft(a);
    auto back = npc::irfft(freq, 8);
    ASSERT_EQ(back.size(), 8);
}

// ── Main ────────────────────────────────────────────────────

int main() {
    printf("\n=== libnpc++ FFT Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
