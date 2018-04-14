#include "sceneryframe.hpp"
#include "../constants.hpp"


SceneryFrame::SceneryFrame(wxWindow *parent)
    : MiniFrame(parent, ID_FRAME_SCENERY, wxT("Scenery"), wxDefaultPosition, wxDefaultSize)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);


    m_sceneryListCtrl = new wxListCtrl(this, wxID_ANY,
            wxDefaultPosition, wxSize(300, 200), wxLC_REPORT | wxLC_NO_HEADER  | wxLC_SINGLE_SEL);
    sizer->Add(m_sceneryListCtrl);
    m_columnIndex = m_sceneryListCtrl->AppendColumn("Scenery");
    m_sceneryListCtrl->SetMaxSize(wxSize(140, 80));
    m_sceneryListCtrl->SetMinSize(wxSize(140, 80));
    m_sceneryListCtrl->SetColumnWidth(m_columnIndex, wxLIST_AUTOSIZE);
    sizer->Layout();


    wxBoxSizer *modifiersSizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(modifiersSizer, 0, wxEXPAND);

    wxBoxSizer *levelSizer = new wxBoxSizer(wxVERTICAL);
    modifiersSizer->Add(levelSizer);

    wxStaticText *levelLabel = new wxStaticText(this, wxID_ANY, "Level:");
    levelSizer->Add(levelLabel);

    m_backLevelRadioButton = new wxRadioButton(this, wxID_ANY, "Back");
    m_backLevelRadioButton->SetValue(true);
    levelSizer->Add(m_backLevelRadioButton);
    m_middleLevelRadioButton = new wxRadioButton(this, wxID_ANY, "Middle");
    levelSizer->Add(m_middleLevelRadioButton);
    m_frontLevelRadioButton = new wxRadioButton(this, wxID_ANY, "Front");
    levelSizer->Add(m_frontLevelRadioButton);

    modifiersSizer->AddStretchSpacer();

    wxBoxSizer *transformSizer = new wxBoxSizer(wxVERTICAL);
    modifiersSizer->Add(transformSizer);

    m_rotateCheckBox = new wxCheckBox(this, wxID_ANY, "Rotate");
    transformSizer->Add(m_rotateCheckBox);
    m_scaleCheckBox = new wxCheckBox(this, wxID_ANY, "Scale");
    transformSizer->Add(m_scaleCheckBox);


    Fit();
}

void SceneryFrame::AddScenery(wxString name, int sceneryId, int sceneryTypeId)
{
    m_sceneryTypes[sceneryId] = sceneryTypeId;

    if (m_addedSceneryTypesCount.count(sceneryTypeId) == 0)
    {
        long index = m_sceneryListCtrl->InsertItem(0, name);
        m_sceneryListCtrl->SetItemData(index, sceneryTypeId);

        m_addedSceneryTypesCount[sceneryTypeId] = 1;
    }
    else
    {
        m_addedSceneryTypesCount[sceneryTypeId]++;
    }

    m_sceneryListCtrl->SetColumnWidth(m_columnIndex, wxLIST_AUTOSIZE);
}

void SceneryFrame::RemoveScenery(int sceneryId)
{
    int sceneryTypeId = m_sceneryTypes[sceneryId];

    m_addedSceneryTypesCount[sceneryTypeId]--;
    if (m_addedSceneryTypesCount[sceneryTypeId] == 0)
    {
        m_addedSceneryTypesCount.erase(sceneryTypeId);

        long itemId = -1;
        itemId = m_sceneryListCtrl->GetNextItem(itemId, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
        while (itemId != -1)
        {
            long sceneryTypeId = m_sceneryListCtrl->GetItemData(itemId);
            if (sceneryTypeId == sceneryTypeId)
            {
                m_sceneryListCtrl->DeleteItem(itemId);
                itemId = -1;
            }
            else
            {
                itemId = m_sceneryListCtrl->GetNextItem(itemId, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
            }
        }
    }
}

void SceneryFrame::ClearSceneryList()
{
    m_sceneryTypes.clear();
    m_addedSceneryTypesCount.clear();
    m_sceneryListCtrl->DeleteAllItems();
}

PMSSceneryLevel SceneryFrame::GetLevel() const
{
    if (m_backLevelRadioButton->GetValue())
        return lBEHIND_ALL;
    else if (m_middleLevelRadioButton->GetValue())
        return lBEHIND_MAP;
    else
        return lBEHIND_NONE;
}

bool SceneryFrame::GetRotate() const
{
    return m_rotateCheckBox->GetValue();
}

bool SceneryFrame::GetScale() const
{
    return m_scaleCheckBox->GetValue();
}
