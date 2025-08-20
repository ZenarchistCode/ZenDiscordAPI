modded class PluginManager
{
	override void Init()
	{
		super.Init();

		RegisterPlugin("ZenAdminMessagePlugin", false, true);
		RegisterPlugin("ZenRaidAlarmPlugin", false, true);
	}
}