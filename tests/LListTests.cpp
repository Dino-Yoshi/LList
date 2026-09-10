#include <catch2/catch_test_macros.hpp>

#include <initializer_list>
#include <sstream>
#include <string>
#include <vector>

// The list intentionally exposes no inspection API.  This visibility change is
// confined to this test translation unit and leaves the production interface
// unchanged, allowing the suite to verify links, node identity, and numNodes.
#define private public
#include "../LList.cpp"
#undef private

namespace {

template <typename T>
using List = LList<T>;

template <typename T>
using Node = typename List<T>::Node;

template <typename T>
std::vector<Node<T>*> nodes(const List<T>& list) {
    std::vector<Node<T>*> result;
    Node<T>* current = list.head;
    while (current != nullptr) {
        result.push_back(current);
        current = current->next;
        REQUIRE(result.size() <= 1000);
    }
    return result;
}

template <typename T>
void require_shape(const List<T>& list, std::initializer_list<T> expected) {
    const auto actual_nodes = nodes(list);
    REQUIRE(actual_nodes.size() == expected.size());
    REQUIRE(list.numNodes == static_cast<int>(expected.size()));
    auto value = expected.begin();
    for (std::size_t i = 0; i < actual_nodes.size(); ++i, ++value) {
        REQUIRE(actual_nodes[i]->data == *value);
        REQUIRE(actual_nodes[i]->next == (i + 1 < actual_nodes.size() ? actual_nodes[i + 1] : nullptr));
    }
    REQUIRE(list.head == (actual_nodes.empty() ? nullptr : actual_nodes.front()));
}

template <typename T>
struct Cleanup {
    List<T>& list;
    ~Cleanup() {
        while (!list.Empty()) list.PopFront();
    }
};

} // namespace

TEST_CASE("PushFront creates a correctly terminated singleton", "[PushFront]") {
    List<int> list; Cleanup<int> cleanup{list};
    REQUIRE(list.PushFront(7)); require_shape(list, {7});
}

TEST_CASE("PushFront prepends to a multi-node list", "[PushFront]") {
    List<int> list; Cleanup<int> cleanup{list};
    list.PushBack(2); list.PushBack(3); REQUIRE(list.PushFront(1)); require_shape(list, {1, 2, 3});
}

TEST_CASE("PushFront preserves duplicate values as distinct nodes", "[PushFront]") {
    List<int> list; Cleanup<int> cleanup{list};
    list.PushFront(4); list.PushFront(4); list.PushFront(4); require_shape(list, {4, 4, 4});
    REQUIRE(nodes(list)[0] != nodes(list)[1]);
}

TEST_CASE("PushFront supports an empty transition after removal", "[PushFront]") {
    List<int> list; Cleanup<int> cleanup{list};
    REQUIRE(list.Empty()); list.PushFront(9); REQUIRE_FALSE(list.Empty()); REQUIRE(list.PopFront() == 9);
    REQUIRE(list.Empty()); list.PushFront(10); require_shape(list, {10});
}

TEST_CASE("PushBack creates a correctly terminated singleton", "[PushBack]") {
    List<int> list; Cleanup<int> cleanup{list};
    REQUIRE(list.PushBack(7)); require_shape(list, {7});
}

TEST_CASE("PushBack appends to a multi-node list", "[PushBack]") {
    List<int> list; Cleanup<int> cleanup{list};
    list.PushBack(1); list.PushBack(2); REQUIRE(list.PushBack(3)); require_shape(list, {1, 2, 3});
}

TEST_CASE("PushBack handles duplicates without overwriting nodes", "[PushBack]") {
    List<int> list; Cleanup<int> cleanup{list};
    list.PushBack(5); list.PushBack(5); require_shape(list, {5, 5}); REQUIRE(nodes(list)[0] != nodes(list)[1]);
}

TEST_CASE("PushBack works after the list returns to empty", "[PushBack]") {
    List<int> list; Cleanup<int> cleanup{list};
    list.PushBack(1); REQUIRE(list.PopBack() == 1); REQUIRE(list.Empty()); list.PushBack(2); require_shape(list, {2});
}

TEST_CASE("TopFront returns the head value", "[TopFront]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(3); list.PushBack(8); REQUIRE(list.TopFront() == 3);
}

TEST_CASE("TopFront returns the value of a singleton", "[TopFront]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushFront(-4); REQUIRE(list.TopFront() == -4);
}

TEST_CASE("TopFront returns value-initialized T when empty", "[TopFront]") {
    List<int> list; Cleanup<int> cleanup{list}; REQUIRE(list.TopFront() == 0); REQUIRE(list.numNodes == 0);
}

