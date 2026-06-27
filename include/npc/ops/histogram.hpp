#pragma once

/// Histogram wrappers for libnpc.

#include "npc/array.hpp"
#include <vector>

namespace npc {

// ── C++ result structs ──────────────────────────────────────

struct HistogramResult {
    Array hist;
    Array bin_edges;
};

struct Histogram2DResult {
    Array H;
    Array x_edges;
    Array y_edges;
};

struct HistogramDDResult {
    Array hist;
    std::vector<Array> edges;
};

// ── 1-D histogram ───────────────────────────────────────────

inline HistogramResult histogram(const Array& x, int bins, bool density = false) {
    ::HistogramResult r = {nullptr, nullptr};
    ::histogram(x.get(), bins, density ? 1 : 0, &r);
    return HistogramResult{Array(r.hist), Array(r.bin_edges)};
}

// ── 2-D histogram ───────────────────────────────────────────

inline Histogram2DResult histogram2d(const Array& x, const Array& y,
                                     int bins_x, int bins_y,
                                     const Array& range_x = Array(),
                                     const Array& range_y = Array(),
                                     bool density = false) {
    ::Histogram2DResult r = {nullptr, nullptr, nullptr};
    ::histogram2d(x.get(), y.get(), bins_x, bins_y,
                  range_x.is_null() ? nullptr : range_x.get(),
                  range_y.is_null() ? nullptr : range_y.get(),
                  density ? 1 : 0, &r);
    return Histogram2DResult{Array(r.H), Array(r.x_edges), Array(r.y_edges)};
}

// ── N-D histogram ───────────────────────────────────────────

inline HistogramDDResult histogramdd(const Array& sample,
                                     const Array& bins,
                                     const Array& range = Array(),
                                     bool density = false) {
    ::HistogramDDResult r = {nullptr, nullptr, 0};
    ::histogramdd(sample.get(), bins.get(),
                  range.is_null() ? nullptr : range.get(),
                  density ? 1 : 0, &r);
    std::vector<Array> edges;
    if (r.edges) {
        edges.reserve(r.num_edges);
        for (int i = 0; i < r.num_edges; ++i) edges.emplace_back(r.edges[i]);
        free(r.edges);
    }
    return HistogramDDResult{Array(r.hist), std::move(edges)};
}

} // namespace npc
