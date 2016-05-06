#pragma once

#include <vector>

template<typename T>
struct CachedArray {
public:
    T &GetById(size_t elementId) {
        return elements[elementId];
    }

    T &GetByIndex(size_t elementIndex) {
        return elements[aliveIds[elementIndex]];
    }

    T &operator[](size_t elementIndex) {
        return elements[aliveIds[elementIndex]];
    }

    size_t GetId(size_t elementIndex) {
        return aliveIds[elementIndex];
    }

    size_t GetElementsCount() {
        return aliveIds.size();
    }

    size_t GetAllocatedCount() {
        return elements.size();
    }

    size_t Add(bool &isCreated) {
        if (freeIds.size() > 0) {
            size_t freeElementId = freeIds.back();
            freeIds.pop_back();
            aliveIds.push_back(freeElementId);
            states[freeElementId] = CachedArray::Alive;
            isCreated = false;
            return freeElementId;
        }
        else {
            aliveIds.push_back(elements.size());
            elements.push_back(T());
            states.push_back(CachedArray::Alive);
            isCreated = true;
            return elements.size() - 1;
        }
    }

    size_t Add() {
        bool isCreated;
        return Add(isCreated);
    }

    size_t Add(const T &newbie) {
        bool isCreated;
        return Add(newbie, isCreated);
    }

    size_t Add(const T &newbie, bool &isCreated) {
        if (freeIds.size() > 0) {
            size_t freeElementId = freeIds.back();
            freeIds.pop_back();
            aliveIds.push_back(freeElementId);
            states[freeElementId] = CachedArray::Alive;
            elements[freeElementId] = newbie;
            isCreated = false;
            return freeElementId;
        } else {
            aliveIds.push_back(elements.size());
            elements.push_back(newbie);
            states.push_back(CachedArray::Alive);
            isCreated = true;
            return elements.size() - 1;
        }
    }

    void RemoveById(size_t elementId) {
        if (states[elementId] == CachedArray::Alive)
            states[elementId] = CachedArray::Releasing;

    }

    void RemoveByIndex(size_t elementIndex) {
        if (states[aliveIds[elementIndex]] == CachedArray::Alive)
            states[aliveIds[elementIndex]] = CachedArray::Releasing;
    }

    void Update() {
        aliveIds.clear();
        for (size_t elementId = 0; elementId < elements.size(); elementId++) {
            if (states[elementId] == CachedArray::Releasing) {
                states[elementId] = CachedArray::Dead;
                freeIds.push_back(elementId);
            }
            if (states[elementId] == CachedArray::Alive) {
                aliveIds.push_back(elementId);
            }
        }
    }

private:
    enum Types {
        Alive,
        Releasing,
        Dead
    };

    std::vector<size_t> freeIds;
    std::vector<size_t> aliveIds;
    std::vector<typename CachedArray::Types> states;
    std::vector<T> elements;
};