TEST_CASE("PopFront on empty is inert", "[PopFront]") {
    List<int> list; Cleanup<int> cleanup{list}; REQUIRE(list.PopFront() == 0); REQUIRE(list.Empty()); REQUIRE(list.numNodes == 0);
}

TEST_CASE("PopFront removes the only node", "[PopFront]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushFront(6); REQUIRE(list.PopFront() == 6); require_shape(list, {});
}

TEST_CASE("PopFront removes the head and preserves the successor chain", "[PopFront]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); list.PushBack(3); REQUIRE(list.PopFront() == 1); require_shape(list, {2, 3});
}

TEST_CASE("Repeated PopFront drains in forward order", "[PopFront]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); list.PushBack(3); REQUIRE(list.PopFront() == 1); REQUIRE(list.PopFront() == 2); REQUIRE(list.PopFront() == 3); require_shape(list, {});
}

TEST_CASE("TopBack returns the tail value", "[TopBack]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); REQUIRE(list.TopBack() == 2);
}

TEST_CASE("TopBack returns a singleton value", "[TopBack]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushFront(11); REQUIRE(list.TopBack() == 11);
}

TEST_CASE("TopBack returns value-initialized T when empty", "[TopBack]") {
    List<int> list; Cleanup<int> cleanup{list}; REQUIRE(list.TopBack() == 0); REQUIRE(list.Empty());
}

TEST_CASE("PopBack on empty is inert", "[PopBack]") {
    List<int> list; Cleanup<int> cleanup{list}; REQUIRE(list.PopBack() == 0); require_shape(list, {});
}

TEST_CASE("PopBack removes a singleton and clears head", "[PopBack]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(8); REQUIRE(list.PopBack() == 8); require_shape(list, {});
}

TEST_CASE("PopBack removes the tail and terminates the new tail", "[PopBack]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); list.PushBack(3); REQUIRE(list.PopBack() == 3); require_shape(list, {1, 2});
}

TEST_CASE("Repeated PopBack drains in reverse order", "[PopBack]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); list.PushBack(3); REQUIRE(list.PopBack() == 3); REQUIRE(list.PopBack() == 2); REQUIRE(list.PopBack() == 1); require_shape(list, {});
}

TEST_CASE("Find locates values at head middle and tail", "[Find]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); list.PushBack(3); REQUIRE(list.Find(1)); REQUIRE(list.Find(2)); REQUIRE(list.Find(3));
}

TEST_CASE("Find rejects absent values and empty lists", "[Find]") {
    List<int> list; Cleanup<int> cleanup{list}; REQUIRE_FALSE(list.Find(1)); list.PushBack(2); REQUIRE_FALSE(list.Find(1));
}

TEST_CASE("Find handles duplicate values", "[Find]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(4); list.PushBack(4); REQUIRE(list.Find(4)); REQUIRE(list.numNodes == 2);
}

TEST_CASE("Find reflects mutations", "[Find]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); list.Erase(1); REQUIRE_FALSE(list.Find(1)); REQUIRE(list.Find(2));
}

TEST_CASE("Erase on empty returns false", "[Erase]") {
    List<int> list; Cleanup<int> cleanup{list}; REQUIRE_FALSE(list.Erase(1)); require_shape(list, {});
}

TEST_CASE("Erase removes a singleton match", "[Erase]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); REQUIRE(list.Erase(1)); require_shape(list, {});
}

TEST_CASE("Erase leaves a singleton mismatch unchanged", "[Erase]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); REQUIRE_FALSE(list.Erase(2)); require_shape(list, {1});
}

TEST_CASE("Erase removes the first matching head duplicate", "[Erase]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(2); list.PushBack(2); list.PushBack(3); REQUIRE(list.Erase(2)); require_shape(list, {2, 3});
}

TEST_CASE("Erase removes a middle node and joins its neighbors", "[Erase]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); list.PushBack(3); REQUIRE(list.Erase(2)); require_shape(list, {1, 3});
}

TEST_CASE("Erase removes the tail and preserves earlier nodes", "[Erase]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); list.PushBack(3); REQUIRE(list.Erase(3)); require_shape(list, {1, 2});
}

TEST_CASE("Erase does not alter a list when value is absent", "[Erase]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); REQUIRE_FALSE(list.Erase(9)); require_shape(list, {1, 2});
}

TEST_CASE("Empty accurately tracks empty and non-empty transitions", "[Empty]") {
    List<int> list; Cleanup<int> cleanup{list}; REQUIRE(list.Empty()); list.PushBack(1); REQUIRE_FALSE(list.Empty()); list.PopFront(); REQUIRE(list.Empty());
}

