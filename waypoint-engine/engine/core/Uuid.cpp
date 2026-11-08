#include "core/Uuid.hpp"
#include <iomanip>
#include <random>
#include <sstream>

namespace waypoint {
std::string GenerateUuid() {
    static std::mt19937_64 rng{std::random_device{}()};
    std::uniform_int_distribution<u64> dist;
    const u64 a = dist(rng);
    const u64 b = dist(rng);
    std::ostringstream stream;
    stream << std::hex << std::setfill('0')
           << std::setw(8) << ((a >> 32) & 0xffffffffu) << '-'
           << std::setw(4) << ((a >> 16) & 0xffffu) << '-'
           << std::setw(4) << ((a & 0x0fffu) | 0x4000u) << '-'
           << std::setw(4) << (((b >> 48) & 0x3fffu) | 0x8000u) << '-'
           << std::setw(12) << (b & 0xffffffffffffull);
    return stream.str();
}
} // namespace waypoint
