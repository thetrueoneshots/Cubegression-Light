#include "main.h"
#include "cwmods/cwsdk.h"
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <windows.h>

const static float STAT_MODIFIERS[7] = {
	1, // Damage
	10, // Health
	1, // Armor
	1, // Resistance
	0.0075f, // Crit
	0.0075f, // Haste
	0.025f, // Regen
};

#include "src/hooks/display/change_stat_display.h"

#include "src/memory/memory_helper.h"

void* Offset(void* x1, uint64_t x2) {
	return (void*)((char*)x1 + x2);
}

void Popup(const char* title, const char* msg) {
	MessageBoxA(0, msg, title, MB_OK | MB_ICONINFORMATION);
}

class Mod : GenericMod {

	void ChangeArtifactDisplay(){
		//Item display
		MemoryHelper::FindAndReplaceString(L"Increases hang gliding speed.", L"Increases base regeneration.");
		MemoryHelper::FindAndReplaceString(L"You can climb faster.", L"Increases base health.");
		MemoryHelper::FindAndReplaceString(L"Increases sailing speed.", L"Increases base crit.");
		MemoryHelper::FindAndReplaceString(L"Increases swimming speed.", L"Increases base armor.");
		MemoryHelper::FindAndReplaceString(L"You consume less stamina when diving.", L"Increases base damage.");
		MemoryHelper::FindAndReplaceString(L"Increases riding speed.", L"Increases base haste.");
		MemoryHelper::FindAndReplaceString(L"Increases the radius of your lamp.", L"Increases base resistance.");

		//Inventory display
		MemoryHelper::FindAndReplaceString(L"Climbing Speed", L"Health Boost");
		MemoryHelper::FindAndReplaceString(L"Swimming Speed", L"Armor Boost");
		MemoryHelper::FindAndReplaceString(L"Diving Skill", L"Damage Boost");
		MemoryHelper::FindAndReplaceString(L"Riding Speed", L"Haste Boost");
		MemoryHelper::FindAndReplaceString(L"Hang Gliding Speed", L"Regen Boost");
		MemoryHelper::FindAndReplaceString(L"Sailing Speed", L"Crit Boost");
		MemoryHelper::FindAndReplaceString(L"Light Radius", L"Resistance Boost");
	}

	virtual void OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) {
		if (cube::GetGame()->world->local_creature == creature) {
			*power = *power + STAT_MODIFIERS[0] * (float)creature->diving_skill;
		}
	}

	virtual void OnCreatureSpellPowerCalculated(cube::Creature* creature, float* power) {
		if (cube::GetGame()->world->local_creature == creature) {
			*power = *power + STAT_MODIFIERS[0] * (float)creature->diving_skill;
		}
	}

	virtual void OnCreatureHPCalculated(cube::Creature* creature, float* hp) {
		cube::Game* game = cube::GetGame();
		if (!game) return;
		cube::World* world = game->world;
		if (!world) return;
		if (cube::GetGame()->world->local_creature == creature) {
			*hp = *hp + (STAT_MODIFIERS[1] * (float)creature->climbing_speed);
		}
	}

	virtual void OnCreatureArmorCalculated(cube::Creature* creature, float* armor) {
		if (cube::GetGame()->world->local_creature == creature) {
			*armor = *armor + STAT_MODIFIERS[2] * (float)creature->swimming_speed;
		}
	}

	virtual void OnCreatureCriticalCalculated(cube::Creature* creature, float* critical) {
		if (cube::GetGame()->world->local_creature == creature) {
			*critical = *critical + (STAT_MODIFIERS[4] * (float)creature->sailing_speed);
		}
	}

	virtual void OnCreatureHasteCalculated(cube::Creature* creature, float* power) {
		if (cube::GetGame()->world->local_creature == creature) {
			*power = *power + (STAT_MODIFIERS[5] * (float)creature->riding_speed);
		}
	}

	virtual void OnCreatureRegenerationCalculated(cube::Creature* creature, float* regeneration) {
		if (cube::GetGame()->world->local_creature == creature) {
			*regeneration = *regeneration + (STAT_MODIFIERS[6] * (float)creature->hang_gliding_speed);
		}
	}

	virtual void OnCreatureResistanceCalculated(cube::Creature* creature, float* resistance) {
		if (cube::GetGame()->world->local_creature == creature) {
			*resistance = *resistance + STAT_MODIFIERS[3] * (float)creature->lamp_diameter;
		}
	}

	virtual void Initialize() override {
		ChangeArtifactDisplay();
		hook::change_stats();
	}
};

EXPORT Mod* MakeMod() {
	return new Mod();
}