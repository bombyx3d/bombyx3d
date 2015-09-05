#include "AtomTable.h"
#include "engine/core/Atom.h"

namespace Engine
{
    size_t AtomTable::HashFunction::operator()(const std::string* item) const
    {
        return std::hash<std::string>()(*item);
    }

    bool AtomTable::Comparator::operator()(const std::string* item1, const std::string* item2) const
    {
        return *item1 == *item2;
    }


    AtomTable::Entry::Entry()
        : id(0)
    {
    }

    AtomTable::Entry::Entry(size_t i, const std::string& s)
        : id(i)
        , text(s)
    {
    }

    AtomTable::Entry::Entry(size_t i, std::string&& s)
        : id(i)
        , text(std::move(s))
    {
    }


    const AtomTable::Entry AtomTable::mNullEntry;
    AtomTable AtomTable::mInstance;

    AtomTable::AtomTable()
    {
    }

    AtomTable::~AtomTable()
    {
    }

    Atom AtomTable::getAtom(std::string&& text)
    {
        std::lock_guard<decltype(mMutex)> lock(mMutex);

        auto it = mMap.find(&text);
        if (it != mMap.end())
            return Atom(it->second.get());

        Entry* entry = new Entry(mMap.size() + 1, std::move(text));
        mMap.emplace(&entry->text, std::unique_ptr<Entry>(entry));

        return Atom(entry);
    }

    Atom AtomTable::getAtom(const std::string& text)
    {
        std::lock_guard<decltype(mMutex)> lock(mMutex);

        auto it = mMap.find(&text);
        if (it != mMap.end())
            return Atom(it->second.get());

        Entry* entry = new Entry(mMap.size() + 1, text);
        mMap.emplace(&entry->text, std::unique_ptr<Entry>(entry));

        return Atom(entry);
    }
}
