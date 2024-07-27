modded class MissionBase
{
	//! SHARED 
	void MissionBase()
	{
		#ifdef SERVER
		// SERVER RECEIVE RPC

		//! RAID ALARM
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenRaidAlarmClientWebhooks", this, SingeplayerExecutionType.Server);
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenAdminCommandMessage", this, SingeplayerExecutionType.Server);
		#else
		// CLIENT RECEIVE RPC

		//! RAID ALARM
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenRaidAlarmServerWebhooks", this, SingeplayerExecutionType.Client);
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenAdminCommandMessageFailed", this, SingeplayerExecutionType.Client);
		#endif
	}

    // Client -> server :: receive potential admin command
	void RPC_ReceiveZenAdminCommandMessage(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Server && GetGame().IsDedicatedServer())
        {
            Param1<string> data;
            if (!ctx.Read(data))
            {
                Error("Error receiving data - RPC_ReceiveZenAdminCommandMessage");
                return;
            }

            if (sender && data.param1)
            {
                ZenAdminMessagePlugin plugin = ZenAdminMessagePlugin.Cast(GetPlugin(ZenAdminMessagePlugin));
                if (plugin)
                {
                    if (plugin.HandleAdminCommand(sender, data.param1))
                        return;
                }

                // If we failed to stop @ plugin.HandleAdminCommand, relay this message back to client
                GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenAdminCommandMessageFailed", new Param1<string>(data.param1), true, sender);
            }
        }
    }

    // Client -> server :: receive webhooks on server
	void RPC_ReceiveZenRaidAlarmClientWebhooks(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Server && GetGame().IsDedicatedServer())
        {
            Param3<array<string>, Zen_RaidAlarmStation, string> data;
            if (!ctx.Read(data))
            {
                Error("Error receiving data - RPC_ReceiveZenRaidAlarmClientWebhooks");
                return;
            }

            if (data.param1.Count() != 3)
            {
                Error("Error sync'ing client-side RaidStation data to server - RPC_ReceiveZenRaidAlarmClientWebhooks - count=" + data.param1.Count());
                return;
            }

            if (!data.param2)
            {
                Error("Error sync'ing client-side RaidStation data to server - RPC_ReceiveZenRaidAlarmClientWebhooks");
                return;
            }

            if (sender && data.param1 && data.param2)
            {
                data.param2.SetWebhooks(data.param1, sender.GetName());
                data.param2.SetBaseName(data.param3);
            }
        }
    }

    // Server -> client :: receive failed admin command and relay it back to chat
	void RPC_ReceiveZenAdminCommandMessageFailed(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Client && GetGame().IsClient())
        {
            Param1<string> data;
            if (!ctx.Read(data))
            {
                Error("Error receiving data - RPC_ReceiveZenAdminCommandMessageFailed");
                return;
            }

            if (data.param1)
            {
                GetGame().ChatPlayer(data.param1);
            }
        }
    }

    // Server -> client :: receive webhooks on client
    void RPC_ReceiveZenRaidAlarmServerWebhooks(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Client && GetGame().IsClient())
        {
            Param3<array<string>, Zen_RaidAlarmStation, string> data;
            if (!ctx.Read(data))
            {
                Error("Error sync'ing server-side data to client - RPC_ReceiveZenRaidAlarmServerWebhooks");
                return;
            }

            if (data.param1.Count() != 3)
            {
                Error("Error sync'ing server-side webhook data to client - RPC_ReceiveZenRaidAlarmServerWebhooks - count=" + data.param1.Count());
                return;
            }

            if (!data.param2)
            {
                Error("Error sync'ing server-side Raid Station data to client - RPC_ReceiveZenRaidAlarmServerWebhooks");
                return;
            }

            if (GetGame().GetUIManager() != NULL)
            {
                ZenRaidAlarmGUI gui = ZenRaidAlarmGUI.Cast(GetGame().GetUIManager().EnterScriptedMenu(ZenDiscordMenus.RAID_ALARM_GUI, NULL));
                if (gui)
                    gui.OnReceivedServerData(data.param1.Get(0), data.param1.Get(1), data.param1.Get(2), data.param2, data.param3);
            }
        }
    }

	override UIScriptedMenu CreateScriptedMenu(int id)
    {
        UIScriptedMenu menu = super.CreateScriptedMenu(id);

        if (!menu)
        {
            switch (id)
            {
				//! RAID ALARM
                case ZenDiscordMenus.RAID_ALARM_GUI:
                {
                    menu = new ZenRaidAlarmGUI;
                    break;
                }
            }
        }

        return menu;
    }
}