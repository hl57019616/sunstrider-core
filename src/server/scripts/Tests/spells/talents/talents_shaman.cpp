#include "TestCase.h"
#include "TestPlayer.h"
#include "World.h"
#include "ClassSpells.h"

class AncestralKnowledgeTest : public TestCaseScript
{
public:
	AncestralKnowledgeTest() : TestCaseScript("talents shaman ancestral_knowledge") { }

	class AncestralKnowledgeTestImpt : public TestCase
	{
	public:
		AncestralKnowledgeTestImpt() : TestCase(true) { }

		void Test() override
		{
			TestPlayer* player = SpawnRandomPlayer(CLASS_SHAMAN);
			uint32 const startMana = player->GetMaxPower(POWER_MANA);
			LearnTalent(player, Talents::Shaman::ANCESTRAL_KNOWLEDGE_RNK_5);
			uint32 const expectedMana = startMana * 1.05f;
			TEST_ASSERT(Between<float>(player->GetMaxPower(POWER_MANA), expectedMana - 1, expectedMana + 1));
		}
	};

	std::shared_ptr<TestCase> GetTest() const override
	{
		return std::make_shared<AncestralKnowledgeTestImpt>();
	}
};

class ConcussionTest : public TestCaseScript
{
public:

	ConcussionTest() : TestCaseScript("talents shaman concussion") { }

	class ConcussionTestImpt : public TestCase
	{
	public:
		ConcussionTestImpt() : TestCase(true) { }

		void Test() override
		{
			TestPlayer* player = SpawnRandomPlayer(CLASS_SHAMAN);
			// Lightning Bolt rank 12
			uint32 const lightningBoltMinDamage = 563;
			uint32 const lightningBoltMaxDamage = 634;

			// Chain Lightning rank 6
			uint32 const chainLightningMinDamage = 734;
			uint32 const chainLightningMaxDamage = 838;

			// Earth Shock rank 8
			uint32 const earthShockMinDamage = 658;
			uint32 const earthShockMaxDamage = 692;

			// Flame Shock rank 7
			uint32 const flameShockMinDamage = 377;
			uint32 const flameShockMinDoT = 420; //blazeit

			// Frost Shock rank 5
			uint32 const frostShockMinDamage = 640;
			uint32 const frostShockMaxDamage = 676;

			Creature* dummyTarget = SpawnCreature();
			//Test regular damage
			TestSpellDamage(player, dummyTarget, ClassSpells::Shaman::LIGHTNING_BOLT_RNK_12, lightningBoltMinDamage, lightningBoltMaxDamage);
			TestSpellDamage(player, dummyTarget, ClassSpells::Shaman::CHAIN_LIGHTNING_RNK_6, chainLightningMinDamage, chainLightningMaxDamage);
			TestSpellDamage(player, dummyTarget, ClassSpells::Shaman::EARTH_SHOCK_RNK_8, earthShockMinDamage, earthShockMaxDamage);
			TestSpellDamage(player, dummyTarget, ClassSpells::Shaman::FLAME_SHOCK_RNK_7, flameShockMinDamage, flameShockMinDoT);
			TestSpellDamage(player, dummyTarget, ClassSpells::Shaman::FROST_SHOCK_RNK_5, frostShockMinDamage, frostShockMaxDamage);

			//Test improved damage 5%
			TestSpellDamage(player, dummyTarget, ClassSpells::Shaman::LIGHTNING_BOLT_RNK_12, lightningBoltMinDamage * 1.05f, lightningBoltMaxDamage * 1.05f);
			TestSpellDamage(player, dummyTarget, ClassSpells::Shaman::CHAIN_LIGHTNING_RNK_6, chainLightningMinDamage * 1.05f, chainLightningMaxDamage * 1.05f);
			TestSpellDamage(player, dummyTarget, ClassSpells::Shaman::EARTH_SHOCK_RNK_8, earthShockMinDamage * 1.05f, earthShockMaxDamage * 1.05f);
			TestSpellDamage(player, dummyTarget, ClassSpells::Shaman::FLAME_SHOCK_RNK_7, flameShockMinDamage * 1.05f, flameShockMinDoT * 1.05f);
			TestSpellDamage(player, dummyTarget, ClassSpells::Shaman::FROST_SHOCK_RNK_5, frostShockMinDamage * 1.05f, frostShockMaxDamage * 1.05f);
		}
	};

	std::shared_ptr<TestCase> GetTest() const override
	{
		return std::make_shared<ConcussionTestImpt>();
	}
};

class EnhancingTotemsTest : public TestCaseScript
{
public:
	EnhancingTotemsTest() : TestCaseScript("talents shaman enhancing_totems") { }

	class EnhancingTotemsTestImpt : public TestCase
	{
	public:
		EnhancingTotemsTestImpt() : TestCase(true) { }

		void Test() override
		{
			TestPlayer* player = SpawnRandomPlayer(CLASS_SHAMAN);

			uint32 const startAgi = player->GetStat(STAT_AGILITY);
			uint32 const startStr = player->GetStat(STAT_STRENGTH);

			LearnTalent(player, Talents::Shaman::ENHANCING_TOTEMS_RNK_2);

			uint32 const expectedAgi = startAgi + 77 * 1.15f;
			uint32 const expectedStr = startAgi + 86 * 1.15f;

			TEST_ASSERT(Between<float>(player->GetStat(STAT_AGILITY), expectedAgi - 1, expectedAgi + 1));
			TEST_ASSERT(Between<float>(player->GetStat(STAT_STRENGTH), expectedStr - 1, expectedStr + 1));
		}
	};

