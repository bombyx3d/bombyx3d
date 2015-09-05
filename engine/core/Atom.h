
#pragma once
#include "engine/core/AtomTable.h"
#include <functional>

namespace Engine
{
    class Atom
    {
    public:
        Atom() : mAtomTableEntry(&AtomTable::mNullEntry) {}
        Atom(const Atom& src) : mAtomTableEntry(src.mAtomTableEntry) {}
        Atom(Atom&& src) : mAtomTableEntry(std::move(src.mAtomTableEntry)) {}
        ~Atom() = default;

        Atom& operator=(const Atom& src) { mAtomTableEntry = src.mAtomTableEntry; return *this; }
        Atom& operator=(Atom&& src) { mAtomTableEntry = std::move(src.mAtomTableEntry); return *this; }

        size_t uniqueID() const { return mAtomTableEntry->id; }
        const std::string& text() const { return mAtomTableEntry->text; }

        bool operator==(const Atom& other) const { return mAtomTableEntry == other.mAtomTableEntry; }
        bool operator!=(const Atom& other) const { return mAtomTableEntry != other.mAtomTableEntry; }
        bool operator<(const Atom& other) const { return mAtomTableEntry->id < other.mAtomTableEntry->id; }

    private:
        const AtomTable::Entry* mAtomTableEntry;

        explicit Atom(const AtomTable::Entry* entry) : mAtomTableEntry(entry) {}

        friend class AtomTable;
    };
}

namespace std {
    template <> struct hash<Engine::Atom> {
        size_t operator()(const Engine::Atom& atom) const {
            return std::hash<size_t>()(atom.uniqueID());
        }
    };
}
