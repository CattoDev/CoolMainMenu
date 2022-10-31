#include "../includes.h"

enum HoverAnimation { Scale, Move };

class CCMenuItemSpriteFancy : public CCMenuItemSprite {
public:
    CCSprite* m_pBtnSpr;
    bool m_bHover;
    bool m_bHoverActionsEnabled;
    CCParticleSystemQuad* m_pBGParticles;

    HoverAnimation m_pButtonHoverAnimation;
    CCPoint m_pOriginalPos;

public:
    bool init(CCNode*, CCObject*, SEL_MenuHandler, CCParticleSystemQuad*);
    void update(float) override;

    void selected() override;
    void unselected() override;
    void activate() override;

    void onMouseHoverStarted();
    void onMouseHoverEnded();

    void playFloatAnimation(float, float);

    static CCMenuItemSpriteFancy* create(CCNode* sprite, CCObject* target, SEL_MenuHandler callback, CCParticleSystemQuad* bgParticles) {
        auto pRet = new CCMenuItemSpriteFancy();
        if(pRet && pRet->init(sprite, target, callback, bgParticles)) {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
};