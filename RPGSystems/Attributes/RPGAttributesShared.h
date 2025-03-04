#pragma once

class ECS;

// Shared data needed by attributes to calculate their final values
struct RPGAttributeCalculationSharedData
{
    ECS* pEcs;
    
    // RPG systems like level up etc. should be passed through here
};