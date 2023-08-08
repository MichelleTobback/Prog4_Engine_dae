#pragma once

#include <stdexcept>
#include <cstddef>
#include <vector>
#include <memory>
#include <vector>

namespace dae
{
	class LinkedListAllocator
	{
	public:
		LinkedListAllocator(size_t size);
		~LinkedListAllocator();

		template<typename T, typename... Args>
		T* Allocate(Args&&... args);
		template<typename T>
		void Deallocate(T* ptr);

	private:
		struct Header
		{
			size_t count;
		};
		struct Block : Header
		{
			const static int size = 16; // this is a different value for x64/Win32 targets;
			union
			{
				Block* pNext;
				char data[size - sizeof(Header)];
			};
		};

		size_t m_BlockCount;
		std::vector<Block> m_pPool;
		Block* m_pHead;
		size_t m_Size{};
	};

	inline dae::LinkedListAllocator::LinkedListAllocator(size_t size)
		: m_BlockCount{ (size + sizeof(Block) - 1) / sizeof(Block) }
		, m_pHead{ nullptr }
		, m_Size{size}

	{
		m_pHead = &m_pPool[0];

		m_pPool.resize(m_BlockCount + 1);
		Block* first = m_pHead + 1;
		first->count = m_BlockCount;
		first->pNext = nullptr;

		m_pHead->count = 0;
		m_pHead->pNext = first;
	}
}

	inline dae::LinkedListAllocator::~LinkedListAllocator()
	{
		
	}

	template<typename T, typename ...Args>
	inline T* dae::LinkedListAllocator::Allocate(Args && ...args)
	{
		const auto numBlocksToAcquire = (sizeof(T) + sizeof(Header) + sizeof(Block) - 1) / sizeof(Block);
		auto previousBlock = m_pHead;
		auto nextBlock = m_pHead->pNext;
		while (nextBlock != nullptr && nextBlock->count < numBlocksToAcquire) 
		{
			previousBlock = nextBlock;
			nextBlock = nextBlock->pNext;
		}

		if (nextBlock == nullptr) // we found no suitable block
		{
			throw std::runtime_error("out of memory");
		}

		if (nextBlock->count > numBlocksToAcquire)
		{
			Block* newBlock = nextBlock + numBlocksToAcquire;
			newBlock->count = nextBlock->count - numBlocksToAcquire;
			newBlock->pNext = nextBlock->pNext;
			nextBlock->count = numBlocksToAcquire;
			nextBlock->pNext = newBlock;
		}

		previousBlock->pNext = nextBlock->pNext;
		T* pInstance{ reinterpret_cast<T*>(nextBlock->data) };
		new (pInstance) T(std::forward<Args>(args)...);
		return pInstance;
	}
	template<typename T>
	inline void dae::LinkedListAllocator::Deallocate(T* ptr)
	{
		Block* block = reinterpret_cast<Block*> (reinterpret_cast<Header*> (ptr) - 1);

		if (block < &m_pPool[0] || block > &m_pPool[0] + m_BlockCount)
			throw std::runtime_error("invalid block");

		auto previousBlock = m_pHead;
		auto current = previousBlock->pNext;
		while (current != nullptr && current < block)
		{
			previousBlock = current;
			current = current->pNext;
		}

		if (current != nullptr && block + block->count == current)
		{
			block->count += current->count;
			block->pNext = current->pNext;
		}
		else
			block->pNext = current;

		if (previousBlock + previousBlock->count == block)
		{
			previousBlock->count += block->count;
			previousBlock->pNext = block->pNext;
		}
		else
			previousBlock->pNext = block;
	}