/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PET_H
#define PET_H

#include "Unit.h"
#include "TemporarySummon.h"

enum ActionFeedback : uint8
{
    FEEDBACK_NONE            = 0,
    FEEDBACK_PET_DEAD        = 1,
    FEEDBACK_NOTHING_TO_ATT  = 2,
    FEEDBACK_CANT_ATT_TARGET = 3,
    FEEDBACK_NO_PATH         = 4
};

enum PetTalk
{
    PET_TALK_SPECIAL_SPELL  = 0,
    PET_TALK_ATTACK         = 1
};

enum PetNameInvalidReason
{
    // custom, not send
    PET_NAME_SUCCESS                                        = 0,

    PET_NAME_INVALID                                        = 1,
    PET_NAME_NO_NAME                                        = 2,
    PET_NAME_TOO_SHORT                                      = 3,
    PET_NAME_TOO_LONG                                       = 4,
    PET_NAME_MIXED_LANGUAGES                                = 6,
    PET_NAME_PROFANE                                        = 7,
    PET_NAME_RESERVED                                       = 8,
    PET_NAME_THREE_CONSECUTIVE                              = 11,
    PET_NAME_INVALID_SPACE                                  = 12,
    PET_NAME_CONSECUTIVE_SPACES                             = 13,
    PET_NAME_RUSSIAN_CONSECUTIVE_SILENT_CHARACTERS          = 14,
    PET_NAME_RUSSIAN_SILENT_CHARACTER_AT_BEGINNING_OR_END   = 15,
    PET_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME              = 16
};

#define ACTIVE_SPELLS_MAX           4

#define PET_FOLLOW_DIST  1.0f
#define PET_FOLLOW_ANGLE (M_PI/2)

enum PetType : uint8
{
    SUMMON_PET = 0,
    HUNTER_PET = 1,
    MAX_PET_TYPE = 4
};

#define MAX_ACTIVE_PETS         5
#define MAX_PET_STABLES         200

// stored in character_pet.slot
enum PetSaveMode : int16
{
    PET_SAVE_AS_DELETED = -2,                        // not saved in fact
    PET_SAVE_AS_CURRENT = -3,                        // in current slot (with player)
    PET_SAVE_FIRST_ACTIVE_SLOT = 0,
    PET_SAVE_LAST_ACTIVE_SLOT = PET_SAVE_FIRST_ACTIVE_SLOT + MAX_ACTIVE_PETS,
    PET_SAVE_FIRST_STABLE_SLOT = 5,
    PET_SAVE_LAST_STABLE_SLOT = PET_SAVE_FIRST_STABLE_SLOT + MAX_PET_STABLES, // last in DB stable slot index
    PET_SAVE_NOT_IN_SLOT = -1                         // for avoid conflict with stable size grow will use negative value
};

constexpr bool IsActivePetSlot(PetSaveMode slot)
{
    return slot >= PET_SAVE_FIRST_ACTIVE_SLOT && slot < PET_SAVE_LAST_ACTIVE_SLOT;
}

constexpr bool IsStabledPetSlot(PetSaveMode slot)
{
    return slot >= PET_SAVE_FIRST_STABLE_SLOT && slot < PET_SAVE_LAST_STABLE_SLOT;
}

enum PetStableFlags : uint8
{
    PET_STABLE_ACTIVE = 0x1,
    PET_STABLE_INACTIVE = 0x2
};

enum PetSpellState
{
    PETSPELL_UNCHANGED = 0,
    PETSPELL_CHANGED = 1,
    PETSPELL_NEW = 2,
    PETSPELL_REMOVED = 3
};

enum PetSpellType
{
    PETSPELL_NORMAL = 0,
    PETSPELL_FAMILY = 1,
    PETSPELL_TALENT = 2
};

#define PET_FOLLOW_DIST  1.0f
#define PET_FOLLOW_ANGLE float(M_PI)

enum class PetTameResult : uint8
{
    Ok = 0,
    InvalidCreature = 1,
    TooMany = 2,
    CreatureAlreadyOwned = 3,
    NotTameable = 4,
    AnotherSummonActive = 5,
    UnitsCantTame = 6,
    NoPetAvailable = 7,
    InternalError = 8,
    TooHighLevel = 9,
    Dead = 10,
    NotDead = 11,
    CantControlExotic = 12,
    InvalidSlot = 13,
    EliteTooHighLevel = 14
};

enum class StableResult : uint8
{
    NotEnoughMoney = 1,                              // "you don't have enough money"
    InvalidSlot = 3,                              // "That slot is locked"
    StableSuccess = 8,                              // stable success
    UnstableSuccess = 9,                              // unstable/swap success
    BuySlotSuccess = 10,                             // buy slot success
    CantControlExotic = 11,                             // "you are unable to control exotic creatures"
    InternalError = 12,                             // "Internal pet error"
};

constexpr uint32 CALL_PET_SPELL_ID = 883;
constexpr uint32 PET_SUMMONING_DISORIENTATION = 32752;

class Player;

class Pet : public Guardian
{
    public:
        explicit Pet(Player* owner, PetType type = MAX_PET_TYPE);
        virtual ~Pet();

        void AddToWorld() override;
        void RemoveFromWorld() override;

        bool isControlled() const;
        bool isTemporarySummoned() const;

        bool IsPermanentPetFor(Player* owner);              // pet have tab in character windows and set UNIT_FIELD_PET_NUMBER

