#include <iostream>

template <typename T>
class assign_mediator
{
private:
	T &ref_;

public:
	assign_mediator(T &ref) : ref_(ref) {}

	template <typename X>
	assign_mediator& operator=(X &&x)
	{
		ref_ = x;

		return *this;
	}
};

template<typename T>
assign_mediator<T> make_mediator(T &ref) { return assign_mediator<T>(ref); }

template<typename T>
struct type_derivatives
{
	typedef T* pointer;
	typedef T const* const_pointer;
	typedef T& reference;
	typedef T const& const_reference;
};

template<typename T>
using pointer_t = T * ;

void f()
{
	type_derivatives<int>::pointer p = nullptr;

	pointer_t<int> q = nullptr;

	p = q;
}

class M
{
private:
	size_t size_;
	int *data_;
public:
	M(int size = 10) : size_(size), data_(new int[size]) { std::cout << "M(int)\n"; }
	M(M const& other) : size_(other.size_), data_(new int[other.size_]) { /*std::copy*/ std::cout << "M(M const& other)\n"; }
	M(M&& other) : size_(other.size_), data_(other.data_) { other.data_ = nullptr; }

	~M() { delete[] data_; std::cout << "~M()\n"; }
};

M operator+(M const&x, M const&y)
{
	std::cout << "M operator+(M const&x, M const&y)\n";
	return M();
}

void g()
{
	M a, b;
	M c = a + b;
}

int main()
{
	/*
	int x = 0;
	//((make_mediator(x) = 9) = 8) = 7;

	auto m = make_mediator(x);

	m = 7;

	GridView.Current().Settings()
		.SetColor(Red)
		.SetWidth(10)
		.SetThickness(1);
	f();

	std::cout << x << std::endl;
	*/

	g();

	return 0;
}
