#pragma once
#include "hackbools.h"
#include "worldtoscreen.h"

int closest = 0;


Vector3 headBone;

int FindClosestEnemy() {
    if (get::EntityList() == false) { return 0; }
    if (get::LocalPlayerBase() == false) { return 0; }
    int localTeam = localplayer::teamcode();
    int ClosestEntity = 0;

    float Closest = FLT_MAX;
    float Finish = 0;

    float finish2 = 0;
    float Closest2 = FLT_MAX;

    Vector3 Bone;
    for (int i = 1; i < 64; i++) { 
        hackbools::aimbot::debug::iterator = i;
        DWORD* entptr = entity::entptr(i);
        if (*entptr == 0x0 || NULL) { continue; }
        DWORD Entity = *entptr;
        bool Dormant = entity::isdormant(Entity); if (Dormant) { continue; }
        int EnmHealth = entity::health(Entity); if (EnmHealth < 1 || EnmHealth > 100) { continue; }
        if (!hackbools::aimbot::targetTeam) {
            int EnmTeam = entity::team(Entity); if (EnmTeam == localTeam) { continue;  }
        }

        

        localplayer::setviewmatrix();
        Bone = WorldToScreen(entity::getbodypart(1, hackbools::aimbot::bodypart, Entity), entity::getbodypart(2, hackbools::aimbot::bodypart, Entity), entity::getbodypart(3, hackbools::aimbot::bodypart, Entity), globalData::localplayer::viewmatrix);

        if (Bone.z < 0.0f) {  continue; }

            Finish = pythag(Bone.x, Bone.y, xhairx, xhairy);
            if (Finish < Closest) {
                 Closest = Finish;
                 ClosestEntity = i; 
            }

    }
    return ClosestEntity;
}

void aimbot() {
    if (GetAsyncKeyState(hackbools::aimbot::toggleKey) & 0x8000 && closest != 0 && hackbools::aimbot::debug::health > 0 || (!hackbools::aimbot::findnewtarget)&& GetAsyncKeyState(hackbools::aimbot::toggleKey) & 0x8000) {
        if (hackbools::aimbot::debug::health <= 0) { closest = 0; }
    }
     else { 
               closest = FindClosestEnemy(); 
     }
    
       

        hackbools::aimbot::debug::closest = closest;
        

        if (get::EntityList() == false) { return; }
        if (get::LocalPlayerBase() == false) { return; }

        if (closest == 0) { hackbools::aimbot::oscillation::canDraw = false;  return; }

        DWORD* entptr = entity::entptr(closest);
        if (*entptr == 0x0 || NULL) { return; }
        DWORD currentEnt = *entptr;
        localplayer::setviewmatrix();
        Vector3 targetPos = WorldToScreen(entity::getbodypart(1, hackbools::aimbot::bodypart , currentEnt), entity::getbodypart(2, hackbools::aimbot::bodypart, currentEnt), entity::getbodypart(3, hackbools::aimbot::bodypart, currentEnt), globalData::localplayer::viewmatrix);
        float rdistance = pythag(targetPos.x, targetPos.y, xhairx, xhairy);


        //============|DEBUG|===========
        POINT cursor;
        GetCursorPos(&cursor);
        hackbools::aimbot::debug::health = entity::health(currentEnt);
        hackbools::aimbot::debug::gotoX = targetPos.x;
        hackbools::aimbot::debug::gotoY = targetPos.y;
        hackbools::aimbot::debug::gotoZ = targetPos.z;
        hackbools::aimbot::debug::mouseposX = cursor.x;
        hackbools::aimbot::debug::mouseposY = cursor.y;

        hackbools::aimbot::debug::reached = 3;
        //============|DEBUG ENDS|=======
        if (targetPos.z >= 0.001f) { hackbools::aimbot::oscillation::canDraw = true; }

        else {
            hackbools::aimbot::oscillation::canDraw = false;
        }

        if (GetAsyncKeyState(hackbools::aimbot::toggleKey)&0x8000) {
            if (targetPos.z >= 0.001f ) {
                if (rdistance < hackbools::aimbot::fov || !hackbools::aimbot::bfov) {

                    if (hackbools::aimbot::smoothaim) {
                        if (rdistance <= hackbools::aimbot::oscillation::antiOscillator) {
                            hackbools::aimbot::speed = 0;
                        }else {
                            if (rdistance > hackbools::aimbot::oscillation::antiOscillator + hackbools::aimbot::speed) { hackbools::aimbot::speed = hackbools::aimbot::newspeed; }
                           
                            
                            if (hackbools::aimbot::yawonly) {
                                int midDistance = xhairx - targetPos.x;
                                midDistance = abs(midDistance);
                                hackbools::aimbot::debug::distanceToY = midDistance;

                                if (midDistance < hackbools::aimbot::oscillation::antiOscillator) { hackbools::aimbot::speed = 0; }
                            }


                        }
                    }
                        POINT cursor;
                        if (hackbools::aimbot::speed != 0) { //todo: smooth in-range to avoid tilting
                            GetCursorPos(&cursor);
                            if (targetPos.x < cursor.x && (xhairx - hackbools::aimbot::oscillation::antiOscillator) > targetPos.x) {
                                SetCursorPos(cursor.x - hackbools::aimbot::speed, cursor.y);
                            }
                            if (targetPos.x > cursor.x && (xhairx + hackbools::aimbot::oscillation::antiOscillator) < targetPos.x) {
                                SetCursorPos(cursor.x + hackbools::aimbot::speed, cursor.y);
                            }
                            GetCursorPos(&cursor);
                            if (!hackbools::aimbot::yawonly) {
                                if (targetPos.y > cursor.y && (xhairy + hackbools::aimbot::oscillation::antiOscillator) < targetPos.y) {
                                    SetCursorPos(cursor.x, cursor.y + hackbools::aimbot::speed);
                                }
                                if (targetPos.y < cursor.y && (xhairy - hackbools::aimbot::oscillation::antiOscillator) > targetPos.y) {
                                    SetCursorPos(cursor.x, cursor.y - hackbools::aimbot::speed);
                                }
                            }

                            
                        }else {
                            if (hackbools::aimbot::yawonly) {
                                GetCursorPos(&cursor);
                                SetCursorPos(targetPos.x, cursor.y);
                            }
                            else {
                                SetCursorPos(targetPos.x, targetPos.y);
                            }
                        }
                }
            }
        }
}
