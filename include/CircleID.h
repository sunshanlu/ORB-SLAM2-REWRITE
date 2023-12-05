#pragma once
#include "common_include.h"

NAMESAPCE_BEGIN

class CircleID {
public:
    explicit CircleID(int maxVal)
        : m_maxVal(maxVal - 1) {}

    CircleID &operator++();

    void setValue(int value) {
        assert(value <= m_maxVal);
        m_ID = value;
    }

    int getValue() const { return m_ID; }

    bool operator==(const CircleID &other) const { return m_ID == other.m_ID; }
    bool operator!=(const CircleID &other) const { return !(*this == other); }

private:
    int m_ID = 0;
    int m_maxVal;
};

NAMESAPCE_END