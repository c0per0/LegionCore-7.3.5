/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
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

 // This is where scripts' loading functions should be declared:
void AddSC_outdoorpvp_hp();
void AddSC_outdoorpvp_na();
void AddSC_outdoorpvp_tf();
void AddSC_outdoorpvp_zm();
void AddSC_outdoorpvp_rg();

void AddSC_AshranMgr();
void AddSC_AshranNPCAlliance();
void AddSC_AshranNPCHorde();
void AddSC_AshranSpells();
void AddSC_AshranAreaTrigger();
void AddSC_AshranNPCNeutral();
void AddSC_AshranQuest();

void AddSC_Kloaka();

// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
void AddOutdoorPvPScripts()
{
    AddSC_outdoorpvp_hp();
    AddSC_outdoorpvp_na();
    AddSC_outdoorpvp_tf();
    AddSC_outdoorpvp_zm();
    AddSC_outdoorpvp_rg();

    AddSC_AshranMgr();
    AddSC_AshranNPCAlliance();
    AddSC_AshranNPCHorde();
    AddSC_AshranSpells();
    AddSC_AshranAreaTrigger();
    AddSC_AshranNPCNeutral();
    AddSC_AshranQuest();

    AddSC_Kloaka();
}