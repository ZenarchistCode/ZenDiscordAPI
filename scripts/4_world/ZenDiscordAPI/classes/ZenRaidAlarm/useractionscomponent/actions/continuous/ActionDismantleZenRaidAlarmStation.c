class ActionDismantleZenRaidAlarmStation: ActionContinuousBase
{
	void ActionDismantleZenRaidAlarmStation()
	{
		m_CallbackClass = ActionBuildZenRaidAlarmStationCB;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_RESTRAINTARGET;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_FullBody = true;
		m_Text = "#dismantle #STR_CfgVehicles_ZenRaidAlarmStation0";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursorNoRuinCheck(UAMaxDistances.SMALL);
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ItemBase itemInHands = player.GetItemInHands();

		if (!itemInHands || itemInHands.IsRuined())
		{
			if (GetZenDiscordConfig().RaidAlarmDismantleTools.Find("hands") == -1)
			{
				return false;
			}
		}
		else 
		{
			bool foundValidItem = false;

			for (int i = 0; i < GetZenDiscordConfig().RaidAlarmDismantleTools.Count(); i++)
			{
				string checkType = GetZenDiscordConfig().RaidAlarmDismantleTools.Get(i);
				
				if (itemInHands.IsKindOf(checkType))
				{
					foundValidItem = true;
					break;
				}
			}

			if (!foundValidItem)
			{
				return false;
			}
		}

		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(target.GetObject());
		if (station)
		{
			BaseRadio radio = station.GetBaseRadio();
			if (radio != NULL && radio.IsLockedInSlot() && !station.GetCompEM().IsWorking())
			{
				return true;
			}
		}

		return false;
	}
	
	override void OnStartAnimationLoop(ActionData action_data)
	{
		super.OnStartAnimationLoop(action_data);

		#ifdef SERVER
		if (!action_data.m_MainItem)
			return;

		ItemBase target_IB = ItemBase.Cast(action_data.m_Target.GetObject());
		Param2<bool, string> play = new Param2<bool, string>(true, action_data.m_MainItem.GetLockSoundSet());
		GetGame().RPCSingleParam(target_IB, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true);
		#endif
	}

	override void OnEnd(ActionData action_data)
	{
		#ifdef SERVER
		if (!action_data.m_MainItem)
			return;

		ItemBase target_IB = ItemBase.Cast(action_data.m_Target.GetObject());
		Param2<bool, string> play = new Param2<bool, string>(false, action_data.m_MainItem.GetLockSoundSet());
		GetGame().RPCSingleParam(target_IB, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true);
		#endif
	}

	override void OnEndAnimationLoop(ActionData action_data)
	{
		super.OnEndAnimationLoop(action_data);

		#ifdef SERVER
		if (!action_data.m_MainItem)
			return;

		ItemBase target_IB = ItemBase.Cast(action_data.m_Target.GetObject());
		Param2<bool, string> play = new Param2<bool, string>(false, action_data.m_MainItem.GetLockSoundSet());
		GetGame().RPCSingleParam(target_IB, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true);
		#endif
	}
	
	override void OnFinishProgressServer(ActionData action_data)
	{
		if (action_data.m_MainItem)
		{
			MiscGameplayFunctions.DealAbsoluteDmg(action_data.m_MainItem, action_data.m_MainItem.GetMaxHealth() * 0.5);
		}
		
		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(action_data.m_Target.GetObject());
		if (station)
			station.LockBaseRadio(false);

		Print("[ZenRaidAlarm] " + action_data.m_Player.GetIdentity().GetId() + " dismantled raid alarm station @ " + action_data.m_Player.GetPosition());
	}
}