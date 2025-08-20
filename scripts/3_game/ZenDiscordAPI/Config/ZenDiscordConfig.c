class ZenDiscordConfig
{
	// Config location
	private const static string zenModFolderPrenested = "$profile:\\Zenarchist\\";
	private const static string zenModFolder = "$profile:\\Zenarchist\\Utilities\\";
	private const static string zenConfigName = "ZenDiscordConfig.json";
	private const static string CURRENT_VERSION = "2";
	string CONFIG_VERSION = "";

	// Settings data
	ref array<string> AdminUIDs;
	ref array<string> AdminWebhooks;
	ref array<string> KillFeedWebhooks;
	string ServerName = "HardcoreDayZ";
	string PingAdminCommand = "admin";
	string MessageReceived = "Your message has been successfully relayed to the admins. Please be patient for their response.";
	string JustLoggedIn = "just logged in to";
	string Explanation = "A player on your watchlist just connected.";
	string KillFeed = "Killfeed";
	string Killed = "killed";
	string With = "with";
	string Melee = "melee";
	string KillerAge = "Killer Age";
	string VictimAge = "Victim Age";
	string Minutes = "minutes";
	string Hours = "hours";
	bool EnableDiscordKillFeed = true;
	bool DisplayKillLocation = true;
	bool DisplayPlayerSteamID = true;
	bool TriggerAdminRaidAlert = true;
	string MapURL = "";
	bool RaidAlarmRequiresTerritory = false;
	bool RaidStationInvincible = true;
	bool RaidRadarDishInvincible = true;
	int RaidRadarDishCheckRoofTimerMinutes = -1; // -1 = off, 0 = only check on server startup, > 0 = minutes between checks
	int RaidDetectionDistance = 150;
	float RaidAlarmResetInMinutes = 3;
	float RaidAlarmBatteryUsagePerSec = 1.0;
	bool RaidPlaySound = true;
	bool RaidDiscordAlertRequiresRadar = false;
	int RaidDiscordAlertCooldownSecs = 1;
	string RaidAlarmMessageTitle = "RAID ALARM";
	string RaidAlarmAttackMessage = "Your base is under attack!";
	string RaidAlarmWebhookUpdate = "Raid alarm configuration has been changed.";
	string RaidAlarmConnectionOnline = "Connection established - monitoring for raid activity.";
	string RaidAlarmConnectionOffline = "Connection lost - raid activity is no longer being monitored.";
	string RaidAlarmRequiresTerritoryText = "You need to be the member of a territory to modify this raid alarm!";
	string RaidAlarmTooClose = "This raid alarm is too close to an existing raid alarm!";
	string RaidAlarmConnectionObstruction = "The radar dish is obstructed! Check for objects blocking its view of the sky.";
	string RaidAlarmBatteryWarning = "Battery Charge";
	bool BaseDamageTriggersRaidAlert = true;
	bool BaseDismantleTriggersRaidAlert = true;
	ref array<string> RaidAlarmBuildTools;
	ref array<string> RaidAlarmDismantleTools;
	ref array<string> ItemsDeployedTriggerRaidAlert;
	bool RequireAnItemForIngameAlert = false;
	ref map<string, bool> ItemsRequiredForIngameAlert;
	ref map<string, string> PlayerWatchlist;

	void Load()
	{
		RaidAlarmBuildTools = new array<string>;
		RaidAlarmDismantleTools = new array<string>;

		if (GetGame().IsClient())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenDiscordConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			if (!RaidAlarmBuildTools || RaidAlarmBuildTools.Count() == 0)
			{
				SetDefaultBuildTools();
			}

			if (!RaidAlarmDismantleTools || RaidAlarmDismantleTools.Count() == 0)
			{
				SetDefaultDismantleTools();
			}

			if (CONFIG_VERSION != CURRENT_VERSION)
			{
				JsonFileLoader<ZenDiscordConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
				CONFIG_VERSION = CURRENT_VERSION;
				Save();
			}
			else
			{
				Save();

				// Convert all arrays to lower case 
				string entry;
				int i;
				for (i = 0; i < ItemsDeployedTriggerRaidAlert.Count(); i++)
				{
					entry = ItemsDeployedTriggerRaidAlert.Get(i);
					entry.ToLower();
					ItemsDeployedTriggerRaidAlert.Set(i, entry);
				}

				for (i = 0; i < RaidAlarmBuildTools.Count(); i++)
				{
					entry = RaidAlarmBuildTools.Get(i);
					entry.ToLower();
					RaidAlarmBuildTools.Set(i, entry);
				}

				for (i = 0; i < RaidAlarmDismantleTools.Count(); i++)
				{
					entry = RaidAlarmDismantleTools.Get(i);
					entry.ToLower();
					RaidAlarmDismantleTools.Set(i, entry);
				}

				// Config exists and version matches, stop here.
				return;
			}
		}

		CONFIG_VERSION = CURRENT_VERSION;

		SetDefaultValues();

		Save();
	}

	void SetDefaultValues()
	{
		// Prepare default config
		AdminUIDs = new array<string>;
		AdminWebhooks = new array<string>;
		KillFeedWebhooks = new array<string>;
		PlayerWatchlist = new map<string, string>;

		// List of admin ids 
		AdminUIDs.Insert("4s_EXAMPLE_pKYemc7adlZyKSrwzIMW0T32Q69CerkE");

		// If any of these player IDs log in it will trigger an alert to the admin discord webhook
		AdminWebhooks.Insert("https://discord.com/api/webhooks/1263661792194728040/AEWqPnfHW8h8sIJuFBKsjW1mtOHFANXtWQVh3oquwcevG0aO2CyowmP4IQuKLqOFHJno");
		KillFeedWebhooks.Insert("https://discord.com/api/webhooks/1015532391575592970/YDLhQjuwSxVagEnTR2jLRVCeNBK415gtfoCPIUrHlxXSjHcrLSbJEH2F2KLhc5aGXETm");
		PlayerWatchlist.Set("4s_12UDE-PKYemc7adlZyKGVSrwzIMW0T32Q69CerkE=", "Example reason they're on watchlist - potential cheater, accused by multiple players etc");
		
		// If any of these items are detected being deployed it will trigger raid alarm
		ItemsDeployedTriggerRaidAlert = new array<string>;
		ItemsDeployedTriggerRaidAlert.Insert("HDSN_BreachingCharge");
		ItemsDeployedTriggerRaidAlert.Insert("expansionsatchel");
		ItemsDeployedTriggerRaidAlert.Insert("plastic_explosive");

		// map = itemType, bool: bool is requires battery working & on
		ItemsRequiredForIngameAlert = new map<string, bool>;
		ItemsRequiredForIngameAlert.Insert("personalradio", true);
		ItemsRequiredForIngameAlert.Insert("baseradio", true);
		ItemsRequiredForIngameAlert.Insert("itempda", true);
		ItemsRequiredForIngameAlert.Insert("zenwatchgps", true);

		SetDefaultBuildTools();
		SetDefaultDismantleTools();

		// Get map name
		string mapName = GetGame().GetWorldName();
		mapName.ToLower();
		if (mapName == "enoch")
			MapURL = "https://www.izurvive.com/livonia/";
		else 
			MapURL = "https://www.izurvive.com/" + mapName + "/";
	}

	void SetDefaultBuildTools()
	{
		RaidAlarmBuildTools = new array<string>;
		RaidAlarmBuildTools.Insert("hands");
		RaidAlarmBuildTools.Insert("screwdriver");
	}

	void SetDefaultDismantleTools()
	{
		RaidAlarmDismantleTools = new array<string>;
		RaidAlarmDismantleTools.Insert("hands");
		RaidAlarmDismantleTools.Insert("screwdriver");
	}

	void Save()
	{
		if (!FileExist(zenModFolderPrenested))
		{
			MakeDirectory(zenModFolderPrenested);
		}

		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenDiscordConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	string GetMapLinkPosition(vector pos, string baseName = "")
	{
		string mapLink = "";

		if (MapURL == "")
		{
			if (baseName != "")
			{
				mapLink = baseName + " @ " + pos[0] + " / " + pos[2];
			}
			else 
			{
				mapLink = "@ " + pos[0] + " / " + pos[2];
			}
		}
		else 
		{
			if (baseName != "")
			{
				"[" + baseName + " @ " + pos[0] + " / " + pos[2] + "](" + MapURL + "#location=" + pos[0] + ";" + pos[2] + ")";
			}
			else 
			{
				mapLink = "[@ " + pos[0] + " / " + pos[2] + "](" + MapURL + "#location=" + pos[0] + ";" + pos[2] + ")";
			}
		}

		return mapLink;
	}
}

ref ZenDiscordConfig m_ZenDiscordConfig;

static ZenDiscordConfig GetZenDiscordConfig()
{
	if (!m_ZenDiscordConfig)
	{
		m_ZenDiscordConfig = new ZenDiscordConfig();
		m_ZenDiscordConfig.Load();
	}

	return m_ZenDiscordConfig;
}