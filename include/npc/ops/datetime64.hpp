#pragma once

/// Datetime64 type wrapper and operations for libnpc.

#include "npc/array.hpp"
#include <string>
#include <ctime>

namespace npc {

enum class DateTimeUnit : int {
    NS   = DT_UNIT_NS,
    US   = DT_UNIT_US,
    MS   = DT_UNIT_MS,
    S    = DT_UNIT_S,
    M    = DT_UNIT_M,
    H    = DT_UNIT_H,
    D    = DT_UNIT_D,
    W    = DT_UNIT_W,
    Month = DT_UNIT_MONTH,
    Year  = DT_UNIT_YEAR,
};

class DateTime64 {
public:
    DateTime64() : dt_{0, DT_UNIT_NS} {}
    DateTime64(::DateTime64 dt) : dt_(dt) {}

    static DateTime64 now() { return DateTime64(::datetime_now()); }
    static DateTime64 from_string(const char* str, DateTimeUnit unit = DateTimeUnit::NS) {
        return DateTime64(::datetime_from_string(str, static_cast<::DateTimeUnit>(unit)));
    }

    std::string to_string() const {
        char* s = ::datetime_as_string(&dt_);
        std::string result(s);
        free(s);
        return result;
    }

    DateTime64 add_days(int64_t days) const { return DateTime64(::datetime_add_days(dt_, days)); }
    DateTime64 sub_days(int64_t days) const { return DateTime64(::datetime_sub_days(dt_, days)); }

    int64_t to_ns() const { return ::datetime_to_ns(dt_); }

    bool operator==(const DateTime64& other) const { return ::datetime_eq(dt_, other.dt_); }
    bool operator<(const DateTime64& other) const  { return ::datetime_lt(dt_, other.dt_); }
    bool operator>(const DateTime64& other) const  { return ::datetime_gt(dt_, other.dt_); }
    bool operator!=(const DateTime64& other) const { return !(*this == other); }
    bool operator<=(const DateTime64& other) const { return !(*this > other); }
    bool operator>=(const DateTime64& other) const { return !(*this < other); }

    // Access underlying C type
    ::DateTime64 get() const { return dt_; }

private:
    ::DateTime64 dt_;
};

// ── is_busday ──────────────────────────────────────────────

inline Array is_busday(const Array& dates, const char* weekmask = "1111100",
                       const Array& holidays = Array()) {
    return Array(::is_busday(dates.get(), weekmask,
                             holidays.is_null() ? nullptr : holidays.get()));
}

} // namespace npc
