modded class PlayerBase
{
	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		AddAction(ActionBuildZenRaidAlarmStation, InputActionMap);
		AddAction(ActionDismantleZenRaidAlarmRadar, InputActionMap);
		AddAction(ActionDismantleZenRaidAlarmStation, InputActionMap);
	}

	// Override this for things like Syberia mod to add character name
	string GetZenKillFeedName()
	{
		return GetCachedName();
	}

	// Send kill feed info
	void ZenKillFeed(notnull PlayerBase playerKiller, Object object)
	{
		if (!GetZenDiscordConfig().EnableDiscordKillFeed || !object)
			return;

		vector myPos = GetPosition();
		string killerText = playerKiller.GetZenKillFeedName();
		string victimText = GetZenKillFeedName();
		string location = GetZenDiscordConfig().GetMapLinkPosition(myPos);
		string distance = MiscGameplayFunctions.TruncateToS(vector.Distance(myPos, playerKiller.GetPosition()));

		if (GetZenDiscordConfig().DisplayPlayerSteamID)
		{
			killerText = "[" + playerKiller.GetZenKillFeedName() + "](http://steamcommunity.com/profiles/" + playerKiller.GetIdentity().GetPlainId() + ")";
			victimText = "[" + GetZenKillFeedName() + "](http://steamcommunity.com/profiles/" + GetIdentity().GetPlainId() + ")";
		}

		string weaponText = "";
		if (object.GetType() == playerKiller.GetType())
			weaponText = GetZenDiscordConfig().Melee;
		weaponText = object.GetDisplayName();

		int victimPlayMinutes = StatGet(AnalyticsManagerServer.STAT_PLAYTIME) / 60;
		int killerPlayerMinutes = playerKiller.StatGet(AnalyticsManagerServer.STAT_PLAYTIME) / 60;
		string victimAge = GetZenDiscordConfig().VictimAge + ": ";
		string killerAge = GetZenDiscordConfig().KillerAge + ": ";
		if (victimPlayMinutes > 60)
			victimAge = victimAge + (victimPlayMinutes / 60) + " " + GetZenDiscordConfig().Hours;
		else
			victimAge = victimAge + (victimPlayMinutes)+" " + GetZenDiscordConfig().Minutes;

		if (killerPlayerMinutes > 60)
			killerAge = killerAge + (killerPlayerMinutes / 60) + " " + GetZenDiscordConfig().Hours;
		else
			killerAge = killerAge + (killerPlayerMinutes)+" " + GetZenDiscordConfig().Minutes;

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