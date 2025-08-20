class Zen_RaidAlarmStationKit extends ZenKitBoxBase_RaidAlarm
{
	override string GetDeployedClassname()
	{
		return "Zen_RaidAlarmStation";
	}

	override vector GetDeployOrientationOffset()
	{
		return "0 0 0";
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}
}