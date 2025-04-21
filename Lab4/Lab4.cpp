#include <cstdint>
#include <cassert>
#include <iostream>

//R = 2^32
class MontgomeryModMul {
private:
    uint32_t N;          // модуль
    uint32_t R2_mod_N;   // R^2 mod N
    uint32_t Nprime;     // -N^{-1} mod R
    uint32_t acc;        // накопичувальний добуток у Montgomery-формі

    uint32_t montReduce(uint64_t T) const {
        uint32_t m = (uint32_t)T * Nprime;
        uint64_t t = (T + (uint64_t)m * N) >> 32;
        if (t >= N) t -= N;
        return (uint32_t)t;
    }

    uint32_t toMont(uint32_t a) const {
        return montReduce((uint64_t)a * R2_mod_N);
    }

public:
    explicit MontgomeryModMul(uint32_t mod) : N(mod) {  
        //ітерації Ньютона
        uint32_t inv = 1;
        for (int i = 0; i < 5; ++i) {
            inv *= 2 - N * inv;
        }
        Nprime = (uint32_t)(-(int64_t)inv);

        uint64_t R_mod = (1ULL << 32) % N;

        R2_mod_N = (uint32_t)(R_mod * R_mod % N);

        reset();
    }

    void reset() {
        acc = toMont(1);
    }

    void mul(uint32_t x) {
        uint32_t mx = toMont(x);
        acc = montReduce((uint64_t)acc * mx);
    }

    uint32_t value() const {
        return montReduce(acc);
    }
};

int main() {
	uint32_t mod, a;
	std::cin >> mod;
	MontgomeryModMul mont(mod);
	while (std::cin >> a) {
		mont.mul(a);
		std::cout << mont.value() << std::endl;
	}
	return 0;
}