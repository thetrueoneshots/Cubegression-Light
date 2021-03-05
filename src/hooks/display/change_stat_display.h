const static auto NO_SUFFIX_JMPBACK_OFFSET = 0x2755DA;
const static auto PERCENTAGE_SUFFIX_JMPBACK_OFFSET = 0x275554;

extern "C" float regen_stat_hook() { // mod 4
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.025f * player->hang_gliding_speed;
}

extern "C" float damage_stat_hook() { // mod 2
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.01f * (float)player->diving_skill;
}

extern "C" float armor_stat_hook() { // mod 1
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.01f * (float)player->swimming_speed;
}

extern "C" float resistance_stat_hook() { // mod 6
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.01f * (float)player->lamp_diameter;
}

extern "C" float crit_stat_hook() { // mod 5
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.0075f * (float)player->sailing_speed;
}

extern "C" float haste_stat_hook() { // mod 3
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.0075f * (float)player->riding_speed;
}

extern "C" float hp_stat_hook() { // mod 0
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.1f * (float)player->climbing_speed;
}

GETTER_VAR(void*, ASM_regen_stat_Hook_jmpback);
__attribute__((naked)) void ASM_regen_stat_Hook() {
	asm(".intel_syntax \n"
		"call regen_stat_hook \n"
		DEREF_JMP(ASM_regen_stat_Hook_jmpback)
		".att_syntax \n"
	);
}

GETTER_VAR(void*, ASM_damage_stat_Hook_jmpback);
__attribute__((naked)) void ASM_damage_stat_Hook() {
	asm(".intel_syntax \n"
		"call damage_stat_hook \n"
		DEREF_JMP(ASM_damage_stat_Hook_jmpback)
		".att_syntax \n"
	);
}

GETTER_VAR(void*, ASM_armor_stat_Hook_jmpback);
__attribute__((naked)) void ASM_armor_stat_Hook() {
	asm(".intel_syntax \n"
		"call armor_stat_hook \n"
		DEREF_JMP(ASM_armor_stat_Hook_jmpback)
		".att_syntax \n"
	);
}

GETTER_VAR(void*, ASM_resistance_stat_Hook_jmpback);
__attribute__((naked)) void ASM_resistance_stat_Hook() {
	asm(".intel_syntax \n"
		"call resistance_stat_hook \n"
		DEREF_JMP(ASM_resistance_stat_Hook_jmpback)
		".att_syntax \n"
	);
}

GETTER_VAR(void*, ASM_crit_stat_Hook_jmpback);
__attribute__((naked)) void ASM_crit_stat_Hook() {
	asm(".intel_syntax \n"
		"call crit_stat_hook \n"
		DEREF_JMP(ASM_crit_stat_Hook_jmpback)
		".att_syntax \n"
	);
}

GETTER_VAR(void*, ASM_haste_stat_Hook_jmpback);
__attribute__((naked)) void ASM_haste_stat_Hook() {
	asm(".intel_syntax \n"
		"call haste_stat_hook \n"
		DEREF_JMP(ASM_haste_stat_Hook_jmpback)
		".att_syntax \n"
	);
}

GETTER_VAR(void*, ASM_hp_stat_Hook_jmpback);
__attribute__((naked)) void ASM_hp_stat_Hook() {
	asm(".intel_syntax \n"
		"call hp_stat_hook \n"
		DEREF_JMP(ASM_hp_stat_Hook_jmpback)
		".att_syntax \n"
	);
}

void change_regen_display() {
	auto base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x27553D), ASM_regen_stat_Hook);
	ASM_regen_stat_Hook_jmpback = Offset(base, PERCENTAGE_SUFFIX_JMPBACK_OFFSET);
}

void change_damage_display() {
	auto base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x2755C3), ASM_damage_stat_Hook);
	ASM_damage_stat_Hook_jmpback = Offset(base, NO_SUFFIX_JMPBACK_OFFSET);
}

void change_armor_display() {
	auto base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x2755AA), ASM_armor_stat_Hook);
	ASM_armor_stat_Hook_jmpback = Offset(base, NO_SUFFIX_JMPBACK_OFFSET);
}

void change_resistance_display() {
	auto base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x275617), ASM_resistance_stat_Hook);
	ASM_resistance_stat_Hook_jmpback = Offset(base, NO_SUFFIX_JMPBACK_OFFSET);
}


void change_crit_display() {
	auto base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x275591), ASM_crit_stat_Hook);
	ASM_crit_stat_Hook_jmpback = Offset(base, PERCENTAGE_SUFFIX_JMPBACK_OFFSET);
}

void change_haste_display() {
	auto base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x2755FB), ASM_haste_stat_Hook);
	ASM_haste_stat_Hook_jmpback = Offset(base, PERCENTAGE_SUFFIX_JMPBACK_OFFSET);
}

void change_hp_display() {
	auto base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x275578), ASM_hp_stat_Hook);
	ASM_hp_stat_Hook_jmpback = Offset(base, NO_SUFFIX_JMPBACK_OFFSET);
}

void remove_roundf_stat_display() {
	char* base = (char*)CWBase();
	int offset = 0x275559;
	for (int i = 0; i < 5; i++) {
		WriteByte(Offset(base, offset + i), 0x90);
	}
}

namespace hook
{
	void change_stats() {
		change_regen_display();
		change_damage_display();
		change_armor_display();
		change_resistance_display();
		change_crit_display();
		change_haste_display();
		change_hp_display();

		remove_roundf_stat_display();
	}
}
