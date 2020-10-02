#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
//

#include <Standard/Pointers.h>

#include <memory>


using namespace ankerl;


__declspec(noinline) int benchStdSharedPtrCopy(std::shared_ptr<int> val)
{
	return *val;
}

__declspec(noinline) int benchStdSharedPtrRef(std::shared_ptr<int>& val)
{
	return *val;
}

__declspec(noinline) int benchTrsSharedPtrCopy(trs::SharedPtr<int> val)
{
	return *val;
}

__declspec(noinline) int benchTrsSharedPtrRef(trs::SharedPtr<int>& val)
{
	return *val;
}

void benchmarkSharedPtr()
{
	nanobench::Config config;
	config.minEpochIterations(100000);

	{
		auto shared_ptr = std::make_shared<int>();

		config
			.run("std::shared_ptr copy",
				 [&shared_ptr]() {
					 volatile int val = benchStdSharedPtrCopy(shared_ptr);
					 (void)val;
				 })
			.doNotOptimizeAway();
	}

	{
		auto shared_ptr = std::make_shared<int>();

		config
			.run("std::shared_ptr ref",
				 [&shared_ptr]() {
					 volatile int val = benchStdSharedPtrRef(shared_ptr);
					 (void)val;
				 })
			.doNotOptimizeAway();
	}

	{
		auto owner = trs::makePtrOwner<int>();
		auto shared_ptr = trs::SharedPtr(owner);

		config
			.run("trs::SharedPtr copy",
				 [&shared_ptr]() {
					 volatile int val = benchTrsSharedPtrCopy(shared_ptr);
					 (void)val;
				 })
			.doNotOptimizeAway();
	}

	{
		auto owner = trs::makePtrOwner<int>();
		auto shared_ptr = trs::SharedPtr(owner);

		config
			.run("trs::SharedPtr ref",
				 [&shared_ptr]() {
					 volatile int val = benchTrsSharedPtrRef(shared_ptr);
					 (void)val;
				 })
			.doNotOptimizeAway();
	}
}

void benchmarkWeakPtrLock()
{
	nanobench::Config config;
	config.minEpochIterations(10000);

	{
		auto owner = trs::makePtrOwner<int>(10);
		trs::WeakPtr<int> weak = owner;

		config.run("trs::WeakPtr::lock", [&weak]() {
			volatile auto shared = weak.lock();
		}).doNotOptimizeAway();
	}

	{
		auto shared = std::make_shared<int>();
		auto weak = std::weak_ptr(shared);

		config.run("std::weak_ptr::lock", [&weak]() {
			volatile auto shared = weak.lock();
		}).doNotOptimizeAway();
	}
}

int main()
{
	benchmarkWeakPtrLock();

	return 0;
}