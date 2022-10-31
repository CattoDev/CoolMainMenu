#include "CustomMenuLayer.hpp"

#include "ParticleTinter.hpp"
#include "ParticleCreator.hpp"
#include "CCMenuMoveAndRemove.hpp"

using namespace gd;

#define MBO(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)

CCLayer* createMenuGameLayer() {
    return reinterpret_cast<CCLayer*(__thiscall*)()>(base + 0x18e6d0)();
}

void CustomMenuLayer::onPlay(CCObject* pSender) {switchToScene(pSender, 0);}

void CustomMenuLayer::onGarage(CCObject* pSender) {switchToScene(pSender, 1);}
void CustomMenuLayer::onCreator(CCObject* pSender) {switchToScene(pSender, 2);}
void CustomMenuLayer::onDaily(CCObject* pSender) {reinterpret_cast<void(__thiscall*)(CCLayer*, CCObject*)>(gd::base + 0x1916e0)(this, pSender);}
void CustomMenuLayer::onProfile(CCObject* pSender) {reinterpret_cast<void(__thiscall*)(CCLayer*, CCObject*)>(gd::base + 0x191870)(this, pSender);}

void CustomMenuLayer::onAchievements(CCObject* pSender) {
    auto layer = reinterpret_cast<GJDropDownLayer*(__thiscall*)()>(gd::base + 0x3c1e0)();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
    layer->showLayer(false);
}
void CustomMenuLayer::onOptions(CCObject* pSender) {
    auto layer = reinterpret_cast<GJDropDownLayer*(__thiscall*)()>(gd::base + 0x1dd310)();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
    layer->showLayer(false);
}
void CustomMenuLayer::onStats(CCObject* pSender) {
    auto layer = reinterpret_cast<GJDropDownLayer*(__thiscall*)()>(gd::base + 0x25bcf0)();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
    layer->showLayer(false);
}

void CustomMenuLayer::onCreate(CCObject* pSender) {reinterpret_cast<void(__thiscall*)(CCLayer*, CCObject*)>(gd::base + 0x4eaa0)(this, pSender);}
void CustomMenuLayer::onFeatured(CCObject* pSender) {reinterpret_cast<void(__thiscall*)(CCLayer*, CCObject*)>(gd::base + 0x4edf0)(this, pSender);}
void CustomMenuLayer::onSearch(CCObject* pSender) {reinterpret_cast<void(__thiscall*)(CCLayer*, CCObject*)>(gd::base + 0x4ef60)(this, pSender);}

void CustomMenuLayer::switchToScene(CCObject* pSender, int scene) {
    stopAllActions();
    unscheduleUpdate();

    runAction(CCEaseInOut::create(CCScaleTo::create(.5, 3), 2));

    // move camera to btn lol
    auto btn = (CCNode*)pSender;
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    runAction(CCEaseInOut::create(CCMoveTo::create(.5, CCPoint(getPositionX() - btn->getPositionX() * 2, 10)), 2));

    if(scene == 0) reinterpret_cast<void(__thiscall*)(CCLayer*, CCObject*)>(gd::base + 0x191b50)(this, pSender);
    if(scene == 1) reinterpret_cast<void(__thiscall*)(CCLayer*, CCObject*)>(gd::base + 0x191dc0)(this, pSender);
    if(scene == 2) reinterpret_cast<void(__thiscall*)(CCLayer*, CCObject*)>(gd::base + 0x191cd0)(this, pSender);
}

