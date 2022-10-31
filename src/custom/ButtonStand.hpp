#include "../includes.h"

class ButtonStand : public CCNode {
public:
    CCParticleSystemQuad* m_pParticles;

public:
    bool init();

    static ButtonStand* create() {
        auto pRet = new ButtonStand();
        if(pRet && pRet->init()) {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
};