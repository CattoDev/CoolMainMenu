#include "includes.h"

//#include "custom/CCMenuItemSpriteFancy.hpp"
//#include "custom/ButtonStand.hpp"
#include "custom/CustomMenuLayer.hpp"

using namespace gd;

#define MBO(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)

// fun stuff
bool (__thiscall* MenuLayer_initO)(MenuLayer*);
bool __fastcall MenuLayer_initH(MenuLayer* self, uintptr_t) {
    if(!MenuLayer_initO(self)) return false;

    self->removeAllChildren();
    
    auto layer = CustomMenuLayer::create();
    layer->setTag(0xDED);
    self->addChild(layer);

    return true;
}


void (__thiscall* MenuGameLayer_updateO)(CCLayer*, float);
void __fastcall MenuGameLayer_updateH(CCLayer* self, uintptr_t, float delta) {
    // all the patches I do just crash the game
    // so I'm recreating the func

    auto layer = (CustomMenuLayer*)self->getParent();
    if(layer != nullptr && layer->m_pMenuGameLayer == nullptr) layer = nullptr;

    auto dir = CCDirector::sharedDirector();
    auto winSize = dir->getWinSize();

    // updateColors
    reinterpret_cast<void(__thiscall*)(CCLayer*)>(gd::base + 0x18edd0)(self);

    auto player = MBO(PlayerObject*, self, 0x12C);
    auto playerPos = player->getPosition();
    MBO(CCPoint, player, 0x64C) = playerPos; // ???

    // PlayerObject::update
    float deltaTime = delta * 60;
    player->update(deltaTime);
    
    // PlayerObject::updateRotation (weird)
    __asm movss xmm1, deltaTime;
    reinterpret_cast<void(__thiscall*)(PlayerObject*)>(gd::base + 0x1ebc00)(player);

    // player scale thing
    float playerScaleFactor1 = 0.0;
    float psf2 = MBO(float, player, 0x644);
    float psf3 = MBO(float, player, 0x698);
    if (psf2 != 1.0) {
        playerScaleFactor1 = (1.0 - psf2) * psf3 * 0.5;
    }

    float groundTop = 60; // epic hardcode moment

    float playerBottom = dir->getScreenBottom() + groundTop + psf3 * 0.5 - playerScaleFactor1;
    if(player->getPositionY() < playerBottom) {
        player->setPosition(CCPoint(player->getPositionX(), playerBottom));

        // PlayerObject::hitGround
        reinterpret_cast<void(__thiscall*)(PlayerObject*, bool)>(gd::base + 0x1f6fe0)(player, false);
    }

    if(playerPos.x > winSize.width + 100) {
        // resetPlayer
        reinterpret_cast<void(__thiscall*)(CCLayer*)>(gd::base + 0x18f4b0)(self);
    }

    // player collision
    /*if(layer != nullptr && layer->m_pMenuGameLayer != nullptr) {
        if(layer->m_pObstaclesMenu->getChildrenCount() > 0) {
            auto getObjectRect = reinterpret_cast<CCRect const&(__fastcall*)(GameObject*)>(gd::base + 0xe4a40);
            auto playerRect = getObjectRect(player);

            for(int i = 0; i < layer->m_pObstaclesMenu->getChildrenCount(); i++) {
                auto spike = (GameObject*)layer->m_pObstaclesMenu->getChildren()->objectAtIndex(i);

                auto hitbox = getObjectRect(spike);
                hitbox.origin = layer->m_pObstaclesMenu->convertToWorldSpace(spike->getPosition());

                // smaller hitbox
                hitbox.size.width *= 0.25;
                hitbox.size.height *= 0.25;

                // collision
                if(playerRect.intersectsRect(hitbox)) {
                    layer->playerCollided();
                    break;
                }
            }
        }
    }*/
}

bool (__thiscall* MenuGameLayer_ccTouchBeganO)(CCLayer*, CCTouch*, CCEvent*);
bool __fastcall MenuGameLayer_ccTouchBeganH(char* self_, uintptr_t, CCTouch* pTouch, CCEvent* pEvent) {
    // fix pointer cuz virtuals break it
    auto self = reinterpret_cast<CCLayer*>(reinterpret_cast<uintptr_t>(self_) - 236);

    auto touchPos = pTouch->getLocation();
    touchPos = self->convertToNodeSpace(touchPos);
    
    if(touchPos.x > 50) {
        auto player = MBO(PlayerObject*, self, 0x12C);

        auto playerPos = player->getPosition();

        if(ccpDistance(playerPos, touchPos) <= 30) {
            // fun stuff
            auto customMenu = (CustomMenuLayer*)self->getParent();
            if(customMenu != nullptr) {
                customMenu->spawnObstacles(player);
            }
        }
    }

    return true;
}

// hooking
DWORD WINAPI MainThread(void* hModule) {
    /*if (AllocConsole())
    {
        FILE *fDummy;
        freopen_s(&fDummy, "CONIN$", "r", stdin);
        freopen_s(&fDummy, "CONOUT$", "w", stderr);
        freopen_s(&fDummy, "CONOUT$", "w", stdout);
    }*/

    MH_Initialize();

    // MenuLayer::init
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x1907b0),
        reinterpret_cast<void*>(&MenuLayer_initH),
        reinterpret_cast<void**>(&MenuLayer_initO)
    );

    // MenuGameLayer::update
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x18f190),
        reinterpret_cast<void*>(&MenuGameLayer_updateH),
        reinterpret_cast<void**>(&MenuGameLayer_updateO)
    );

    /*
        I was planning to do something cool here where instead of the player
        instantly dying, it would spawn obstacles to make it die in a cool way
        but I don't think I have enough time anymore
        + it's hard to do regardless
    */

    // MenuGameLayer::ccTouchBegan
    /*MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x1903a0),
        reinterpret_cast<void*>(&MenuGameLayer_ccTouchBeganH),
        reinterpret_cast<void**>(&MenuGameLayer_ccTouchBeganO)
    );*/

    MH_EnableHook(MH_ALL_HOOKS);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        auto h = CreateThread(0, 0, MainThread, handle, 0, 0);
        if (h)
            CloseHandle(h);
        else
            return FALSE;
    }
    return TRUE;
}