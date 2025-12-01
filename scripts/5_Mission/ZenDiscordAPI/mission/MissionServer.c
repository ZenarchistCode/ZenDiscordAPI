modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		//! DISCORD API
		GetZenDiscordConfig();
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity) 
	{
		super.InvokeOnConnect(player, identity);

		if (!identity || !player)
			return;

		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenDiscordConfig", new Param2<array<string>, array<string>>(GetZenDiscordConfig().RaidAlarmBuildTools, GetZenDiscordConfig().RaidAlarmDismantleTools), true, identity);

		//! DISCORD WATCHLIST
		string reason;
		if (GetZenDiscordConfig().PlayerWatchlist.Find(identity.GetId(), reason))
		{
			ZenDiscordMessage playerWatchlistMsg = new ZenDiscordMessage("Watchlist");
			playerWatchlistMsg.SetTitle(player.GetCachedName() + " " + GetZenDiscordConfig().JustLoggedIn + " " + GetZenDiscordConfig().ServerName);
			playerWatchlistMsg.SetMessage(GetZenDiscordConfig().Explanation + "\n\n" + reason + "\n\n" + GetZenDiscordConfig().GetMapLinkPosition(player.GetPosition()) + "\n\n" + identity.GetId() + "\n\n[" + identity.GetPlainId() + "](http://steamcommunity.com/profiles/" + identity.GetPlainId() + ")");
			playerWatchlistMsg.SetColor(255, 255, 0);

			foreach (string s : GetZenDiscordConfig().AdminWebhooks)
			{
				playerWatchlistMsg.AddWebhook(s);
			}
				
			GetZenDiscordAPI().SendMessage(playerWatchlistMsg);
		}
	}
}