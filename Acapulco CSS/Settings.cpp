#include "Settings.h"

// Menu
bool Settings::Menu::Enabled = false;
int Settings::Menu::Tab = 0;

// ESP
bool Settings::ESP::Enabled = false;
bool Settings::ESP::Boxes = false;
BOXES_TYPE Settings::ESP::BoxesType = BOXES_TYPE::BOXES_DEFAULT;
bool Settings::ESP::Name = false;
bool Settings::ESP::Health = false;
bool Settings::ESP::Armor = false;
bool Settings::ESP::HealthBar = false;
bool Settings::ESP::Bones = false;
bool Settings::ESP::HeadDot = false;
bool Settings::ESP::Distance = false;
bool Settings::ESP::Items = false;
bool Settings::ESP::DrawTeam = false;

// Bhop
bool Settings::Bhop::Enabled = false;
BHOP_TYPE Settings::Bhop::BhopType = BHOP_TYPE::BHOP_DEFAULT;

// AutoStrafe
bool Settings::AutoStrafe::Enabled = false;

// Misc
bool Settings::Misc::AntiSS = false;
bool Settings::Misc::Recorder = false;