bool CustomMenuLayer::init() {
    if(!CCLayer::init()) return false;

    auto dir = CCDirector::sharedDirector();
    auto winSize = dir->getWinSize();

    m_pParticles = CCArray::create();
    m_pParticles->retain();
    m_sScreenSide = ScreenSide::Middle;
    m_bAllowSway = false;

    m_pObstaclesMenu = CCMenu::create();
    m_pObstaclesMenu->setPosition(CCPointZero);

    auto mgLayer = createMenuGameLayer();
    m_pMenuGameLayer = mgLayer;
    m_pMenuGameLayer->addChild(m_pObstaclesMenu);
    addChild(mgLayer);

    // move the ground
    auto ground = MBO(CCNode*, mgLayer, 0x134);
    ground->setPositionY(ground->getPositionY() - 30);

    // move bg
    auto background = MBO(CCSprite*, mgLayer, 0x130);
    BGstartX = -winSize.width / 2;
    mgBG = background;
    mgBG->setPositionX(BGstartX);

    // logo
    auto logoSpr = CCSprite::createWithSpriteFrameName("GJ_logo_001.png");
    logoSpr->setPosition(CCPoint(winSize.width / 2, winSize.height - 40));
    logoSpr->runAction(
        CCRepeatForever::create(
            CCSequence::create(
                CCEaseSineInOut::create(CCMoveBy::create(3, CCPoint(0, 5))),
                CCEaseSineInOut::create(CCMoveBy::create(3, CCPoint(0, -5))),
                nullptr
            )
        )
    );
    
    addChild(logoSpr, 2);
    
    auto btnMenu1 = CCMenu::create();
    btnMenu1->setPosition(winSize / 2 + CCPoint(0, 10));
    addChild(btnMenu1);

    // custom buttons!!!!!!!!!!!!!
    auto playBtnSpr = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");

    auto playBtnBGParticles = ParticleCreator::createBtnBackgroundParticles(100);

    auto playBtn = CCMenuItemSpriteFancy::create(playBtnSpr, this, menu_selector(CustomMenuLayer::onPlay), playBtnBGParticles);
    playBtn->playFloatAnimation(8, 2);

    playBtnBGParticles->setPosition(playBtn->getPosition());

    btnMenu1->addChild(playBtn, 1);
    btnMenu1->addChild(playBtnBGParticles);

    auto garageBtnSpr = CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png");

    auto garageBtnBGParticles = ParticleCreator::createBtnBackgroundParticles(80);

    auto garageBtn = CCMenuItemSpriteFancy::create(garageBtnSpr, this, menu_selector(CustomMenuLayer::onGarage), garageBtnBGParticles);
    garageBtn->setPositionX(-150);
    garageBtn->playFloatAnimation(4, 2);

    garageBtnBGParticles->setPosition(garageBtn->getPosition());

    btnMenu1->addChild(garageBtn, 1);
    btnMenu1->addChild(garageBtnBGParticles);

    auto creatorBtnSpr = CCSprite::createWithSpriteFrameName("GJ_creatorBtn_001.png");

    auto creatorBtnBGParticles = ParticleCreator::createBtnBackgroundParticles(80);

    auto creatorBtn = CCMenuItemSpriteFancy::create(creatorBtnSpr, this, menu_selector(CustomMenuLayer::onCreator), creatorBtnBGParticles);
    creatorBtn->setPositionX(150);
    creatorBtn->playFloatAnimation(4, 2);

    creatorBtnBGParticles->setPosition(creatorBtn->getPosition());

    btnMenu1->addChild(creatorBtn, 1);
    btnMenu1->addChild(creatorBtnBGParticles);

    // particles lol
    auto playBtnParticle = ParticleCreator::createBtnFloatParticles(CCPoint(35, 100));
    playBtnParticle->setPosition(CCPoint(winSize.width / 2, ground->getPositionY() + 190));
    m_pParticles->addObject(playBtnParticle);

    mgLayer->addChild(playBtnParticle, 1);

    auto garageBtnParticle = ParticleCreator::createBtnFloatParticles(CCPoint(25, 90));
    garageBtnParticle->setPosition(CCPoint(winSize.width / 2 + garageBtn->getPositionX(), ground->getPositionY() + 170));
    m_pParticles->addObject(garageBtnParticle);

    mgLayer->addChild(garageBtnParticle, 1);

    auto creatorBtnParticle = ParticleCreator::createBtnFloatParticles(CCPoint(25, 90));
    creatorBtnParticle->setPosition(CCPoint(winSize.width / 2 + creatorBtn->getPositionX(), ground->getPositionY() + 170));
    m_pParticles->addObject(creatorBtnParticle);

    mgLayer->addChild(creatorBtnParticle, 1);

    // other buttons
    auto otherBtnsMenu = CCMenu::create();
    otherBtnsMenu->setPosition(CCPointZero);
    addChild(otherBtnsMenu, 10);

    auto rewardsBtnSpr = CCSprite::createWithSpriteFrameName("GJ_dailyRewardBtn_001.png");
    auto rewardsBtn = CCMenuItemSpriteFancy::create(rewardsBtnSpr, this, menu_selector(CustomMenuLayer::onDaily), nullptr);
    rewardsBtn->m_bHoverActionsEnabled = false;
    rewardsBtn->setPosition(CCPoint(dir->getScreenRight() - 40, dir->getScreenTop() / 2 + 20));
    rewardsBtn->playFloatAnimation(3.5, 1);

    otherBtnsMenu->addChild(rewardsBtn);

    auto profileBtnSpr = CCSprite::createWithSpriteFrameName("GJ_profileButton_001.png");
    m_pProfileBtn= CCMenuItemSpriteFancy::create(profileBtnSpr, this, menu_selector(CustomMenuLayer::onProfile), nullptr);
    m_pProfileBtn->m_bHoverActionsEnabled = false;
    m_pProfileBtn->setPosition(CCPoint(45, 105));
    m_pProfileBtn->playFloatAnimation(-2.5, 2);

    otherBtnsMenu->addChild(m_pProfileBtn);

    m_pUserName = CCLabelBMFont::create(" ", "goldFont.fnt");
    m_pUserName->setPosition(CCPoint(profileBtnSpr->getContentSize().width / 2, 65));

    profileBtnSpr->addChild(m_pUserName);

    updateUserProfileButton();

    // button shortcuts
    m_pCreatorBtnShortcuts = createShortcuts();

    m_pCreatorBtnShortcuts->setPosition(creatorBtn->getPosition() + CCPoint(0, -55));
    m_pCreatorBtnShortcuts->runAction(CCRepeatForever::create(
        CCSequence::create(
            CCEaseInOut::create(CCMoveBy::create(2, CCPoint(0, 4)), 2), 
            CCEaseInOut::create(CCMoveBy::create(2, CCPoint(0, -4)), 2), 
            nullptr)
    ));
    m_pCreatorBtnShortcuts->setOpacity(0);
    
    btnMenu1->addChild(m_pCreatorBtnShortcuts);

    // bottom buttons
    auto bottomButtons = CCMenu::create();

    auto achievementsBtn = CCMenuItemSpriteFancy::create(CCSprite::createWithSpriteFrameName("GJ_achBtn_001.png"), this, menu_selector(CustomMenuLayer::onAchievements), nullptr);
    achievementsBtn->m_pButtonHoverAnimation = HoverAnimation::Move;
    bottomButtons->addChild(achievementsBtn);

    auto optionsBtn = CCMenuItemSpriteFancy::create(CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"), this, menu_selector(CustomMenuLayer::onOptions), nullptr);
    optionsBtn->m_pButtonHoverAnimation = HoverAnimation::Move;
    bottomButtons->addChild(optionsBtn);

    auto statsBtn = CCMenuItemSpriteFancy::create(CCSprite::createWithSpriteFrameName("GJ_statsBtn_001.png"), this, menu_selector(CustomMenuLayer::onStats), nullptr);
    statsBtn->m_pButtonHoverAnimation = HoverAnimation::Move;
    bottomButtons->addChild(statsBtn);

    bottomButtons->setPosition(CCPoint(winSize.width / 2, 30));
    bottomButtons->alignItemsHorizontallyWithPadding(5);

    achievementsBtn->m_pOriginalPos = achievementsBtn->getPosition();
    optionsBtn->m_pOriginalPos = optionsBtn->getPosition();
    statsBtn->m_pOriginalPos = statsBtn->getPosition();

    addChild(bottomButtons, 2);

    // other stuff
    animateParticles();
    m_pParticles->removeAllObjects();

    scheduleUpdate();

    setKeypadEnabled(true);

    return true;
}

void CustomMenuLayer::updateUserProfileButton() {

    auto gjm = GJAccountManager::sharedState();

    bool loggedIn = gjm->m_nPlayerAccountID - gjm->m_nPlayerAccountIDSeed > 0; // ???

    m_pProfileBtn->setVisible(loggedIn);
    m_pUserName->setVisible(loggedIn);

    if(loggedIn) {
        m_pUserName->setString(gjm->getUsername());
        m_pUserName->limitLabelWidth(70, .7, 0);
    }
}

CCMenuExtra* CustomMenuLayer::createShortcuts() {
    auto menu = CCMenuExtra::create();

    // background
    auto bg = extension::CCScale9Sprite::create("square02_001.png", {0, 0, 80, 80});
    bg->setOpacity(100);
    bg->setContentSize(CCSize(240, 80));
    bg->setScale(.3);
    bg->setTag(0xA);

    // buttons
    // 1
    auto createBtnSpr = CCSprite::createWithSpriteFrameName("GJ_createBtn_001.png");
    createBtnSpr->setScale(.2);
    auto createBtn = CCMenuItemSpriteFancy::create(createBtnSpr, this, menu_selector(CustomMenuLayer::onCreate), nullptr);
    createBtn->m_bHoverActionsEnabled = false;

    menu->addChild(createBtn);

    // 2
    auto featuredBtnSpr = CCSprite::createWithSpriteFrameName("GJ_featuredBtn_001.png");
    featuredBtnSpr->setScale(.2);
    auto featuredBtn = CCMenuItemSpriteFancy::create(featuredBtnSpr, this, menu_selector(CustomMenuLayer::onFeatured), nullptr);
    featuredBtn->m_bHoverActionsEnabled = false;

    menu->addChild(featuredBtn);

    // 3
    auto searchBtnSpr = CCSprite::createWithSpriteFrameName("GJ_searchBtn_001.png");
    searchBtnSpr->setScale(.2);
    auto searchBtn = CCMenuItemSpriteFancy::create(searchBtnSpr, this, menu_selector(CustomMenuLayer::onSearch), nullptr);
    searchBtn->m_bHoverActionsEnabled = false;

    menu->addChild(searchBtn);

    menu->alignItemsHorizontallyWithPadding(2.5);
    menu->addChild(bg, -1);

    return menu;
}

void CustomMenuLayer::spawnObstacles(PlayerObject* player) {
    /* 
        funky ways to make the player die 
    */

    if(m_pObstaclesMenu->getChildrenCount() > 0) return;

    auto dir = CCDirector::sharedDirector();
    auto playerPos = player->getPosition();

    if(MBO(bool, player, 0x638) /*isFlyMode*/ 
        || MBO(bool, player, 0x639) /*isBirdMode*/
        || MBO(bool, player, 0x63B) /*isDartMode*/) {
        /*
            I literally don't know what to make here
            so temporary solution: kill the player instantly
        */
        
        playerCollided();
    }
    else { // everything else
        // spawn spikes depending on player speed and size
        int spikesMin = 4;
        int spikesMax = 6;

        float playerSpeed = MBO(float, player, 0x648);
        if(playerSpeed > 1) spikesMin = 5;

        double x = rand() / static_cast<double>(RAND_MAX + 1);
        int spikeAmt = spikesMin + static_cast<int>(x * (spikesMax - spikesMin));//rand() % 5 + spikesMin;

        for(int i = 1; i <= spikeAmt; i++) {
            auto spike = GameObject::createWithKey(8);
            spike->setPosition(CCPoint(playerPos.x + 60 + 30 * i, 45));
            m_pObstaclesMenu->addChild(spike); // behind ground and above bg
        }

        // animate in
        m_pObstaclesMenu->runAction(
            CCEaseExponentialOut::create(
                CCMoveTo::create(.5, CCPoint(0, 30))
            )
        );
    }

    // make player jumpy into doom >:)
    if(!MBO(bool, player, 0x63D) /*isSpider*/ && !MBO(bool, player, 0x63A) /*isRollMode*/)
        player->pushButton(1);
}

void CustomMenuLayer::playerCollided() {
    // first of all, obviously, destroy the player
    // MenuGameLayer::destroyPlayer
    reinterpret_cast<void(__thiscall*)(CCLayer*)>(gd::base + 0x190100)(m_pMenuGameLayer);

    // remove spikes
    //m_pObstaclesMenu->runAction(CCMoveBy::create(.5, CCPoint(0, -30)));
    m_pObstaclesMenu->runAction(CCMenuMoveAndRemove::create(.5, CCPoint(0, -30)));
}

void CustomMenuLayer::onEnterTransitionDidFinish() {
    CCLayer::onEnterTransitionDidFinish();

    m_bAllowSway = true;
}

bool CustomMenuLayer::sceneContainsPopups() {
    return CCDirector::sharedDirector()->getRunningScene()->getChildrenCount() > 1;
}

#define DEADZONE 220
void CustomMenuLayer::update(float delta) {
    CCLayer::update(delta);

    if(!m_bAllowSway) return;

    // god damn
    if(sceneContainsPopups()) {
        if(m_sScreenSide != ScreenSide::Middle) {
            m_sScreenSide = ScreenSide::Middle;
            swayToSide();
        }

        return;
    }

    auto dir = CCDirector::sharedDirector();
    auto winSize = dir->getWinSize();
    auto winSizeRect = dir->getOpenGLView()->getViewPortRect();
    auto mPos = dir->getOpenGLView()->getMousePosition();

    ScreenSide lastSide = m_sScreenSide;

    if(mPos.x < winSizeRect.getMidX() + DEADZONE && mPos.x > winSizeRect.getMidX() - DEADZONE) {
        // middle
        m_sScreenSide = ScreenSide::Middle;
    }
    else {
        if(mPos.x < winSizeRect.getMidX()) {
            // left
            m_sScreenSide = ScreenSide::Left;
        }
        else {
            // right
            m_sScreenSide = ScreenSide::Right;
        }
    }

    if(lastSide != m_sScreenSide) swayToSide();
}

void CustomMenuLayer::updateBGPos(CCPoint pos) {
    auto bgMove = CCEaseInOut::create(CCMoveTo::create(.5, CCPoint(BGstartX - pos.x * 0.3, mgBG->getPositionY())), 2);
    bgMove->setTag(0xDAD);
    if(mgBG->getActionByTag(0xDAD) != nullptr) mgBG->stopAction(mgBG->getActionByTag(0xDAD));
    mgBG->runAction(bgMove);
}

void CustomMenuLayer::swayToSide() {
    if(!m_bAllowSway) return;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    CCPoint pos;
    float scale = 1;
    if(m_sScreenSide != ScreenSide::Middle) {
        scale = 1.25;
        int sideMod = 1;
        if(m_sScreenSide == ScreenSide::Right) sideMod = -1;

        pos = CCPoint((winSize.width / 2 * 0.25) * sideMod, 0);
    }
    else {
        pos = CCPoint(0, 0);
    }

    auto move = CCEaseInOut::create(CCMoveTo::create(.5, pos), 2);
    move->setTag(0xDAD);
    if(getActionByTag(0xDAD) != nullptr) stopAction(getActionByTag(0xDAD));
    runAction(move);
    updateBGPos(pos);

    runAction(CCEaseInOut::create(CCScaleTo::create(.5, scale), 2));

    // button shortcuts
    if(m_sScreenSide == ScreenSide::Right) {
        m_pCreatorBtnShortcuts->runAction(CCEaseOut::create(CCFadeTo::create(.5, 255), 2));
    }
    else {
        m_pCreatorBtnShortcuts->runAction(CCEaseOut::create(CCFadeTo::create(.5, 0), 2));
    }
}

void CustomMenuLayer::animateParticles() {
    for(size_t i = 0; i < m_pParticles->count(); i++) {
        auto p = (CCParticleSystemQuad*)m_pParticles->objectAtIndex(i);

        auto seq = CCSequence::create(
            CCTintParticle::create(1.5, ccc4f(0, 1, 0, 1)),
            CCTintParticle::create(1.5, ccc4f(1, 1, 0, 1)),
            CCTintParticle::create(1.5, ccc4f(1, 0, 0, 1)),
            CCTintParticle::create(1.5, ccc4f(1, 0, 1, 1)),
            CCTintParticle::create(1.5, ccc4f(0, 0, 1, 1)),
            CCTintParticle::create(1.5, ccc4f(0, 1, 1, 1)),
            nullptr
        );

        p->runAction(CCRepeatForever::create(seq)); 
    }
}

void CustomMenuLayer::keyBackClicked() {
    // reset pos
    m_sScreenSide = ScreenSide::Middle;
    swayToSide();

    // MenuLayer::onQuit
    reinterpret_cast<void(__thiscall*)(CCLayer*, CCObject*)>(gd::base + 0x191fa0)(this, nullptr);
}

CustomMenuLayer::~CustomMenuLayer() {
    m_pParticles->release();
}