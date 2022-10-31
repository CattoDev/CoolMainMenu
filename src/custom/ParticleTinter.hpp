#include "../includes.h"

class CCTintParticle : public CCActionInterval {
public:
    ccColor4F m_pFromColor;
    ccColor4F m_pToColor;

public:
    bool initWithDuration(float, ccColor4F);
    void startWithTarget(CCNode*);
    void update(float);

    static CCTintParticle* create(float duration, ccColor4F color) {
        auto pRet = new CCTintParticle();

        pRet->initWithDuration(duration, color);
        pRet->autorelease();

        return pRet;
    }
};