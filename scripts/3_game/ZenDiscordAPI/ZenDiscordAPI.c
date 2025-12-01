class ZenDiscordAPI extends Managed
{
	/*
	PROBLEM: 
	
	In some countries the webhook context is: https://discord.com/api/webhooks/
	In other countries it's something like: https://discordapp.com/api/webhooks/
	So we can't just use a static DISCORD_API context here. We have to derive it from the webhook
	the player pastes into the alarm.

	static const string DISCORD_API = "https://discord.com/api/webhooks/";
	*/

	static const string DISCORD_API = "/api/webhooks/";

	void ZenDiscordAPI()
	{
		if (!GetRestApi())
			CreateRestApi();
	}

	void SendMessage(notnull ZenDiscordMessage msg, vector alarmPosition = vector.Zero)
	{
		ZenDiscordWebhookCB cb = new ZenDiscordWebhookCB();
        string json = msg.GetJSON();
        cb.SetJSON(json);

        for (int i = 0; i < msg.GetWebhooks().Count(); i++)
        {
            string full = msg.GetWebhooks().Get(i); // full webhook URL
            cb.SetWebhook(full);

            int idx = full.IndexOf(DISCORD_API);
            if (idx < 0)
            {
                Print("[ZenDiscordAPI] Invalid Discord webhook URL: " + full);
                continue;
            }

            int baseEnd = idx + DISCORD_API.Length();

            // Base URL, e.g.:
            //   https://discord.com/api/webhooks/
            //   https://discordapp.com/api/webhooks/
            //   https://ptb.discord.com/api/webhooks/
            //   https://canary.discord.com/api/webhooks/
            string baseUrl = full.Substring(0, baseEnd);

            // Remainder, e.g. "1234567890/token[/github]"
            string route = full.Substring(baseEnd, full.Length() - baseEnd);

            RestContext ctx = GetRestApi().GetRestContext(baseUrl);
            ctx.SetHeader("application/json");
            ctx.POST(cb, route, json);
        }

        delete msg; // msg is a ref so delete it once no longer needed.
	}

	void SendAdminMessage(string title, string message)
	{
		// Must use ref here or msg gets gc'ed before it's sent for some reason?
		ref ZenDiscordMessage msg = new ZenDiscordMessage(title);
		msg.SetTitle(title);
		msg.SetMessage(message);
		msg.SetColor(255, 255, 255);
		msg.AddWebhooks(GetZenDiscordConfig().AdminWebhooks);
		GetZenDiscordAPI().SendMessage(msg);
	}
}

static ref ZenDiscordAPI m_ZenDiscordAPI;

static ZenDiscordAPI GetZenDiscordAPI()
{
	if (!m_ZenDiscordAPI)
		m_ZenDiscordAPI = new ZenDiscordAPI;

	return m_ZenDiscordAPI;
}