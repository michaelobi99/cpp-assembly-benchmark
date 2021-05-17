#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <random>
#include <memory>
#include <sstream>
extern "C" std::int32_t asmSum(std::int32_t* x, std::uint32_t size);
struct Timer {
	Timer() = default;
	void Start(){ start = std::chrono::high_resolution_clock::now(); }
	void Stop(){ stop = std::chrono::high_resolution_clock::now(); }
	float ElapsedTime() { return std::chrono::duration<float, std::milli>(stop - start).count(); }
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;
};
std::string trim(std::string_view str) {
	auto [first, last] = std::tuple(str.find_first_not_of(' '), str.find_last_not_of(' '));
	return std::move(std::string{ str.substr(first, last - first + 1) });
}
std::int32_t cppSum(std::shared_ptr<std::int32_t[]> const& ptr, std::uint32_t size) {
	std::int32_t sum{ 0 };
	for (auto i{ 0u }; i < size; ++i) {
		sum += ptr[i];
	}
	return sum;
}
void print(std::shared_ptr<std::int32_t[]> const& ptr, std::uint32_t const size) {
	std::ostringstream output;
	auto counter{ 0u };
	for (auto i{ 0u }; i < size; ++i) {
		output<<std::left<<std::setw(6)<<ptr[i];
		if (++counter % 10 == 0) output << '\n';
	}
	output << '\n';
	std::cout << output.str();
}
int main() {
	std::string num_as_string{ "" };
	std::uint32_t number{ 0 };
	while (true) {
		auto time = Timer();
		std::shared_ptr<std::int32_t[]> array_ = nullptr;
		std::cout << "Enter a number for array length: ";
		getline(std::cin, num_as_string);
		try {
			number = std::stoul(trim(num_as_string));
			array_ = std::make_shared<std::int32_t[]>(number);
			auto mt{ std::mt19937(std::random_device{}()) };
			auto dist = std::uniform_int_distribution<std::int32_t>{ -100, 100 };
			for (auto i{ 0u }; i < number; ++i) {
				array_[i] = dist(mt);
			}
			print(array_, number);
			time.Start();
			std::cout << "c++ sum = " << cppSum(array_, number) << "\n";
			time.Stop();
			std::cout << "Elapsed time = " << time.ElapsedTime() << "\n";
			time.Start();
			std::cout << "asm sum = " << asmSum(array_.get(), number) << "\n";
			time.Stop();
			std::cout << "Elapsed time = " << time.ElapsedTime() << "\n";
		}
		catch (std::invalid_argument const& error) {
			std::cout << "ERROR: " << error.what() << "\n";
		}
	}
}