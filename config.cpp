class CfgPatches
{
	class ZenDiscordAPI
	{
		requiredVersion = 0.1;
		units[] = {};
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Radio"
		};
	};
};

/*
Model Credits:
https://sketchfab.com/3d-models/old-scifi-satellite-dish-f01a91e9773e4622b25c4d27903e85c6
https://sketchfab.com/3d-models/shield-rack-05ab5d8e85c84463b6648d52e1e45b24
https://sketchfab.com/3d-models/cardboard-box-1bcef93a72c74e9683720788716ed9a6
*/

class CfgMods
{
	class ZenDiscordAPI
	{
		dir = "ZenDiscordAPI";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "ZenDiscordAPI";
		credits = "";
		author = "Zenarchist";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = { "Game","World","Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "ZenDiscordAPI/scripts/3_Game" };
			};
			class worldScriptModule
			{
				value = "";
				files[] = { "ZenDiscordAPI/scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "ZenDiscordAPI/scripts/5_Mission" };
			};
		};
	};
};

class CfgVehicles
{
	class HouseNoDestruct;
	class Inventory_Base;

	// Vanilla megaphone redefine - add megaphone attachment for raid station
	class Megaphone: Inventory_Base
	{
		inventorySlot[] += { "ZenMegaphone" };
	}

	//! RAID ALARM
	class Zen_RaidAlarmStationKit : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenRaidAlarmStation0";
		descriptionShort = "$STR_CfgVehicles_ZenRaidAlarmStation1";
		model="ZenDiscordAPI\data\models\RaidAlarm\rack_packed.p3d";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenDiscordAPI\data\textures\raidalarm\rack_co.paa" };
		weight = 10000;
		itemSize[] = { 5,5 };
		physLayer = "item_small";
		rotationFlags = 17;
		itemBehaviour = 0;
		repairableWithKits[] = { 10 };
		repairCosts[] = { 30 };
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenDiscordAPI\data\textures\RaidAlarm\rack.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenDiscordAPI\data\textures\RaidAlarm\rack.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenDiscordAPI\data\textures\RaidAlarm\rack_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenDiscordAPI\data\textures\RaidAlarm\rack_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenDiscordAPI\data\textures\RaidAlarm\rack_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType = "wood";
	};
	class Zen_RaidAlarmStation : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenRaidAlarmStation0";
		descriptionShort = "$STR_CfgVehicles_ZenRaidAlarmStation1";
		model="ZenDiscordAPI\data\models\RaidAlarm\rack.p3d";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenDiscordAPI\data\textures\raidalarm\rack_co.paa" };
		forceFarBubble = "true";
		physLayer = "item_large";
		weight = 10000;
		itemSize[] = { 10,10 };
		repairableWithKits[] = { 10 };
		repairCosts[] = { 30 };
		attachments[] = { "Back", "ZenMegaphone" };
		quantityBar = 1;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 604800; // 1 week of energy from full battery
		varQuantityDestroyOnMin = 0;
		stackedUnit = "percentage";
		class EnergyManager
		{
			hasIcon=1;
			autoSwitchOff=0;
			convertEnergyToQuantity = 1;
			energyStorageMax=604800; // 1 week of energy from full battery
			energyUsagePerSecond=1;
			reduceMaxEnergyByDamageCoef=0.01;
			energyAtSpawn = 0;
			powerSocketsCount=1;
			compatiblePlugTypes[]={2};
			updateInterval=15;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenDiscordAPI\data\textures\RaidAlarm\rack.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenDiscordAPI\data\textures\RaidAlarm\rack.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenDiscordAPI\data\textures\RaidAlarm\rack_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenDiscordAPI\data\textures\RaidAlarm\rack_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenDiscordAPI\data\textures\RaidAlarm\rack_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType = "wood";
	};
	class Zen_RaidAlarmRadarKit : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenRaidAlarmRadar0";
		descriptionShort = "$STR_CfgVehicles_ZenRaidAlarmRadar1";
		model = "ZenDiscordAPI\data\models\zenkitbox\zenkitbox.p3d";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenDiscordAPI\data\textures\zenkitbox\kit_radar_co.paa" };
		weight = 10000;
		itemSize[] = { 9,5 };
		physLayer = "item_small";
		itemBehaviour = 0;
		soundImpactType = "cloth";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenDiscordAPI\data\textures\zenkitbox\kit.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenDiscordAPI\data\textures\zenkitbox\kit.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenDiscordAPI\data\textures\zenkitbox\kit_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenDiscordAPI\data\textures\zenkitbox\kit_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenDiscordAPI\data\textures\zenkitbox\kit_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Zen_RaidAlarmRadar : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenRaidAlarmRadar0";
		descriptionShort = "$STR_CfgVehicles_ZenRaidAlarmRadar1";
		model="ZenDiscordAPI\data\models\RaidDish\radar.p3d";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenDiscordAPI\data\textures\raiddish\radar_co.paa" };
		weight = 10000;
		itemSize[] = { 10,10 };
		forceFarBubble = "true";
		physLayer = "item_large";
		class EnergyManager
		{
			hasIcon=1;
			energyUsagePerSecond=0.01;
			cordLength=15;
			plugType=2;
			updateInterval=60;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenDiscordAPI\data\textures\raiddish\radar.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenDiscordAPI\data\textures\raiddish\radar.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenDiscordAPI\data\textures\raiddish\radar_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenDiscordAPI\data\textures\raiddish\radar_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenDiscordAPI\data\textures\raiddish\radar_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType = "wood";
	};

	// Static radar dish for aesthetic purposes
	class Zen_RaidAlarmRadar_Static : HouseNoDestruct
	{
		scope = 1;
		model = "ZenDiscordAPI\data\models\RaidDish\radar.p3d";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenDiscordAPI\data\textures\raiddish\radar_co.paa" };
	};
}

class CfgNonAIVehicles
{
	class ProxyAttachment;

	//! RAID ALARM
	class Proxyzenbaseradio_prox : ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Back";
		model = "ZenDiscordAPI/data/models/RaidAlarm/proxy/zenbaseradio_prox.p3d";
	}
	class Proxyzenmegaphone_prox : ProxyAttachment
	{
		scope = 2;
		inventorySlot = "ZenMegaphone";
		model = "ZenDiscordAPI/data/models/RaidAlarm/proxy/zenmegaphone_prox.p3d";
	}
}

class CfgSlots
{
	//! RAID ALARM
	class Slot_ZenMegaphone
	{
		name = "ZenMegaphone";
		displayName = "$STR_CfgVehicles_Megaphone0";
		ghostIcon = "missing";
	}
}

class CfgSoundShaders
{
	//! RAID ALARM
	class Zen_RaidAlarm_SoundShader
    {
        frequency = 1;
        range = 500;
        volume = 1;
		samples[] = 
		{
			{ "ZenDiscordAPI\data\sounds\raidalarm\alarm", 1 }
		}; 
    }
}

class CfgSoundSets
{
	class baseVehicles_SoundSet;

	//! RAID ALARM
	class Zen_RaidAlarm_SoundSet : baseVehicles_SoundSet
	{
		soundShaders[] = { "Zen_RaidAlarm_SoundShader" };
		volumefactor = 1;
		class Noise
		{
			strength = 500;
			type = "shot";
		}
	}
}