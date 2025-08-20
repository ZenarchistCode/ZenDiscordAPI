modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);

		actions.Insert(ActionTurnOffZenRaidAlarm);
		actions.Insert(ActionTurnOnZenRaidAlarm);
		actions.Insert(ActionViewRaidAlarmGUI);
		actions.Insert(ActionBuildZenRaidAlarmStation);
		actions.Insert(ActionDismantleZenRaidAlarmStation);
		actions.Insert(ActionDismantleZenRaidAlarmRadar);
		actions.Insert(ActionFoldZenRaidAlarmKit);
	}
}