#include "stdafx.h"
#include "App.h"
#include <exception>
#include <iostream>

struct AllocationMetrics
{
	uint32_t totalAllocated = 0;
	uint32_t totalFreed = 0;

	uint32_t CurrentUsage() { return totalAllocated - totalFreed; }
};
static AllocationMetrics heapMetrics;

//memory tracking for heap allocated objects 
void* operator new(size_t size)
{
	//std::cout << "Allocating " << size << " bytes\n";
	heapMetrics.totalAllocated += size;
	return malloc(size);
}
void operator delete(void* memory, size_t size)
{
	//std::cout << "DeAllocating " << size << " Bytes\n";
	heapMetrics.totalFreed += size;
	free(memory);
}

static void PrintMemoryUsage()
{
	std::cout << "Total Allocated: " << heapMetrics.totalAllocated << " bytes\n";
	std::cout << "Total Freed: " << heapMetrics.totalFreed << " bytes\n";
	std::cout << "Current Usage: " << heapMetrics.CurrentUsage() << " bytes\n\n";
}

int main()
{
	PrintMemoryUsage();
	App app;
	PrintMemoryUsage();
	{
		PrintMemoryUsage();

		app.createLoop();
		PrintMemoryUsage();

	}
	PrintMemoryUsage();
	return 0;
}