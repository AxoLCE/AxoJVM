#include "stdafx.h"
#include "UIScene_ModsMenu.h"

#include "UI.h"
#include "UILayer.h"
#include "../Minecraft.World/HtmlString.h"
#include "Windows64/KeyboardMouseInput.h"

// Shit code :D
UIScene_ModsMenu::UIScene_ModsMenu(
	int iPad,
	void* initData,
	UILayer* parentLayer
)
	: UIScene(iPad, parentLayer)
{
	initialiseMovie();
	m_labelMods.init(L"Mods");
	m_labelName.init(L"");
	m_labelDescription.init(L"");
	m_modsList.init(0);
	m_funcSetDescription =
		registerFastName(L"SetAchievementDescription");
	m_mods = AxoBridge_GetMods();
	m_showDescription = false;
	m_selection = 0;
	for (int i = 0; i < static_cast<int>(m_mods.size()); i++)
	{
		m_modsList.addnewItem(i + 1, L"");
		m_modsList.EnableButton(i, true);
	}
	if (!m_mods.empty())
	{
		m_selection = 1;
		m_modsList.m_iCurrentSelection = 1;
		m_modsList.setCurrentSelection(1);
		UpdateSelectedMod();
	}
	else
	{
		m_labelName.setLabel(L"No mods detected");
		SetModDescription(L"");
	}
}

UIScene_ModsMenu::~UIScene_ModsMenu()
{
}

void UIScene_ModsMenu::handleDestroy()
{
	m_parentLayer->showComponent(
		m_iPad,
		eUIComponent_MenuBackground,
		false
	);
}

wstring UIScene_ModsMenu::getMoviePath()
{
	return L"AchievementsMenu";
}

void UIScene_ModsMenu::SetModDescription(
	const std::wstring& description
)
{
	std::wstring formattedDescription = L"";
	if (!description.empty())
	{
		HtmlString htmlDescription(
			description,
			eHTMLColor_White
		);
		std::vector<HtmlString>* lines =
			new std::vector<HtmlString>();

		lines->push_back(htmlDescription);
		formattedDescription =
			HtmlString::Compose(lines);
	}

	IggyDataValue result;
	IggyDataValue value[1];
	value[0].type = IGGY_DATATYPE_string_UTF16;
	IggyStringUTF16 stringValue;
	stringValue.string =
		(IggyUTF16*)formattedDescription.c_str();
	stringValue.length =
		static_cast<S32>(formattedDescription.length());
	value[0].string16 = stringValue;
	IggyPlayerCallMethodRS(
		getMovie(),
		&result,
		IggyPlayerRootPath(getMovie()),
		m_funcSetDescription,
		1,
		value
	);
}

void UIScene_ModsMenu::UpdateSelectedMod()
{
	if (m_mods.empty())
	{
		m_labelName.setLabel(L"No mods detected");
		SetModDescription(L"");
		return;
	}

	if (m_selection < 1)
	{
		m_selection = 1;
	}

	if (m_selection > static_cast<int>(m_mods.size()))
	{
		m_selection =
			static_cast<int>(m_mods.size());
	}

	const AxoModInfo& mod =
		m_mods[m_selection - 1];

	std::wstring name = mod.name;

	if (name.empty())
	{
		name = mod.id;
	}

	if (!mod.version.empty())
	{
		name += L" ";
		name += mod.version;
	}

	m_labelName.setLabel(name);

	if (!m_showDescription)
	{
		SetModDescription(L"");
		return;
	}

	std::wstring description;

	if (!mod.status.empty())
	{
		description += L"Status: ";
		description += mod.status;
	}

	if (!mod.author.empty())
	{
		if (!description.empty())
		{
			description += L"\n";
		}
		description += L"Author: ";
		description += mod.author;
	}

	if (!mod.description.empty())
	{
		if (!description.empty())
		{
			description += L"\n\n";
		}
		description += mod.description;
	}

	if (!mod.reason.empty())
	{
		if (!description.empty())
		{
			description += L"\n\n";
		}
		description += mod.reason;
	}
	SetModDescription(description);
}

