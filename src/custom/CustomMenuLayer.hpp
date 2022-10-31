#include "../includes.h"

#include "CCMenuExtra.hpp"
#include "CCMenuItemSpriteFancy.hpp"

enum ScreenSide { Middle, Left, Right };

class CustomMenuLayer : public gd::MenuLayer {
public:
    bool m_bAllowSway;

    CCMenu* m_pObstaclesMenu;

    CCLayer* m_pMenuGameLayer;
    CCArray* m_pParticles;
    ScreenSide m_sScreenSide;

    float BGstartX;
    CCSprite* mgBG;

    CCMenuExtra* m_pCreatorBtnShortcuts;

    CCMenuItemSpriteFancy* m_pProfileBtn;
    CCLabelBMFont* m_pUserName;

public:
    ~CustomMenuLayer();

    bool init();
    bool sceneContainsPopups();

    void update(float);
    void spawnObstacles(gd::PlayerObject*);
    void playerCollided();
    void keyBackClicked() override;
    void onEnterTransitionDidFinish() override;

    void updateUserProfileButton();

    void animateParticles();
    void swayToSide();
    void updateBGPos(CCPoint);

    void onPlay(CCObject*);
    void onGarage(CCObject*);
    void onCreator(CCObject*);
    void onDaily(CCObject*);
    void onProfile(CCObject*);
    void onAchievements(CCObject*);
    void onOptions(CCObject*);
    void onStats(CCObject*);
    
    void onCreate(CCObject*);
    void onFeatured(CCObject*);
    void onSearch(CCObject*);

    CCMenuExtra* createShortcuts();

    void switchToScene(CCObject*, int);

    static CustomMenuLayer* create() {
        auto pRet = new CustomMenuLayer();
        if(pRet && pRet->init()) {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
};