TEST_CASE("Empty remains false across multiple nodes", "[Empty]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushFront(1); list.PushFront(2); REQUIRE_FALSE(list.Empty()); list.PopBack(); REQUIRE_FALSE(list.Empty());
}

TEST_CASE("Empty agrees with head and numNodes", "[Empty]") {
    List<int> list; Cleanup<int> cleanup{list}; REQUIRE(list.Empty()); REQUIRE(list.head == nullptr); REQUIRE(list.numNodes == 0); list.PushBack(4); REQUIRE_FALSE(list.Empty()); REQUIRE(list.head != nullptr); REQUIRE(list.numNodes == 1);
}

TEST_CASE("AddBefore inserts before the head by exact identity", "[AddBefore]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); Node<int>* target = list.head; REQUIRE(list.AddBefore(target, 9)); require_shape(list, {9, 1, 2}); REQUIRE(nodes(list)[1] == target);
}

TEST_CASE("AddBefore inserts before a middle node with duplicate values", "[AddBefore]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(5); list.PushBack(5); list.PushBack(7); Node<int>* target = list.head->next; REQUIRE(list.AddBefore(target, 8)); require_shape(list, {5, 8, 5, 7}); REQUIRE(nodes(list)[2] == target);
}

TEST_CASE("AddBefore inserts before the tail by exact identity", "[AddBefore]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); list.PushBack(3); Node<int>* target = list.head->next->next; REQUIRE(list.AddBefore(target, 9)); require_shape(list, {1, 2, 9, 3}); REQUIRE(nodes(list)[3] == target);
}

TEST_CASE("AddBefore inserts before a singleton", "[AddBefore]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(2); Node<int>* target = list.head; REQUIRE(list.AddBefore(target, 1)); require_shape(list, {1, 2}); REQUIRE(list.head->next == target);
}

TEST_CASE("AddBefore rejects an external destination node", "[AddBefore]") {
    List<int> list; List<int> other; Cleanup<int> cleanup{list}; Cleanup<int> cleanup_other{other}; list.PushBack(1); other.PushBack(2); Node<int>* external = other.head; REQUIRE_FALSE(list.AddBefore(external, 9)); require_shape(list, {1}); require_shape(other, {2});
}

TEST_CASE("AddBefore rejects null and preserves structure", "[AddBefore]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); REQUIRE_FALSE(list.AddBefore(nullptr, 9)); require_shape(list, {1});
}

TEST_CASE("AddBefore rejects any destination on an empty list", "[AddBefore]") {
    List<int> list; Cleanup<int> cleanup{list}; REQUIRE_FALSE(list.AddBefore(nullptr, 1)); require_shape(list, {});
}

TEST_CASE("AddAfter inserts after the head by exact identity", "[AddAfter]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(3); Node<int>* target = list.head; REQUIRE(list.AddAfter(target, 2)); require_shape(list, {1, 2, 3}); REQUIRE(nodes(list)[0] == target);
}

TEST_CASE("AddAfter inserts after a middle node with duplicate values", "[AddAfter]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(5); list.PushBack(5); list.PushBack(7); Node<int>* target = list.head->next; REQUIRE(list.AddAfter(target, 8)); require_shape(list, {5, 5, 8, 7}); REQUIRE(nodes(list)[1] == target);
}

TEST_CASE("AddAfter appends after the tail by exact identity", "[AddAfter]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); list.PushBack(2); Node<int>* target = list.head->next; REQUIRE(list.AddAfter(target, 3)); require_shape(list, {1, 2, 3}); REQUIRE(nodes(list)[1] == target); REQUIRE(nodes(list).back()->next == nullptr);
}

TEST_CASE("AddAfter inserts after a singleton", "[AddAfter]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); Node<int>* target = list.head; REQUIRE(list.AddAfter(target, 2)); require_shape(list, {1, 2}); REQUIRE(list.head == target);
}

TEST_CASE("AddAfter rejects an external destination node", "[AddAfter]") {
    List<int> list; List<int> other; Cleanup<int> cleanup{list}; Cleanup<int> cleanup_other{other}; list.PushBack(1); other.PushBack(2); Node<int>* external = other.head; REQUIRE_FALSE(list.AddAfter(external, 9)); require_shape(list, {1}); require_shape(other, {2});
}

TEST_CASE("AddAfter rejects null and preserves structure", "[AddAfter]") {
    List<int> list; Cleanup<int> cleanup{list}; list.PushBack(1); REQUIRE_FALSE(list.AddAfter(nullptr, 9)); require_shape(list, {1});
}

TEST_CASE("AddAfter rejects any destination on an empty list", "[AddAfter]") {
    List<int> list; Cleanup<int> cleanup{list}; REQUIRE_FALSE(list.AddAfter(nullptr, 1)); require_shape(list, {});
}
