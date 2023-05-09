//
// Created by zach on 2021-09-28.
//

#ifndef GATHERSUN_PACKEDFREELIST_H
#define GATHERSUN_PACKEDFREELIST_H

// Copied (and adapted to project naming conventions) from https://github.com/nlguillemot/dof/blob/master/viewer/packed_freelist.h

// self-packing freelist implementation based on http://bitsquid.blogspot.ca/2011/09/managing-decoupling-part-4-id-lookup.html
// has NOT been unit tested. beware of using in production.

#include <cstdint>
#include <cassert>
#include <utility>

namespace gathersun {

    template<class T>
    class PackedFreelist {
    public:
        struct Iterator {
            Iterator(uint32_t *in) {
                currentObjectAllocationID_ = in;
            }

            Iterator &operator++() {
                currentObjectAllocationID_++;
                return *this;
            }

            uint32_t operator*() {
                return *currentObjectAllocationID_;
            }

            bool operator!=(const Iterator &other) const {
                return currentObjectAllocationID_ != other.currentObjectAllocationID_;
            }

        private:
            uint32_t *currentObjectAllocationID_;
        };

        PackedFreelist() = default;

        explicit PackedFreelist(size_t maxObjects);

        ~PackedFreelist();

        PackedFreelist(const PackedFreelist &other);

        PackedFreelist &operator=(const PackedFreelist &other);

        void swap(PackedFreelist &other);

    private:
        // used to extract the allocation index from an object id
        static constexpr uint16_t ALLOCATION_INDEX_MASK = UINT16_MAX;
        // used to mark an allocation as owning no object
        static constexpr uint16_t TOMBSTONE = UINT16_MAX;

        struct allocation_ {
            // the ID of this allocation
            // * The 16 LSBs store the index of this allocation in the list of allocations
            // * The 16 MSBs store the number of times this allocation struct was used to allocate an object
            //      * this is used as a (non-perfect) counter-measure to reusing IDs for objects
            uint32_t AllocationID;

            // the index in the objects array which stores the allocated object for this allocation
            uint16_t ObjectIndex;

            // the index in the allocations array for the next allocation to allocate after this one
            uint16_t NextAllocation;
        };

        // Storage for objects
        // Objects are contiguous, and always packed to the start of the storage.
        // Objects can be relocated in this storage thanks to the separate list of allocations.
        size_t numObjects_ = 0;
        size_t maxObjects_ = 0;
        size_t capObjects_ = 0;
        T *objects_ = nullptr;

        // the allocation ID of each object in the object array (1-1 mapping)
        uint32_t *objectAllocationIDs_ = nullptr;

        // FIFO queue to allocate objects with least ID reuse possible
        allocation_ *allocations_ = nullptr;

        // when an allocation is freed, the enqueue index struct's next will point to it
        // this ensures that allocations are reused as infrequently as possible,
        // which reduces the likelihood that two objects have the same ID.
        // note objects are still not guaranteed to have globally unique IDs, since IDs will be reused after N * 2^16 allocations
        uint16_t lastAllocation_ = -1;

        // the next index struct to use for an allocation
        uint16_t nextAllocation_ = -1;
    };
}

#endif // GATHERSUN_PACKEDFREELIST_H
