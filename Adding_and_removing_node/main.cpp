
#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#include <iostream>

struct ListNode
{
public:
    ListNode(int value, ListNode* prev = nullptr, ListNode* next = nullptr)
        : value(value), prev(prev), next(next)
    {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }

public:
    int value;
    ListNode* prev;
    ListNode* next;
};


class List
{
public:
    List()
        : m_head(new ListNode(static_cast<int>(0))), m_size(0),
        m_tail(new ListNode(0, m_head))
    {
    }

    virtual ~List()
    {
        Clear();
        delete m_head;
        delete m_tail;
    }

    bool Empty() { return m_size == 0; }

    unsigned long Size() { return m_size; }

    void PushFront(int value)
    {
        new ListNode(value, m_head, m_head->next);
        ++m_size;
    }

    void PushBack(int value)
    {
        new ListNode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int PopFront()
    {
        if (Empty()) throw std::runtime_error("List is empty");
        auto node = extractPrev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int PopBack()
    {
        if (Empty()) throw std::runtime_error("List is empty");
        auto node = extractPrev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void Clear()
    {
        auto current = m_head->next;
        while (current != m_tail)
        {
            current = current->next;
            delete extractPrev(current);
        }
    }

    int Back()
    {
        if (Empty())
            throw std::runtime_error("List is empty");
        return m_tail->prev->value;
    }

    int Front()
    {
        if (Empty())
            throw std::runtime_error("List is empty");
        return m_head->next->value;
    }

private:
    ListNode* extractPrev(ListNode* node)
    {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    ListNode* m_head;
    ListNode* m_tail;
    unsigned long m_size;
};


TEST_CASE("Testing", "[ListNod]")
{
    SECTION("Addition to the beginning")
    {
        List newList;
        newList.PushFront(5);
        newList.PushFront(4);
        newList.PushFront(3);
        REQUIRE(newList.PopFront() == 3);  
    }

    SECTION("Appending to the end")
    {
        List newList;
        newList.PushBack(15);
        newList.PushBack(14);
        newList.PushBack(13);
        REQUIRE(newList.PopBack() == 13);
    }

    SECTION("Removing the first node")
    {
        List newList;
        newList.PushBack(35);
        newList.PushFront(42);
        REQUIRE(newList.PopFront() == 42);
    }

    SECTION("Removing the first node, empty list")
    {
        List newList;
        REQUIRE_THROWS_AS(newList.PopFront(), std::runtime_error);
    }

    SECTION("Removing the last node")
    {
        List newList;
        newList.PushBack(100);
        newList.PushFront(200);
        REQUIRE(newList.PopBack() == 100);
    }

    SECTION("Removing the last node, empty list")
    {
        List newList;
        REQUIRE_THROWS_AS(newList.PopBack(), std::runtime_error);
    }

    SECTION("Removing and adding")
    {
        List newList;
        newList.PushFront(10);
        newList.PushBack(20);
        newList.PushBack(30);
        REQUIRE(newList.PopFront() == 10);
        REQUIRE(newList.PopBack() == 30);
    }
}