void UIScene_ModsMenu::updateComponents()
{
	bool notInGame =
		(Minecraft::GetInstance()->level == nullptr);

	if (!notInGame)
	{
		m_parentLayer->showComponent(
			m_iPad,
			eUIComponent_MenuBackground,
			true
		);
	}

	m_parentLayer->showComponent(
		m_iPad,
		eUIComponent_Logo,
		false
	);
}

void UIScene_ModsMenu::handleInput(
	int iPad,
	int key,
	bool repeat,
	bool pressed,
	bool released,
	bool& handled
)
{
	ui.AnimateKeyPress(
		m_iPad,
		key,
		repeat,
		pressed,
		released
	);

	switch (key)
	{
	case ACTION_MENU_CANCEL:
		if (pressed)
		{
			ui.NavigateBack(iPad);
			handled = true;
		}
		break;

	case ACTION_MENU_Y:
		if (pressed && !g_KBMInput.IsKBMActive())
		{
			m_showDescription = !m_showDescription;
			UpdateSelectedMod();

			sendInputToMovie(
				key,
				repeat,
				pressed,
				released
			);
			handled = true;
		}
		break;

	case ACTION_MENU_X:
		if (pressed && g_KBMInput.IsKBMActive())
		{
			m_showDescription = !m_showDescription;
			UpdateSelectedMod();

			sendInputToMovie(
				key,
				repeat,
				pressed,
				released
			);
			handled = true;
		}
		break;

	case ACTION_MENU_UP:
		if (pressed && m_selection > 10)
		{
			m_selection -= 10;
			m_modsList.m_iCurrentSelection =
				m_selection;

			sendInputToMovie(
				key,
				repeat,
				pressed,
				released
			);
			handled = true;
		}
		break;

	case ACTION_MENU_DOWN:
		if (pressed &&
			m_selection + 10 <=
			static_cast<int>(m_mods.size()))
		{
			m_selection += 10;
			m_modsList.m_iCurrentSelection =
				m_selection;

			sendInputToMovie(
				key,
				repeat,
				pressed,
				released
			);
			handled = true;
		}
		break;

	case ACTION_MENU_LEFT:
		if (pressed && m_selection > 1)
		{
			m_selection--;
			m_modsList.m_iCurrentSelection =
				m_selection;

			sendInputToMovie(
				key,
				repeat,
				pressed,
				released
			);
			handled = true;
		}
		break;

	case ACTION_MENU_RIGHT:
		if (pressed &&
			m_selection <
			static_cast<int>(m_mods.size()))
		{
			m_selection++;
			m_modsList.m_iCurrentSelection =
				m_selection;

			sendInputToMovie(
				key,
				repeat,
				pressed,
				released
			);
			handled = true;
		}
		break;
	}
}

void UIScene_ModsMenu::tick()
{
	UIScene::tick();

	if (m_mods.empty())
	{
		return;
	}
	if (m_modsList.m_iCurrentSelection <= 0)
	{
		return;
	}
	if (m_modsList.m_iCurrentSelection >
		static_cast<int>(m_mods.size()))
	{
		return;
	}
	if (m_modsList.m_iCurrentSelection != m_selection)
	{
		m_selection =
			m_modsList.m_iCurrentSelection;

		m_modsList.setCurrentSelection(
			m_selection
		);
		UpdateSelectedMod();
	}
}

void UIScene_ModsMenu::handleFocusChange(
	F64 controlId,
	F64 childId
)
{
	switch (static_cast<int>(controlId))
	{
	case 0:
		m_modsList.updateChildFocus(
			static_cast<int>(childId)
		);
		if (m_modsList.m_iCurrentSelection > 0 &&
			m_modsList.m_iCurrentSelection <=
			static_cast<int>(m_mods.size()))
		{
			m_selection =
				m_modsList.m_iCurrentSelection;

			UpdateSelectedMod();
		}
		break;
	}
	updateTooltips();
}

void UIScene_ModsMenu::updateTooltips()
{
	ui.SetTooltips(
		m_iPad,
		-1,
		IDS_TOOLTIPS_CANCEL,
		-1,
		IDS_TOOLTIPS_SHOW_DESCRIPTION
	);
}