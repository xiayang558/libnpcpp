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

#define ASSERT_TRUE(e) do { if (!(e)) { \
    printf("  FAIL  %s:%d: ASSERT_TRUE\n", __FILE__, __LINE__); \
    tests_failed++; tests_passed--; return; \
}} while(0)

// ── Reshape ──────────────────────────────────────────────────

TEST(reshape_test) {
    auto a = npc::arange(0.0, 6.0, 1.0);
    auto r = npc::reshape(a, {2, 3});
    ASSERT_EQ(r.ndim(), 2);
    ASSERT_EQ(r.shape()[0], 2);
    ASSERT_EQ(r.shape()[1], 3);
    ASSERT_NEAR(r.at<double>({0, 0}), 0.0, 1e-12);
    ASSERT_NEAR(r.at<double>({1, 2}), 5.0, 1e-12);
}

// ── Transpose ────────────────────────────────────────────────

TEST(transpose_test) {
    auto a = npc::array({{1.0, 2.0}, {3.0, 4.0}});
    auto r = npc::transpose(a);
    ASSERT_NEAR(r.at<double>({0, 1}), 3.0, 1e-12);
    ASSERT_NEAR(r.at<double>({1, 0}), 2.0, 1e-12);
}

// ── Flatten / ravel ──────────────────────────────────────────

TEST(flatten_test) {
    auto a = npc::array({{1.0, 2.0}, {3.0, 4.0}});
    auto r = npc::flatten(a);
    ASSERT_EQ(r.ndim(), 1);
    ASSERT_EQ(r.size(), 4);
    ASSERT_NEAR(r.at<double>({0}), 1.0, 1e-12);
    ASSERT_NEAR(r.at<double>({3}), 4.0, 1e-12);
}

// ── Squeeze / expand_dims ────────────────────────────────────

TEST(squeeze_test) {
    auto a = npc::zeros({1, 3, 1});
    auto r = npc::squeeze(a);
    ASSERT_EQ(r.ndim(), 1);
    ASSERT_EQ(r.shape()[0], 3);
}

TEST(expand_dims_test) {
    auto a = npc::arange(0.0, 3.0, 1.0);
    auto r = npc::expand_dims(a, 0);
    ASSERT_EQ(r.ndim(), 2);
    ASSERT_EQ(r.shape()[0], 1);
    ASSERT_EQ(r.shape()[1], 3);
}

// ── Concatenate ──────────────────────────────────────────────

TEST(concatenate_test) {
    auto a = npc::arange(0.0, 3.0, 1.0);
    auto b = npc::arange(3.0, 6.0, 1.0);
    auto r = npc::concatenate(a, b);
    ASSERT_EQ(r.size(), 6);
    ASSERT_NEAR(r.at<double>({0}), 0.0, 1e-12);
    ASSERT_NEAR(r.at<double>({5}), 5.0, 1e-12);
}

// ── Stack ────────────────────────────────────────────────────

TEST(stack_test) {
    auto a = npc::arange(0.0, 3.0, 1.0);
    auto b = npc::arange(3.0, 6.0, 1.0);
    auto r = npc::stack({a, b});
    ASSERT_EQ(r.ndim(), 2);
    ASSERT_EQ(r.shape()[0], 2);
    ASSERT_EQ(r.shape()[1], 3);
    ASSERT_NEAR(r.at<double>({0, 0}), 0.0, 1e-12);
    ASSERT_NEAR(r.at<double>({1, 2}), 5.0, 1e-12);
}

TEST(hstack_test) {
    auto a = npc::arange(0.0, 3.0, 1.0);
    auto b = npc::arange(3.0, 6.0, 1.0);
    auto r = npc::hstack({a, b});
    ASSERT_EQ(r.size(), 6);
}

TEST(vstack_test) {
    auto a = npc::arange(0.0, 3.0, 1.0);
    auto b = npc::arange(3.0, 6.0, 1.0);
    auto r = npc::vstack({a, b});
    ASSERT_EQ(r.ndim(), 2);
    ASSERT_EQ(r.shape()[0], 2);
    ASSERT_EQ(r.shape()[1], 3);
}

