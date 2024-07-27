modded class Hologram
{
	override string ProjectionBasedOnParent()
    {
        //! RAID ALARM
        Zen_RaidAlarmStationKit station_kit = Zen_RaidAlarmStationKit.Cast(m_Parent);
		if (station_kit != NULL)
			return station_kit.GetDeployedClassname();

        Zen_RaidAlarmRadarKit radar_kit = Zen_RaidAlarmRadarKit.Cast(m_Parent);
		if (radar_kit != NULL)
			return radar_kit.GetDeployedClassname();

        return super.ProjectionBasedOnParent();
    }

    override void SetProjectionPosition(vector position)
	{
		//! RAID ALARM
		// Sink radar into ground slightly
		Zen_RaidAlarmRadarKit radar_kit = Zen_RaidAlarmRadarKit.Cast(m_Parent);
		if (radar_kit != NULL)
		{
			m_Projection.SetPosition(position + radar_kit.GetDeployPositionOffset());
			return;
		}

		super.SetProjectionPosition(position);
	}

	override void SetProjectionOrientation(vector orientation)
	{	
		//! RAID ALARM
		Zen_RaidAlarmRadarKit radar_kit = Zen_RaidAlarmRadarKit.Cast(m_Parent);
		if (radar_kit != NULL)
		{
			m_Projection.SetOrientation(orientation + radar_kit.GetDeployOrientationOffset());
			return;	
		}

		super.SetProjectionOrientation(orientation);
	}

	//! RAID ALARM
	override void RefreshVisual()
    {
        super.RefreshVisual();

        if (m_Parent && m_Parent.IsInherited(Zen_RaidAlarmRadarKit) && m_Player.IsPlacingLocal())
        {
            Zen_RaidAlarmRadarKit zenkitbox = Zen_RaidAlarmRadarKit.Cast(m_Parent);
            if (zenkitbox != NULL)
            {
                zenkitbox.SetHologrammed(m_Projection != NULL);
            }
        }
    }

	void ~Hologram()
    {
        if (GetGame())
        {
            if (m_Parent && m_Parent.IsInherited(Zen_RaidAlarmRadarKit))
            {
                Zen_RaidAlarmRadarKit zenkitbox = Zen_RaidAlarmRadarKit.Cast(m_Parent);
                if (zenkitbox != NULL)
                {
                    zenkitbox.SetHologrammed(false);
                }
            }
        }
    }

	override EntityAI PlaceEntity(EntityAI entity_for_placing)
	{
		if (entity_for_placing != NULL)
		{
			//! RAID ALARM
			string itemType = entity_for_placing.GetType();
			itemType.ToLower();

			// Check if this item is on our raid config
			foreach (string s : GetZenDiscordConfig().ItemsDeployedTriggerRaidAlert)
			{
				s.ToLower();
				if (itemType == s || entity_for_placing.IsKindOf(s))
				{
					ZenRaidAlarmPlugin plugin = ZenRaidAlarmPlugin.Cast(GetPlugin(ZenRaidAlarmPlugin));
					if (plugin)
						plugin.AlertNearestRaidStation(entity_for_placing.GetPosition());

					break;
				}
			}
		}

		return super.PlaceEntity(entity_for_placing);
	}
}