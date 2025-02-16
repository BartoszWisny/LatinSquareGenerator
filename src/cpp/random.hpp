#include <chrono>
#include <cstdint>
#include <cstdlib>

namespace cpp {
	class splitmix64 {
		public:
			constexpr explicit splitmix64(uint_fast64_t x = get_random_seed()) noexcept
				: x_(x), z_(0) {}

			[[nodiscard]] inline constexpr uint_fast64_t next() noexcept {
				z_ = (x_ += 0x9E3779B97F4A7C15);
				z_ = (z_ ^ (z_ >> 30)) * 0xBF58476D1CE4E5B9;
				z_ = (z_ ^ (z_ >> 27)) * 0x94D049BB133111EB;
				return z_ ^ (z_ >> 31);
			}

		private:
			[[nodiscard]] inline static uint_fast64_t get_random_seed() noexcept {
                return std::chrono::steady_clock::now().time_since_epoch().count();
            }

			uint_fast64_t x_;
			uint_fast64_t z_;
	};
}