// ── Flip / fliplr / flipud ───────────────────────────────────

TEST(flip_test) {
    auto a = npc::arange(0.0, 5.0, 1.0);
    auto r = npc::flip(a);
    ASSERT_NEAR(r.at<double>({0}), 4.0, 1e-12);
    ASSERT_NEAR(r.at<double>({4}), 0.0, 1e-12);
}

TEST(fliplr_test) {
    auto a = npc::array({{1.0, 2.0}, {3.0, 4.0}});
    auto r = npc::fliplr(a);
    ASSERT_NEAR(r.at<double>({0, 0}), 2.0, 1e-12);
    ASSERT_NEAR(r.at<double>({0, 1}), 1.0, 1e-12);
}

// ── Roll ─────────────────────────────────────────────────────

TEST(roll_test) {
    auto a = npc::arange(1.0, 6.0, 1.0);
    auto r = npc::roll(a, 2);
    ASSERT_NEAR(r.at<double>({0}), 4.0, 1e-12);
    ASSERT_NEAR(r.at<double>({1}), 5.0, 1e-12);
    ASSERT_NEAR(r.at<double>({2}), 1.0, 1e-12);
}

// ── Tile ─────────────────────────────────────────────────────

TEST(tile_test) {
    auto a = npc::arange(0.0, 3.0, 1.0);
    auto r = npc::tile(a, {2});
    ASSERT_EQ(r.size(), 6);
    ASSERT_NEAR(r.at<double>({0}), 0.0, 1e-12);
    ASSERT_NEAR(r.at<double>({3}), 0.0, 1e-12);
}

// ── Diag ─────────────────────────────────────────────────────

TEST(diag_test) {
    auto a = npc::arange(1.0, 4.0, 1.0);
    auto r = npc::diag(a);
    ASSERT_EQ(r.ndim(), 2);
    ASSERT_NEAR(r.at<double>({0, 0}), 1.0, 1e-12);
    ASSERT_NEAR(r.at<double>({2, 2}), 3.0, 1e-12);
    ASSERT_NEAR(r.at<double>({0, 1}), 0.0, 1e-12);
}

// ── atleast helpers ──────────────────────────────────────────

TEST(atleast_2d_test) {
    auto a = npc::arange(0.0, 3.0, 1.0);
    auto r = npc::atleast_2d(a);
    ASSERT_EQ(r.ndim(), 2);
    // atleast_2d adds a trailing axis for 1-D input → shape=[3, 1]
    ASSERT_EQ(r.shape()[0], 3);
    ASSERT_EQ(r.shape()[1], 1);
}

// ── Moveaxis ─────────────────────────────────────────────────

TEST(moveaxis_test) {
    auto a = npc::arange(0.0, 6.0, 1.0);
    auto b = npc::reshape(a, {2, 3});
    auto r = npc::moveaxis(b, 0, 1);
    ASSERT_EQ(r.shape()[0], 3);
    ASSERT_EQ(r.shape()[1], 2);
}

// ── Broadcast_to ─────────────────────────────────────────────

TEST(broadcast_to_test) {
    auto a = npc::arange(0.0, 3.0, 1.0);
    auto r = npc::broadcast_to(a, {2, 3});
    ASSERT_EQ(r.shape()[0], 2);
    ASSERT_EQ(r.shape()[1], 3);
    ASSERT_NEAR(r.at<double>({1, 0}), 0.0, 1e-12);
}

// ── Split ────────────────────────────────────────────────────

TEST(split_test) {
    auto a = npc::arange(0.0, 10.0, 1.0);
    // Split at positions [3, 7] → three pieces: [0,1,2], [3,4,5,6], [7,8,9]
    auto indices = npc::array({3, 7});
    auto sec = npc::astype(indices, npc::DType::Int32);
    auto pieces = npc::split(a, sec, 0);
    // Should produce 3 pieces
    ASSERT_EQ(static_cast<int>(pieces.size()), 3);
}

// ── Main ─────────────────────────────────────────────────────

int main() {
    printf("\n=== libnpc++ Shape Test Suite ===\n\n");
    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return (tests_failed > 0) ? 1 : 0;
}
