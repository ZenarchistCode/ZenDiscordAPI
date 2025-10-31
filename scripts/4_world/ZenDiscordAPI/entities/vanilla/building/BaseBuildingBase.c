modded class BaseBuildingBase
{
	//! RAID ALERT - Damage alarm
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
			return false;

		#ifdef ZENMODPACK
		if (!ZenModEnabled("ZenRaidAlarm"))
			return true;
		#endif

		if (GetZenDiscordConfig().BaseDamageTriggersRaidAlert)
		{
			TriggerZenRaidAlarm();
		}

		return true;
	}

	//! RAID ALERT - Dismantle alarm
	override void OnPartDismantledServer(notnull Man player, string part_name, int action_id)
	{
		super.OnPartDismantledServer(player, part_name, action_id);

		#ifdef ZENMODPACK
		if (!ZenModEnabled("ZenRaidAlarm"))
			return;
		#endif

		if (GetZenDiscordConfig().BaseDismantleTriggersRaidAlert)
		{
			PlayerBase pb = PlayerBase.Cast(player);
			if (pb != NULL)
			{
				if (!Zen_RaidAlarmStation.GetTerritoryPermission(pb.GetCachedID(), pb.GetPosition()))
				{
					TriggerZenRaidAlarm();
				}
			}
		}
	}
}