        bool Create (ObjectGuid::LowType const& guidlow, Map* map, uint32 phaseMask, uint32 Entry, uint32 pet_number);
        bool CreateBaseAtCreature(Creature* creature);
        bool CreateBaseAtCreatureInfo(CreatureTemplate const* cinfo, Unit* owner);
        bool CreateBaseAtTamed(CreatureTemplate const* cinfo, Map* map, uint32 phaseMask);
        bool LoadPetFromDB(Player* owner, uint32 petentry = 0, uint32 petnumber = 0);
        bool isBeingLoaded() const override { return m_loading;}
        void SavePetToDB(bool isDelete = false);
        void Remove();
        static void DeleteFromDB(uint32 guidlow);

        void setDeathState(DeathState s) override;                   // overwrite virtual Creature::setDeathState and Unit::setDeathState
        void Update(uint32 diff) override;                           // overwrite virtual Creature::Update and Unit::Update

        void SetSlot(PetSlot slot) { m_slot = slot; }
        PetSlot GetSlot() { return m_slot; }

        void GivePetLevel(uint8 level);
        void SynchronizeLevelWithOwner();
        bool HaveInDiet(ItemTemplate const* item) const;
        uint32 GetCurrentFoodBenefitLevel(uint32 itemlevel);
        void SetDuration(int32 dur) { m_duration = dur; }
        int32 GetDuration() { return m_duration; }

        bool HasSpell(uint32 spell) override;

        bool IsPetAura(Aura const* aura);

        void _LoadSpellCooldowns();
        void _SaveSpellCooldowns(SQLTransaction& trans);
        void _LoadAuras(uint32 timediff);
        void _SaveAuras(SQLTransaction& trans);
        void _LoadSpells();
        void _SaveSpells(SQLTransaction& trans);

        void CleanupActionBar();

        void InitPetCreateSpells();

        DeclinedName const* GetDeclinedNames() const { return m_declinedname; }

        bool    m_removed;                                  // prevent overwrite pet state in DB at next Pet::Update if pet already removed(saved)

        Unit* GetOwner() { return m_owner; }

        uint32 GetSpecializationId() const { return m_specialization; }
        void SetSpecialization(uint32 id) { m_specialization = id; }
        void LearnSpecializationSpell();
        void UnlearnSpecializationSpell();
        void CheckSpecialization();
        void ProhibitSpellSchool(SpellSchoolMask idSchoolMask, uint32 unTimeMs) override;
        
        uint32 GetGroupUpdateFlag() const { return m_groupUpdateMask; }
        void SetGroupUpdateFlag(uint32 flag);
        void ResetGroupUpdateFlag();

    protected:
        int32   m_duration;                                 // time until unsummon (used mostly for summoned guardians and not used for controlled pets)
        uint32  m_specialization;
        PetSlot m_slot;
        uint32  m_groupUpdateMask;

        DeclinedName *m_declinedname;

    private:
        void SaveToDB(uint32, uint64, uint32) override
        // override of Creature::SaveToDB     - must not be called
        {
            ASSERT(false);
        }
        void DeleteFromDB() override
        // override of Creature::DeleteFromDB - must not be called
        {
            ASSERT(false);
        }
};

class PetStable
{
public:
    struct PetInfo
    {
        PetInfo() { }

        std::string Name;
        std::string ActionBar;
        uint32 PetNumber = 0;
        uint32 CreatureId = 0;
        uint32 DisplayId = 0;
        uint32 Experience = 0;
        uint32 Health = 0;
        uint32 Mana = 0;
        uint32 LastSaveTime = 0;
        uint32 CreatedBySpellId = 0;
        uint16 SpecializationId = 0;
        uint8 Level = 0;
        ReactStates ReactState = ReactStates(0);
        PetType Type = MAX_PET_TYPE;
        bool WasRenamed = false;
    };

    Optional<uint32> CurrentPetIndex;                               // index into ActivePets or UnslottedPets if highest bit is set
    std::array<Optional<PetInfo>, MAX_ACTIVE_PETS> ActivePets;      // PET_SAVE_FIRST_ACTIVE_SLOT - PET_SAVE_LAST_ACTIVE_SLOT
    std::array<Optional<PetInfo>, MAX_PET_STABLES> StabledPets;     // PET_SAVE_FIRST_STABLE_SLOT - PET_SAVE_LAST_STABLE_SLOT
    std::vector<PetInfo> UnslottedPets;                             // PET_SAVE_NOT_IN_SLOT

    PetInfo* GetCurrentPet() { return const_cast<PetInfo*>(const_cast<PetStable const*>(this)->GetCurrentPet()); }
    PetInfo const* GetCurrentPet() const
    {
        if (!CurrentPetIndex)
            return nullptr;

        if (Optional<uint32> activePetIndex = GetCurrentActivePetIndex())
            return ActivePets[*activePetIndex] ? &ActivePets[*activePetIndex].value() : nullptr;

        if (Optional<uint32> unslottedPetIndex = GetCurrentUnslottedPetIndex())
            return *unslottedPetIndex < UnslottedPets.size() ? &UnslottedPets[*unslottedPetIndex] : nullptr;

        return nullptr;
    }

    Optional<uint32> GetCurrentActivePetIndex() const { return CurrentPetIndex && ((*CurrentPetIndex & UnslottedPetIndexMask) == 0) ? CurrentPetIndex : std::nullopt; }
    void SetCurrentActivePetIndex(uint32 index) { CurrentPetIndex = index; }
    Optional<uint32> GetCurrentUnslottedPetIndex() const { return CurrentPetIndex && ((*CurrentPetIndex & UnslottedPetIndexMask) != 0) ? Optional<uint32>(*CurrentPetIndex & ~UnslottedPetIndexMask) : std::nullopt; }
    void SetCurrentUnslottedPetIndex(uint32 index) { CurrentPetIndex = index | UnslottedPetIndexMask; }

private:
    static constexpr uint32 UnslottedPetIndexMask = 0x80000000;
};

#endif
