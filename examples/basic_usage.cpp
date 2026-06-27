#include <npc/npc.hpp>
#include <cstdio>

int main() {
    printf("=== libnpc++ Basic Usage Example ===\n\n");

    // ── Array creation ──────────────────────────────────
    printf("Creating arrays:\n");
    auto a = npc::ones({3, 3});
    printf("a = ones(3, 3):\n");
    a.print();

    auto b = npc::full({3, 3}, 2.0);
    printf("b = full(3, 3, 2.0):\n");
    b.print();

    // ── Arithmetic operators ────────────────────────────
    printf("Arithmetic:\n");
    auto c = a + b;
    printf("c = a + b:\n");
    c.print();

    auto d = c * 3.0;
    printf("d = c * 3.0:\n");
    d.print();

    // ── Math functions ──────────────────────────────────
    printf("Math functions:\n");
    auto e = npc::sqrt(npc::full({5}, 16.0));
    printf("sqrt(16) x 5:\n");
    e.print();

    auto f = npc::exp(npc::full({5}, 0.0));
    printf("exp(0) x 5:\n");
    f.print();

    // ── Eye / identity ──────────────────────────────────
    printf("Matrix operations:\n");
    auto I = npc::eye(3);
    printf("eye(3):\n");
    I.print();

    // ── arange ──────────────────────────────────────────
    printf("arange:\n");
    auto r = npc::arange(0.0, 10.0, 2.0);
    printf("arange(0, 10, 2):\n");
    r.print();

    // ── Comparison ──────────────────────────────────────
    printf("Comparison:\n");
    auto eq = npc::equal(c, a + b);
    printf("equal(c, a + b):\n");
    eq.print();

    // ── Statistics ──────────────────────────────────────
    printf("Statistics:\n");
    auto data = npc::arange(1.0, 6.0, 1.0); // [1, 2, 3, 4, 5]
    printf("data = arange(1, 6, 1):\n");
    data.print();

    printf("sum(data) = ");
    npc::sum(data).print();

    printf("mean(data) = ");
    npc::mean(data).print();

    printf("stddev(data) = ");
    npc::stddev(data).print();

    printf("cumsum(data):\n");
    npc::cumsum(data).print();

    // ── Maximum / minimum ───────────────────────────────
    printf("Element-wise max:\n");
    auto x = npc::arange(0.0, 5.0, 1.0);
    auto y = npc::arange(5.0, 0.0, -1.0);
    printf("maximum(arange(0,5), arange(5,0,-1)):\n");
    npc::maximum(x, y).print();

    // ── Copy / move ─────────────────────────────────────
    printf("Copy:\n");
    auto copy_a = npc::copy(a);
    printf("copy(a):\n");
    copy_a.print();

    printf("\n=== All examples completed successfully ===\n");
    return 0;
}
