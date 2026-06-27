#pragma once

/// Random number generation wrappers for libnpc.
/// Uses xoshiro256** PRNG seeded via splitmix64.
/// All distribution functions produce a new Array with the given shape.
/// An empty shape (or no shape argument) produces a scalar array (size 1).

#include "npc/array.hpp"
#include <vector>

namespace npc {

namespace detail {
    inline std::vector<int> to_shape_vec(std::initializer_list<int> shape) {
        return std::vector<int>(shape);
    }
}

// ── RNG engine control ────────────────────────────────────────

inline void random_seed(uint64_t seed) { ::random_seed(seed); }
inline uint64_t random_uint64()        { return ::random_uint64(); }
inline double random_double()          { return ::random_double(); }

// ── Helper: convert init_list shape to (data, ndim) ─────────
// Returns shape={} / ndim=0 for scalar output.

#define NPCPP_SHAPE(sh) \
    std::vector<int> _s = detail::to_shape_vec(sh); \
    int _nd = static_cast<int>(_s.size()); \
    int* _sp = _s.empty() ? nullptr : _s.data()

// ── Uniform generators ───────────────────────────────────────

inline Array random_sample(std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_sample(_sp, _nd));
}
inline Array rand(std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_rand(_sp, _nd));
}
inline Array randint(int low, int high, std::initializer_list<int> shape = {},
                     DType dtype = DType::Int64) {
    NPCPP_SHAPE(shape);
    return Array(::random_randint(low, high, _sp, _nd, static_cast<DataType>(dtype)));
}
inline Array uniform(double low, double high, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_uniform(low, high, _sp, _nd));
}

// ── Normal distribution ──────────────────────────────────────

inline double standard_normal_val() { return ::random_standard_normal_val(); }
inline Array standard_normal(std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_standard_normal(_sp, _nd));
}
inline Array randn(std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_randn(_sp, _nd));
}
inline Array normal(double loc, double scale, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_normal(loc, scale, _sp, _nd));
}

// ── Exponential ──────────────────────────────────────────────

inline double standard_exponential_val() { return ::random_standard_exponential_val(); }
inline Array standard_exponential(std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_standard_exponential(_sp, _nd));
}
inline Array exponential(double scale, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_exponential(scale, _sp, _nd));
}

// ── Gamma family ─────────────────────────────────────────────

inline double standard_gamma_val(double shape) { return ::random_standard_gamma_val(shape); }
inline Array standard_gamma(double shape, std::initializer_list<int> out_shape = {}) {
    NPCPP_SHAPE(out_shape);
    return Array(::random_standard_gamma(shape, _sp, _nd));
}
inline Array gamma(double shape, double scale, std::initializer_list<int> out_shape = {}) {
    NPCPP_SHAPE(out_shape);
    return Array(::random_gamma(shape, scale, _sp, _nd));
}
inline Array chisquare(double df, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_chisquare(df, _sp, _nd));
}
inline Array beta(double a, double b, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_beta(a, b, _sp, _nd));
}
inline Array standard_t(double df, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_standard_t(df, _sp, _nd));
}
inline Array f(double dfn, double dfd, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_f(dfn, dfd, _sp, _nd));
}

// ── Continuous distributions ─────────────────────────────────

inline Array laplace(double loc, double scale, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_laplace(loc, scale, _sp, _nd));
}
inline Array logistic(double loc, double scale, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_logistic(loc, scale, _sp, _nd));
}
inline Array lognormal(double mean, double sigma, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_lognormal(mean, sigma, _sp, _nd));
}
inline Array pareto(double a, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_pareto(a, _sp, _nd));
}
inline Array power(double a, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_power(a, _sp, _nd));
}
inline Array rayleigh(double scale, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_rayleigh(scale, _sp, _nd));
}
inline Array weibull(double a, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_weibull(a, _sp, _nd));
}
inline Array standard_cauchy(std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_standard_cauchy(_sp, _nd));
}
inline Array gumbel(double loc, double scale, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_gumbel(loc, scale, _sp, _nd));
}
inline Array triangular(double left, double mode, double right,
                        std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_triangular(left, mode, right, _sp, _nd));
}
inline Array vonmises(double mu, double kappa, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_vonmises(mu, kappa, _sp, _nd));
}
inline Array wald(double mean, double scale, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_wald(mean, scale, _sp, _nd));
}

// ── Discrete distributions ───────────────────────────────────

inline Array geometric(double p, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_geometric(p, _sp, _nd));
}
inline Array poisson(double lam, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_poisson(lam, _sp, _nd));
}
inline Array binomial(int n, double p, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_binomial(n, p, _sp, _nd));
}
inline Array negative_binomial(int n, double p, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_negative_binomial(n, p, _sp, _nd));
}
inline Array hypergeometric(int ngood, int nbad, int nsample,
                            std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_hypergeometric(ngood, nbad, nsample, _sp, _nd));
}
inline Array zipf(double a, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_zipf(a, _sp, _nd));
}
inline Array logseries(double p, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_logseries(p, _sp, _nd));
}

// ── Multivariate & utilities ─────────────────────────────────

inline Array permutation(const Array& x) {
    return Array(::random_permutation(x.get()));
}

inline void shuffle(Array& x) {
    ::random_shuffle(x.get());
}

inline Array choice(const Array& a, int size, bool replace = true,
                    const Array& p = Array()) {
    return Array(::random_choice(a.get(), size, replace ? 1 : 0,
                                 p.is_null() ? nullptr : p.get()));
}

inline Array multinomial(int n, const Array& pvals,
                         std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_multinomial(n, pvals.get(), _sp, _nd));
}

inline Array multivariate_normal(const Array& mean, const Array& cov,
                                 std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_multivariate_normal(mean.get(), cov.get(), _sp, _nd));
}

inline Array dirichlet(const Array& alpha, std::initializer_list<int> shape = {}) {
    NPCPP_SHAPE(shape);
    return Array(::random_dirichlet(alpha.get(), _sp, _nd));
}

inline Array random_bytes(int n) {
    return Array(::random_bytes(n));
}

#undef NPCPP_SHAPE

} // namespace npc
