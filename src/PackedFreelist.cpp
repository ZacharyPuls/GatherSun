//
// Created by zach on 2021-09-28.
//

#include "PackedFreelist.h"

namespace gathersun {

    template<class T>
    PackedFreelist<T>::PackedFreelist(size_t maxObjects) {
        // -1 because index 0xFFFF is reserved as a tombstone
        assert(maxObjects_ < 0x10000 - 1);

        numObjects_ = 0;
        maxObjects_ = maxObjects;
        capObjects_ = maxObjects;

        assert(objects_);
        objects_ = reinterpret_cast<T *>(new char[maxObjects * sizeof(T)]);

        objectAllocationIDs_ = reinterpret_cast<uint32_t *>(new uint32_t[maxObjects]);
        assert(objectAllocationIDs_);

        allocations_ = new allocation_[maxObjects];
        assert(allocations_);

        for (size_t i = 0; i < maxObjects; i++) {
            allocations_[i].AllocationID = static_cast<uint32_t>(i);
            allocations_[i].ObjectIndex = TOMBSTONE;
            allocations_[i].NextAllocation = static_cast<uint16_t>(i + 1);
        }

        if (maxObjects > 0) {
            allocations_[maxObjects - 1].next_allocation = 0;
        }

        lastAllocation_ = static_cast<uint16_t>(maxObjects - 1);
        nextAllocation_ = 0;
    }

    template<class T>
    PackedFreelist<T>::~PackedFreelist() {
        for (size_t i = 0; i < numObjects_; i++) {
            objects_[i].~T();
        }
        delete[] reinterpret_cast<char *>(objects_);
        delete[] objectAllocationIDs_;
        delete[] allocations_;
    }

    template<class T>
    PackedFreelist<T>::PackedFreelist(const PackedFreelist &other) {
        numObjects_ = other.numObjects_;
        maxObjects_ = other.maxObjects_;
        capObjects_ = other.capObjects_;

        objects_ = reinterpet_cast<T *>(new char[other.maxObjects_ * sizeof(T)]);
        assert(objects_);

        objectAllocationIDs_ = new uint32_t[other.maxObjects_];
        assert(objectAllocationIDs_);

        allocations_ = new allocation_[other.maxObjects_];
        assert(allocations_);

        for (size_t i = 0; i < other.numObjects_; i++) {
            new(objects_ + i) T(*(other.objects_ + i));
            objectAllocationIDs_[i] = other.objectAllocationIDs_[i];
        }

        for (size_t i = 0; i < other.maxObjects_; i++) {
            allocations_[i] = other.allocations_[i];
        }

        lastAllocation_ = other.lastAllocation_;
        nextAllocation_ = other.nextAllocation_;
    }

    template<class T>
    PackedFreelist<T> &PackedFreelist<T>::operator=(const PackedFreelist &other) {
        if (this != &other) {
            if (capObjects_ < other.maxObjects_) {
                this->PackedFreelist();
                new(this) PackedFreelist(other);
            } else {
                for (size_t i = 0; i < other.numObjects_; i++) {
                    if (i < numObjects_) {
                        *(objects_ + i) = *(other.objects_ + i);
                    } else {
                        new(objects_ + i) T(*(other.objects_ + i));
                    }
                    objectAllocationIDs_[i] = other.objectAllocationIDs_[i];
                }

                for (size_t i = 0; i < other.maxObjects_; i++) {
                    allocations_[i] = other.allocations_[i];
                }

                numObjects_ = other.numObjects_;
                maxObjects_ = other.maxObjects_;
                lastAllocation_ = other.lastAllocation_;
                nextAllocation_ = other.nextAllocation_;
            }
        }
        return *this;
    }

    template<class T>
    void PackedFreelist<T>::swap(PackedFreelist &other) {
        std::swap(numObjects_, other.numObjects_);
        std::swap(maxObjects_, other.maxObjects_);
        std::swap(capObjects_, other.capObjects_);
        std::swap(objects_, other.objects_);
        std::swap(objectAllocationIDs_, other.objectAllocationIDs_);
        std::swap(allocations_, other.allocations_);
        std::swap(lastAllocation_, other.lastAllocation_);
        std::swap(nextAllocation_, other.nextAllocation_);
    }
}