	std::shared_ptr<TestCase> GetTest() const override
	{
		return std::make_shared<EnhancingTotemsTestImpt>();
	}
};

class ToughnessTest : public TestCaseScript
{
public:
	ToughnessTest() : TestCaseScript("talents shaman toughness") { }

	class ToughnessTestImpt : public TestCase
	{
	public:
		ToughnessTestImpt() : TestCase(true) { }

		void Test() override
		{
			TestPlayer* player = SpawnRandomPlayer(CLASS_SHAMAN);

			// Armor effect
			uint32 const startArmor = player->GetArmor();
			uint32 const expectedArmor = startArmor * 1.1f;

			LearnTalent(player, Talents::Shaman::TOUGHNESS_RNK_5);
			TEST_ASSERT(Between<float>(player->GetArmor(), expectedArmor - 1, expectedArmor + 1));

			// Slowing effects
			player->AddAura(ClassSpells::Warrior::HAMSTRING_RNK_4, player);
			Aura* hamstring = player->GetAura(ClassSpells::Warrior::HAMSTRING_RNK_4, EFFECT_0);
			if (!hamstring)
				return;
			TEST_ASSERT(hamstring->GetAuraMaxDuration() < 7.6f);
			player->RemoveAurasDueToSpell(ClassSpells::Warrior::HAMSTRING_RNK_4);

			player->AddAura(ClassSpells::Rogue::CRIPPLING_POISON_II_RNK_2, player);
			Aura* poison = player->GetAura(ClassSpells::Rogue::CRIPPLING_POISON_II_RNK_2, EFFECT_0);
			if (!poison)
				return;
			TEST_ASSERT(poison->GetAuraMaxDuration() < 6.1f);
			player->RemoveAurasDueToSpell(ClassSpells::Rogue::CRIPPLING_POISON_II_RNK_2);

			player->AddAura(ClassSpells::Shaman::FROST_SHOCK_RNK_5, player);
			Aura* shock = player->GetAura(ClassSpells::Shaman::FROST_SHOCK_RNK_5, EFFECT_0);
			if (!shock)
				return;
			TEST_ASSERT(shock->GetAuraMaxDuration() < 4.1f);
			player->RemoveAurasDueToSpell(ClassSpells::Shaman::FROST_SHOCK_RNK_5);
		}
	};

	std::shared_ptr<TestCase> GetTest() const override
	{
		return std::make_shared<ToughnessTestImpt>();
	}
};

class WeaponMasteryTest : public TestCaseScript
{
public:
	WeaponMasteryTest() : TestCaseScript("talents shaman weapon_mastery") { }

	class WeaponMasteryTestImpt : public TestCase
	{
	public:
		WeaponMasteryTestImpt() : TestCase(true) { }

		void Test() override
		{
			TestPlayer* player = SpawnRandomPlayer(CLASS_SHAMAN);
			LearnTalent(player, Talents::Shaman::DUAL_WIELD_RNK_1);
			player->SetSkill(44, 0, 350, 350); // Axe
			EquipItem(player, 34331); // Rising Tide MH
			player->SetSkill(473, 0, 350, 350); // Fist weapon
			EquipItem(player, 34203); // Grip of Mannoroth OH

			uint32 const minOH = 113;
			uint32 const maxMH = 313;

			Creature* dummyTarget = SpawnCreature();
			// Test regular damage
			//TestDamage(player, dummyTarget, minOH, maxMH);

			// Test improved 10%
			LearnTalent(player, Talents::Shaman::WEAPON_MASTERY_RNK_5);
			//TestDamage(player, dummyTarget, minOH * 1.1f, maxMH * 1.1f);
		}
	};

	std::shared_ptr<TestCase> GetTest() const override
	{
		return std::make_shared<WeaponMasteryTestImpt>();
	}
};

class ImprovedHealingWaveTest : public TestCaseScript
{
public:
	ImprovedHealingWaveTest() : TestCaseScript("talents shaman improved_healing_wave") { }

	class ImprovedHealingWaveTestImpt : public TestCase
	{
	public:
		ImprovedHealingWaveTestImpt() : TestCase(true) { }

		void Test() override
		{
			TestPlayer* player = SpawnRandomPlayer(CLASS_SHAMAN);

			uint32 const startCast = 3;
			uint32 const expectedCast = 2.5;

			// Test before
			LearnTalent(player, Talents::Shaman::IMPROVED_HEALING_WAVE_RNK_5);
			// Test after
		}
	};

	std::shared_ptr<TestCase> GetTest() const override
	{
		return std::make_shared<ImprovedHealingWaveTestImpt>();
	}
};

void AddSC_test_talents_shaman()
{
	new AncestralKnowledgeTest();
	new ConcussionTest();
	new EnhancingTotemsTest();
	new ToughnessTest();
	new WeaponMasteryTest();
	new ImprovedHealingWaveTest();
}