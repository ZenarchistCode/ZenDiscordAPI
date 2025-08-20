// Copy & pasted from ZenModPack but extracted into its own class to work standalone.
class ZenKitBoxBase_RaidAlarm extends DeployableContainer_Base
{
	#ifndef ZENMODPACK
	protected bool m_WasZenHologrammed;

	void SetZenHologrammed(bool hologram, string textureOverride = "")
	{
		m_WasZenHologrammed = hologram;

		array<string> config_textures = GetHiddenSelectionsTextures();
		if (!config_textures || config_textures.Count() == 0)
			return;

		if (m_WasZenHologrammed)
		{
			string textureAlpha = "#(argb,8,8,3)color(1,1,1,0.1,ca)";
			if (textureOverride != "")
				textureAlpha = textureOverride;

			SetObjectTexture(0, textureAlpha);
		}
		else
		{
			string textureNoAlpha = config_textures.Get(0);
			SetObjectTexture(0, textureNoAlpha);
		}
	}

	bool IsZenHologrammed()
	{
		return m_WasZenHologrammed;
	}

	bool ShouldZenHologram()
	{
		return true;
	}
	#endif

	string GetDeployedClassname()
	{
		return "";
	}

	vector GetDeployPositionOffset()
	{
		return "0 0 0";
	}

	vector GetDeployOrientationOffset()
	{
		return "0 0 0";
	}

	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete(player, position, orientation);

		if (!GetGame().IsDedicatedServer() || position == vector.Zero)
			return;

		PlayerBase pb = PlayerBase.Cast(player);
		if (!pb)
			return;

		ItemBase deployedItem = ItemBase.Cast(GetGame().CreateObject(GetDeployedClassname(), pb.GetLocalProjectionPosition(), false));
		if (!deployedItem)
		{
			Error("ZenKitBoxBase - failed to deploy classname: " + GetDeployedClassname());
			return;
		}

		SetIsDeploySound(true);
		deployedItem.SetPosition(position);
		deployedItem.SetOrientation(orientation);
		DeleteSafe();
	}

	override bool IsBasebuildingKit()
	{
		return true;
	}

	override bool IsDeployable()
	{
		return true;
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}
}