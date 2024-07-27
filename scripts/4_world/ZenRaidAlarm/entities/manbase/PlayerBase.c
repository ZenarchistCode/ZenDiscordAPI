modded class PlayerBase
{
	// Override this for things like Syberia mod to add character name
	string GetZenKillFeedName()
	{
		return GetCachedName();
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		//! DISCORD KILL FEED
		if (!GetZenDiscordConfig().EnableDiscordKillFeed)
			return;

		// Expansion & Syberia compatibility
		if (GetType().Contains("eAI") || GetType().Contains("_Ghost"))
			return;

		if (!GetIdentity())
			return;

		EntityAI killerAI = EntityAI.Cast(killer);
		if (!killerAI)
			return;

		if (killerAI.GetType().Contains("eAI") || killerAI.IsZombie())
			return;

		// Check player melee kill & guns through HierarchyRootPlayer
		PlayerBase playerKiller = PlayerBase.Cast(killerAI);
		if (!playerKiller)
			playerKiller = PlayerBase.Cast(killerAI.GetHierarchyRootPlayer());

		if (!playerKiller)
		{
			// Check for grenades 
			Grenade_Base grenade = Grenade_Base.Cast(killerAI);
			if (grenade != NULL)
				playerKiller = PlayerBase.Cast(grenade.Zen_GetUnpinPlayer());

			// Check for traps 
			if (!playerKiller)
			{
				TrapBase trap = TrapBase.Cast(killerAI);
				if (trap != NULL)
					playerKiller = PlayerBase.Cast(trap.Zen_GetPlayerTrapper());
			}

			// Check for vehicles
			if (!playerKiller)
			{
				CarScript vehicle = CarScript.Cast(killerAI);
				if (vehicle != NULL)
				{
					for (int index = 0; index < vehicle.CrewSize(); index++)
					{
						if (vehicle.CrewMember(index) != NULL && vehicle.GetSeatAnimationType(index) == DayZPlayerConstants.VEHICLESEAT_DRIVER)
							playerKiller = PlayerBase.Cast(vehicle.CrewMember(index));
					}
				}
			}

			//! TODO: ChemGas and projectile explosions/crossbow...
		}

		if (!playerKiller || !playerKiller.GetIdentity() || playerKiller == this)
			return;

		// We have a valid killer and a valid victim, notify discord
		vector myPos = GetPosition();
		string killerText = playerKiller.GetZenKillFeedName();
		string victimText = GetZenKillFeedName();
		string location = GetZenDiscordConfig().GetMapLinkPosition(myPos);
		string distance = MiscGameplayFunctions.TruncateToS(vector.Distance(myPos, playerKiller.GetPosition()));

		if (GetZenDiscordConfig().DisplayPlayerSteamID)
		{
			killerText = "[" + playerKiller.GetZenKillFeedName()  + "](http://steamcommunity.com/profiles/" + playerKiller.GetIdentity().GetPlainId() + ")";
			victimText = "[" + GetZenKillFeedName()  + "](http://steamcommunity.com/profiles/" + GetIdentity().GetPlainId() + ")";
		}

		string weaponText = "";
		if (killer.GetType() == playerKiller.GetType())
			weaponText = GetZenDiscordConfig().Melee;
		weaponText = killer.GetDisplayName();

		int victimPlayMinutes = StatGet(AnalyticsManagerServer.STAT_PLAYTIME) / 60;
		int killerPlayerMinutes = playerKiller.StatGet(AnalyticsManagerServer.STAT_PLAYTIME) / 60;
		string victimAge = GetZenDiscordConfig().VictimAge + ": ";
		string killerAge = GetZenDiscordConfig().KillerAge + ": ";
		if (victimPlayMinutes > 60)
			victimAge = victimAge + (victimPlayMinutes / 60) + " " + GetZenDiscordConfig().Hours;
		else 
			victimAge = victimAge + (victimPlayMinutes) + " " + GetZenDiscordConfig().Minutes;

		if (killerPlayerMinutes > 60)
			killerAge = killerAge + (killerPlayerMinutes / 60) + " " + GetZenDiscordConfig().Hours;
		else 
			killerAge = killerAge + (killerPlayerMinutes) + " " + GetZenDiscordConfig().Minutes;

		string discordMsg = killerText + " " + GetZenDiscordConfig().Killed + " ";
		discordMsg = discordMsg + victimText + " " + GetZenDiscordConfig().With + " " + weaponText + " " + distance + "m ";
		if (GetZenDiscordConfig().DisplayKillLocation)
			discordMsg = discordMsg + location + " ";
		discordMsg = discordMsg + "\n\n" + victimAge + "\n" + killerAge;

		if (GetZenDiscordConfig().DisplayPlayerSteamID)
		{
			discordMsg = discordMsg + "\n\nVictim: " + GetCachedID();
			discordMsg = discordMsg + "\nKiller: " + playerKiller.GetCachedID();
		}

		// Send discord webhook message
		ZenDiscordMessage msg = new ZenDiscordMessage(GetZenDiscordConfig().KillFeed);
		msg.SetTitle(GetZenDiscordConfig().KillFeed);
		msg.SetMessage(discordMsg);
		msg.SetColor(255, 255, 255);
		msg.AddWebhooks(GetZenDiscordConfig().KillFeedWebhooks);
		GetZenDiscordAPI().SendMessage(msg);
	}
}