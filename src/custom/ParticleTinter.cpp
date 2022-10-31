#include "ParticleTinter.hpp"

#define MBO(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)

bool CCTintParticle::initWithDuration(float duration, ccColor4F color) {
    if(!CCActionInterval::initWithDuration(duration)) return false;

    m_pToColor = color;

    return true;
}

void CCTintParticle::startWithTarget(CCNode* target) {
    CCActionInterval::startWithTarget(target);

    auto startCol = MBO(ccColor4F, getTarget(), 0x1A0);

    m_pFromColor = startCol;
}

void CCTintParticle::update(float time) {
    ccColor4F newColor;

    newColor.r = (m_pFromColor.r + (m_pToColor.r - m_pFromColor.r)) * time;
    newColor.g = (m_pFromColor.g + (m_pToColor.g - m_pFromColor.g)) * time;
    newColor.b = (m_pFromColor.b + (m_pToColor.b - m_pFromColor.b)) * time;
    newColor.a = m_pFromColor.a;

    MBO(ccColor4F, getTarget(), 0x1A0) = newColor;
    //MBO(ccColor4F, getTarget(), 0x1C0) = newColor;
}