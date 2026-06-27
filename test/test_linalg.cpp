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

#define ASSERT_TRUE(e) do { if (!(e)) { \
    printf("  FAIL  %s:%d: ASSERT_TRUE(%s)\n", __FILE__, __LINE__, #e); \
    tests_failed++; tests_passed--; return; \
}} while(0)

// ── Dot product ──────────────────────────────────────────────

TEST(dot_vector) {
    auto a = npc::ones({3});
    auto b = npc::full({3}, 2.0);
    auto r = npc::dot(a, b);
    ASSERT_NEAR(r.scalar<double>(), 6.0, 1e-12);
}

TEST(dot_matrix) {
    auto a = npc::ones({2, 3});
    auto b = npc::full({3, 2}, 2.0);
    auto r = npc::dot(a, b);
    // 1*2*3 = 6 per element for 2x2 result
    ASSERT_NEAR(r.at<double>({0, 0}), 6.0, 1e-12);
    ASSERT_NEAR(r.at<double>({1, 1}), 6.0, 1e-12);
}

// ── Matmul ───────────────────────────────────────────────────

TEST(matmul_matrix) {
    auto a = npc::ones({2, 3});
    auto b = npc::full({3, 2}, 2.0);
    auto r = npc::matmul(a, b);
    ASSERT_NEAR(r.at<double>({0, 0}), 6.0, 1e-12);
}

// ── Trace ────────────────────────────────────────────────────

TEST(trace_test) {
    auto a = npc::eye(3);
    auto r = npc::trace(a);
    ASSERT_NEAR(r.scalar<double>(), 3.0, 1e-12);
}

// ── Det ──────────────────────────────────────────────────────

TEST(det_test) {
    auto a = npc::eye(3);
    auto r = npc::det(a);
    ASSERT_NEAR(r.scalar<double>(), 1.0, 1e-12);
}

TEST(det_2x2) {
    // [[2,1],[1,2]] → det = 3
    auto a = npc::array({{2.0, 1.0}, {1.0, 2.0}});
    auto r = npc::det(a);
    ASSERT_NEAR(r.scalar<double>(), 3.0, 1e-6);
}

// ── Inv ──────────────────────────────────────────────────────

TEST(inv_test) {
    // [[2,1],[1,2]] → inv = [[2/3, -1/3],[-1/3, 2/3]]
    auto a = npc::array({{2.0, 1.0}, {1.0, 2.0}});
    auto r = npc::inv(a);
    ASSERT_NEAR(r.at<double>({0, 0}), 2.0 / 3.0, 1e-6);
    ASSERT_NEAR(r.at<double>({1, 1}), 2.0 / 3.0, 1e-6);
    ASSERT_NEAR(r.at<double>({0, 1}), -1.0 / 3.0, 1e-6);
}

// ── Cholesky ─────────────────────────────────────────────────

TEST(cholesky_test) {
    // [[4,2],[2,3]] → L = [[2,0],[1,sqrt(2)]]
    auto a = npc::array({{4.0, 2.0}, {2.0, 3.0}});
    auto L = npc::cholesky(a);
    ASSERT_NEAR(L.at<double>({0, 0}), 2.0, 1e-6);
    ASSERT_NEAR(L.at<double>({0, 1}), 0.0, 1e-6);
    ASSERT_NEAR(L.at<double>({1, 0}), 1.0, 1e-6);
    ASSERT_NEAR(L.at<double>({1, 1}), std::sqrt(2.0), 1e-6);
}

// ── Inner / outer / cross ────────────────────────────────────

TEST(inner_test) {
    auto a = npc::ones({3});
    auto b = npc::full({3}, 2.0);
    auto r = npc::inner(a, b);
    ASSERT_NEAR(r.scalar<double>(), 6.0, 1e-12);
}

TEST(outer_test) {
    auto a = npc::arange(1.0, 4.0, 1.0);
    auto b = npc::arange(1.0, 4.0, 1.0);
    auto r = npc::outer(a, b);
    ASSERT_NEAR(r.at<double>({0, 0}), 1.0, 1e-12);
    ASSERT_NEAR(r.at<double>({2, 2}), 9.0, 1e-12);
}

// ── SVD ──────────────────────────────────────────────────────

TEST(svd_test) {
    // [[2,1],[1,2]] — a simple 2x2 matrix
    auto a = npc::array({{2.0, 1.0}, {1.0, 2.0}});
    auto r = npc::svd(a, true);
    ASSERT_TRUE(!r.U.is_null());
    ASSERT_TRUE(!r.s.is_null());
    ASSERT_TRUE(!r.Vt.is_null());
    ASSERT_NEAR(r.s.at<double>({0}), 3.0, 1e-6); // σ1 = 3
    ASSERT_NEAR(r.s.at<double>({1}), 1.0, 1e-6); // σ2 = 1
}

// ── QR ───────────────────────────────────────────────────────

TEST(qr_test) {
    auto a = npc::array({{1.0, 0.0}, {0.0, 1.0}});
    auto r = npc::qr(a);
    ASSERT_TRUE(!r.Q.is_null());
    ASSERT_TRUE(!r.R.is_null());
}

// ── LU ───────────────────────────────────────────────────────

TEST(lu_test) {
    auto a = npc::array({{2.0, 1.0}, {1.0, 2.0}});
    auto r = npc::lu(a);
    ASSERT_TRUE(!r.P.is_null());
    ASSERT_TRUE(!r.L.is_null());
    ASSERT_TRUE(!r.U.is_null());
}

// ── Eig ──────────────────────────────────────────────────────

TEST(eig_test) {
    auto a = npc::array({{2.0, 1.0}, {1.0, 2.0}});
    auto r = npc::eig(a);
    ASSERT_TRUE(!r.eigenvalues.is_null());
    ASSERT_TRUE(!r.eigenvectors.is_null());
}

// ── Pinv ─────────────────────────────────────────────────────

TEST(pinv_test) {
    auto a = npc::array({{1.0, 2.0}, {3.0, 4.0}});
    auto r = npc::pinv(a);
    ASSERT_TRUE(!r.is_null());
}

// ── Main ─────────────────────────────────────────────────────

int main() {
    printf("\n=== libnpc++ Linalg Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
