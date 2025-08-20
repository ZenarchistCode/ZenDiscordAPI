modded class PluginRecipesManagerBase
{
    override void RegisterRecipies()
    {
        super.RegisterRecipies();

        //! RAID ALARM
        RegisterRecipe(new Zen_CraftZenRaidAlarmStationKit);
    }
}