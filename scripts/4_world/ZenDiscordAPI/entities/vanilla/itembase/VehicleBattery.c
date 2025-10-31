modded class VehicleBattery
{
	//! UPDATE RAID STATION ENERGY WHEN BATTERY IS ATTACHED TO BASE RADIO
	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);

#ifdef SERVER
		UpdateRaidStationEnergy(parent);
#endif
	}

	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		super.OnWasDetached(parent, slot_id);

#ifdef SERVER
		UpdateRaidStationEnergy(parent);
#endif
	}

	void UpdateRaidStationEnergy(EntityAI parent)
	{
		BaseRadio radio = BaseRadio.Cast(parent);
		if (!radio)
			return;

		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(radio.GetHierarchyParent());
		if (!station)
			return;

		station.UpdateBatteryEnergy();
	}
}