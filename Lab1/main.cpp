#include <iostream>

template<typename T>
struct gcd_res {
	T gcd, x, y; //a*x + b*y = gcd
};

template<typename T>
gcd_res<T> gcd_v1(T a, T b) {
	if (a < b) std::swap(a, b);
	return b != 0 ? gcd_v1(b, a % b) : gcd_res<T>{ a, 0, 0 };
}

template<typename T>
gcd_res<T> gcd_v2(T a, T b) {
	bool swap_flag = a < b;
	if (swap_flag) std::swap(a, b);

	T	r0 = a, r1 = b,
		x0 = 1, x1 = 0,
		y0 = 0, y1 = 1;

	while (r0 % r1 != 0) {
		T	q = r0 / r1,
			r2 = r0 - r1 * q,
			x2 = x0 - x1 * q,
			y2 = y0 - y1 * q;

		r0 = r1;	r1 = r2;
		x0 = x1;	x1 = x2;
		y0 = y1;	y1 = y2;
	}

	return swap_flag
		? gcd_res{ r1, y1, x1 }
		: gcd_res{ r1, x1, y1 };
}

template<typename T>
gcd_res<T> gcd_v3(T a, T b) {
	bool swap_flag = a < b;
	if (swap_flag) std::swap(a, b);

	if (a % b == 0)
		return swap_flag
		? gcd_res{ b, 1, 0 }
		: gcd_res{ b, 0, 1 };
	
	auto res = gcd_v3(b, a % b);
	return swap_flag 
		? gcd_res{ res.gcd, res.x - (a / b) * res.y, res.y }
		: gcd_res{ res.gcd, res.y, res.x - (a / b) * res.y };
}

template<typename T, typename GCD_F>
T reverse(T a, T m, GCD_F _gcd = gcd_v3) {
	if (a == 0) return 0;

	gcd_res res = _gcd(a, m);

	return res.gcd == 1
		? (res.x + m) % m
		: 0;
}

template<typename T, typename GCD_F>
void test(T a, T m, GCD_F _gcd, const char* msg) {
	gcd_res res = _gcd(a, m);

	std::cout
		<< msg
		<< "\n\tgcd:\t" << res.gcd
		<< "\n\tx:\t" << res.x
		<< "\n\ty:\t" << res.y
		<< '\n';
}

int main()
{
	int a = 135, m = 1000000007;

	test(a, m, gcd_v1<int>, "gcd_v1");
	test(a, m, gcd_v2<int>, "gcd_v2");
	test(a, m, gcd_v3<int>, "gcd_v3");

	std::cout 
		<< "\n\nReversed: "
		<< reverse(a, m, gcd_v3<int>) << '\n';
}