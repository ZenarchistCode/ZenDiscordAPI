#ifndef ZENMODPACK
class ZenFunctions
{
	//! Returns a readable time string based on the seconds
	static string GetTimeToString(int totalSeconds, bool showHours = true, bool showMinutes = true, bool showSeconds = true)
	{
		string formattedTime = "";

		int days = totalSeconds / 86400;        // 86400 seconds in a day
		int remainingSeconds = totalSeconds - (days * 86400);

		int hours = remainingSeconds / 3600;    // 3600 seconds in an hour
		remainingSeconds = remainingSeconds - (hours * 3600);

		int minutes = remainingSeconds / 60;    // 60 seconds in a minute
		int seconds = remainingSeconds - (minutes * 60);

		if (days > 0)
		{
			formattedTime += days.ToString() + " #STR_ZenRaidAlarmGui_Days";
		}

		if (showHours && hours > 0)
		{
			if (formattedTime != "")
				formattedTime += " ";

			formattedTime += hours.ToString() + " #STR_ZenRaidAlarmGui_Hours";
		}

		// Optionally include minutes and seconds
		if (showMinutes && minutes > 0)
		{
			if (formattedTime != "")
				formattedTime += " ";

			formattedTime += minutes.ToString() + " #STR_ZenRaidAlarmGui_Minutes";
		}

		if (showSeconds && seconds > 0)
		{
			if (formattedTime != "")
				formattedTime += " ";

			formattedTime += seconds.ToString() + " #STR_ZenRaidAlarmGui_Seconds";
		}

		return formattedTime;
	}

	
	//! Debug message - sends a server-side player message to all online players
	static void SendGlobalMessage(string msg)
	{
		SendGlobalMessageEx(msg, "[SERVER] ");
	}

	//! Debug message - sends a server-side player message to all online players
	static void SendGlobalMessageEx(string msg, string prefix = "")
	{
		#ifdef SERVER
		array<Man> players = new array<Man>;
		GetGame().GetWorld().GetPlayerList(players);
		for (int x = 0; x < players.Count(); x++)
		{
			PlayerBase pb = PlayerBase.Cast(players.Get(x));
			if (pb)
			{
				SendPlayerMessage(pb, prefix + msg);
			}
		}
		#endif
	}

	//! Display client message ONLY on client
	static void ZenClientMessage(string message)
	{
#ifndef SERVER
		if (GetGame().GetPlayer())
		{
			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", message, ""));
		}
#endif
	}

	//! Print a debug chat message both client-side & server-side
	static void DebugMessage(string message)
	{
		ZenClientMessage("[CLIENT] " + message);
		SendGlobalMessage(message);
	}

	//! For client-only error message
	static void ZenClientError(string message)
	{
		ZenClientMessage("[CLIENT ERROR] " + message);
		Error("[ZENMODPACK CLIENT ERROR] " + message);
	}

	//! Send a message to the given player
	static void SendPlayerMessage(PlayerBase player, string msg)
	{
#ifdef SERVER
		if (msg == "" || msg == string.Empty)
			return;

		if (!player || player.IsPlayerDisconnected() || !player.GetIdentity())
			return;

		Param1<string> m_MessageParam = new Param1<string>("");
		if (m_MessageParam && msg != "")
		{
			m_MessageParam.param1 = msg;
			GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, player.GetIdentity());
		}
#endif
	}
}
#endif