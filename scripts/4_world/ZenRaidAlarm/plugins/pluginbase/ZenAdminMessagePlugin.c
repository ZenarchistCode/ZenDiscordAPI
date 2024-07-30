//! Handles all "!chatmsg" text input from client.
class ZenAdminMessagePlugin extends PluginBase
{
    protected float m_DeltaTime = 0;

    bool HandleAdminCommand(notnull PlayerIdentity identity, string text)
    {
        Print("[ZenAdminMessagePlugin] Received admin command: " + text + " from: " + identity.GetId());

        // Get player object
        int highBits, lowBits;
		GetGame().GetPlayerNetworkIDByIdentityID(identity.GetPlayerId(), lowBits, highBits);
		PlayerBase player = PlayerBase.Cast(GetGame().GetObjectByNetworkId(lowBits, highBits));
        if (!player)
        {
            Error("[ZenDiscordAPI] Failed to retrieve player network object in ZenAdminMessagePlugin.c from uid -> " + identity.GetId());
            return false;
        }

        // Strip '!'
        text = text.Substring(1, text.Length() - 1);
        string message = "";

        // Get message/parameters if they exist
        int spaceIndex = text.IndexOf(" ");
        if (spaceIndex != -1)
        {
            // Calculate the length of the message substring
            int messageLength = text.Length() - (spaceIndex + 1);
            message = text.Substring(spaceIndex + 1, messageLength);
        }

        // Get command
        string command;
        if (spaceIndex != -1)
        {
            command = text.Substring(0, spaceIndex);
        }
        else
        {
            command = text;
        }

        command.ToLower();
        array<string> params = new array<string>;
        string parameters = message;
        parameters.ToLower();
        parameters.Split(" ", params);

        // Act on command if recognized
        if (ProcessCommand(player, command, message, params))
            return true;

        Print("[ZenAdminMessagePlugin] Message was not handled.");
        return false;
    }

    // Process regular civilian commands
    bool ProcessCommand(notnull PlayerBase player, string command, string text, array<string> params)
    {
        if (command == "")
            return false;

        // Check admin cmds first
        if (IsAdmin(player.GetCachedID()))
        {
            if (ProcessAdminCommand(player, command, text, params))
                return true;
        }

        // !admin command
        if (command == GetZenDiscordConfig().PingAdminCommand)
        {
            string profileName = player.GetIdentity().GetName();
            string uid = player.GetCachedID();
            string steamid = player.GetIdentity().GetPlainId();
            string from = "[" + profileName + "](http://steamcommunity.com/profiles/" + steamid + ")\n" + uid + "\n" + steamid;
            string status = GetZenDiscordConfig().GetMapLinkPosition(player.GetPosition()) + "\nHP=" + (player.GetHealth01() * 100) + "%";

            string title = command;
            title.ToUpper();

            // Build discord notification string
            ZenDiscordMessage msg = new ZenDiscordMessage(title);
		    msg.SetTitle(title);
		    msg.SetMessage(text + "\n\n" + from + "\n\n" + status);
		    msg.SetColor(255, 160, 0);
		    msg.AddWebhooks(GetZenDiscordConfig().AdminWebhooks);
		    GetZenDiscordAPI().SendMessage(msg);

            // Notify player
			MsgPlayer(player, GetZenDiscordConfig().MessageReceived);
            return true;
        }

        return false;
    }

    // Process admin commands
    bool ProcessAdminCommand(notnull PlayerBase player, string command, string text, array<string> params)
    {
        // Tele
        if (command == "tele")
        {
            vector teleSpot = player.GetPosition();

            if (params.Count() == 2)
            {
                float x = params.Get(0).ToFloat();
                float z = params.Get(1).ToFloat();
                teleSpot[0] = x;
                teleSpot[2] = z;
                teleSpot[1] = GetGame().SurfaceY(x, z);
            }
            else
            if (params.Count() == 3)
            {
                teleSpot = text.ToVector();
            }
            else
            {
                teleSpot = "0 0 0";
            }

            if (teleSpot[0] == 0 && teleSpot[2] == 0)
            {
                MsgPlayer(player, "Incorrect syntax - use !tele X Z or !tele X Y Z");
                return true;
            }

            MsgPlayer(player, "Teleporting to " + teleSpot);
            player.SetPosition(teleSpot);
            return true;
        }

        return false;
    }

    // Trigger ProcessUpdate() once per second
    override void OnUpdate(float delta_time)
    {
        m_DeltaTime += delta_time;
        if (m_DeltaTime < 1)
            return;

        m_DeltaTime = 0;
        ProcessUpdate();
    }
    
    // Override this to do anything with this plugin, unless you need milisecond precision then override above
    void ProcessUpdate() {};

    // Sends a text message to the given player
    protected void MsgPlayer(notnull PlayerBase player, string msg)
    {
        GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>(msg), true, player.GetIdentity()); 
    }

    // Check if given player id is an admin
    bool IsAdmin(string uid)
    {
        for (int i = 0; i < GetZenDiscordConfig().AdminUIDs.Count(); i++)
        {
            if (GetZenDiscordConfig().AdminUIDs.Get(i) == uid)
                return true;
        }

        return true;
    }
}