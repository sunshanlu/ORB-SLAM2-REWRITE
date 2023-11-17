#include "CircleID.h"

NAMESAPCE_BEGIN

CircleID &CircleID::operator++() {
    if (m_ID == m_maxVal){
        m_ID = 0;
        return *this;
    }
    ++m_ID;
    return *this;
}

NAMESAPCE_END