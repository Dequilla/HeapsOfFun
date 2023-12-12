#include "memory.hpp"

#include <cstdlib>
#include <cstdint>

namespace memory
{
    Block* SimpleHeap::findInactiveBlock(size_t len)
    {
        if(m_rootBlock == nullptr) return nullptr;

        Block* pBlock = m_rootBlock;
        while(pBlock->active != false && pBlock->size < len)
            pBlock = pBlock->next;

        return pBlock; // Will be nullptr if nothing found
    }

    bool SimpleHeap::reserve(size_t len)
    {
        if(m_heap != nullptr) return false;

        len = align(len);

        m_heap = malloc(len);
        if(m_heap != nullptr) 
        {
            m_size = len;
            m_currentEnd = m_heap;

            m_largestAvailable = len;
            m_largestAvailablePtr = (void*)((uint8_t*)m_heap + len);

            return true;
        }
        else
        {
           return false;
        }
    }

    void SimpleHeap::quit()
    {
        Block* pBlock = m_rootBlock;
        while(pBlock != nullptr)
        {
            Block* toFree = pBlock;
            pBlock = pBlock->next;
            free(toFree);
        }
        free(m_heap);
    }

    void* SimpleHeap::allocate(size_t len)
    {
        len = align(len);

        if(m_heap == nullptr || m_size == 0) return nullptr; // Heap not initialized
        if(m_occupied + len >= m_size) return nullptr; // Not enough size left

        // We can attempt to find a free block of a acceptable size
        if(Block* pInactive = findInactiveBlock(len))
        {
            // It is not resized, instead it keeps it's original size
            pInactive->active = true;
            m_occupied += pInactive->size;
            return pInactive->data;
        }

        Block* newBlock = (Block*)malloc(sizeof(Block));
        if(newBlock == nullptr) return nullptr;
        newBlock->size = len;
        newBlock->data = m_currentEnd;
        newBlock->active = true;
        newBlock->next = nullptr;

        if(m_rootBlock == nullptr)
        {
            m_rootBlock = newBlock;
            m_currentEnd = ((uint8_t*)m_rootBlock->data + m_rootBlock->size);

            m_largestAvailable = m_largestAvailable - m_rootBlock->size;
            m_largestAvailablePtr = m_currentEnd;
        }
        else
        {
            Block* pBlock = m_rootBlock;
            while(pBlock->next != nullptr)
                pBlock = pBlock->next;
            pBlock->next = newBlock;
            m_currentEnd = ((uint8_t*)pBlock->next->data + pBlock->next->size);

            size_t endSize = ((uint8_t*)m_heap + m_size) - (uint8_t*)m_currentEnd;
            if(endSize > m_largestAvailable)
            {
                m_largestAvailable = endSize;
                m_largestAvailablePtr = m_currentEnd;
            }
        }

        m_occupied += len;
        return newBlock->data;
    }

    void SimpleHeap::deallocate(void* ptr)
    {
        Block* pBlock = m_rootBlock;
        while(pBlock->data != ptr)
            pBlock = pBlock->next;
        pBlock->active = false;
        m_occupied -= pBlock->size;
    }

}
