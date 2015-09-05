
#pragma once
#include "engine/core/macros.h"
#include <mutex>
#include <unordered_map>
#include <memory>
#include <string>

namespace Engine
{
    class Atom;

    class AtomTable
    {
    public:
        static AtomTable* instance() { return &mInstance; }

        Atom getAtom(std::string&& text);
        Atom getAtom(const std::string& text);

    private:
        struct HashFunction
        {
            size_t operator()(const std::string* item) const;
        };

        struct Comparator
        {
            bool operator()(const std::string* item1, const std::string* item2) const;
        };

        struct Entry
        {
            size_t id;
            std::string text;

            Entry();
            Entry(size_t i, const std::string& s);
            Entry(size_t i, std::string&& s);
        };

        static const Entry mNullEntry;
        static AtomTable mInstance;

        std::unordered_map<const std::string*, std::unique_ptr<Entry>, HashFunction, Comparator> mMap;
        std::mutex mMutex;

        AtomTable();
        ~AtomTable();

        Z_DISABLE_COPY(AtomTable);
        friend class Atom;
    };
}
