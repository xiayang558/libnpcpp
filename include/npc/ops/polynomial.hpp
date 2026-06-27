#pragma once

/// Polynomial operations wrappers for libnpc.

#include "npc/array.hpp"
#include <utility>

namespace npc {

// ── Core polynomial ops ─────────────────────────────────────

inline Array poly(const Array& roots) { return Array(::poly(roots.get())); }
inline Array roots(const Array& p)    { return Array(::roots(p.get())); }

inline Array polyadd(const Array& a, const Array& b) { return Array(::polyadd(a.get(), b.get())); }
inline Array polysub(const Array& a, const Array& b) { return Array(::polysub(a.get(), b.get())); }
inline Array polymul(const Array& a, const Array& b) { return Array(::polymul(a.get(), b.get())); }

inline std::pair<Array, Array> polydiv(const Array& u, const Array& v) {
    ::Array* r = nullptr;
    auto q = Array(::polydiv(u.get(), v.get(), &r));
    return {std::move(q), Array(r)};
}

inline Array polyder(const Array& p, int m = 1) { return Array(::polyder(p.get(), m)); }
inline Array polyint(const Array& p, int m = 1) { return Array(::polyint(p.get(), m)); }

inline Array polyval(const Array& p, const Array& x) { return Array(::polyval(p.get(), x.get())); }
inline Array polyfit(const Array& x, const Array& y, int deg) { return Array(::polyfit(x.get(), y.get(), deg)); }

} // namespace npc
