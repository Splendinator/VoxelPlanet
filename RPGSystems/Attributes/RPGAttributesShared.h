#pragma once

class ECS;

struct RPGAttributeCalculationSharedData
{
    ECS* pEcs;
    
    // RPG systems like level up etc. should be passed through here
};