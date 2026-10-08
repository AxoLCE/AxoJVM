#pragma once

#include <vector>

#include "UIScene.h"
#include "UIControl_Label.h"
#include "UIControl_AchievementsList.h"
#include "../../../Minecraft.World/Axo/AxoBridge.h"

class UIScene_ModsMenu : public UIScene
{
private:
	enum EControls
	{
		eControl_ModsLabel,
		eControl_ModsName,
		eControl_ModsDescription,
		eControl_ModsListContainer
	};
	UIControl m_controlMainPanel;
	UIControl_Label m_labelMods;
	UIControl_Label m_labelName;
	UIControl_Label m_labelDescription;
	UIControl_AchievementsList m_modsList;
	std::vector<AxoModInfo> m_mods;
	IggyName m_funcSetDescription;
	bool m_showDescription;
	int m_selection;
	UI_BEGIN_MAP_ELEMENTS_AND_NAMES(UIScene)
		UI_MAP_ELEMENT(m_labelMods, "AcheivementsLabel")
		UI_MAP_ELEMENT(m_labelName, "AchievementName")
		UI_MAP_ELEMENT(m_labelDescription, "AchievementDescription")
		UI_MAP_ELEMENT(m_controlMainPanel, "AchievementsListContainer")

		UI_BEGIN_MAP_CHILD_ELEMENTS(m_controlMainPanel)
			UI_MAP_ELEMENT(m_modsList, "AchievementsList")
		UI_END_MAP_CHILD_ELEMENTS()

		UI_MAP_NAME(
			m_funcSetDescription,
			L"SetAchievementDescription"
		)
	UI_END_MAP_ELEMENTS_AND_NAMES()

	void SetModDescription(const std::wstring& description);
	void UpdateSelectedMod();

public:
	UIScene_ModsMenu(
		int iPad,
		void* initData,
		UILayer* parentLayer
	);
	virtual ~UIScene_ModsMenu();
	virtual void handleDestroy();
	virtual void tick();
	virtual EUIScene getSceneType()
	{
		return eUIScene_ModsMenu;
	}
	virtual void handleInput(
		int iPad,
		int key,
		bool repeat,
		bool pressed,
		bool released,
		bool& handled
	);
	virtual void handleFocusChange(
		F64 controlId,
		F64 childId
	);
	virtual void updateComponents();
	virtual void updateTooltips();
protected:
	virtual wstring getMoviePath();
};