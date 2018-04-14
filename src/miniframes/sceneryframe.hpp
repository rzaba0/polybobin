#ifndef SCENERYFRAME_HPP
#define SCENERYFRAME_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/listctrl.h>
#include <map>

#include "miniframe.hpp"
#include "../constants.hpp"
#include "../map/pmsenums.hpp"

/**
* \brief Mini frame with options for scenery tool.
*/
class SceneryFrame : public MiniFrame
{
public:
    SceneryFrame(wxWindow *parent);

    void AddScenery(wxString name, int sceneryId, int sceneryTypeId);
    void RemoveScenery(int sceneryId);
    void ClearSceneryList();

    PMSSceneryLevel GetLevel() const;
    bool GetRotate() const;
    bool GetScale() const;

    wxListCtrl* GetSceneryListCtrl() { return m_sceneryListCtrl; } 

private:
    wxListCtrl *m_sceneryListCtrl;
    std::map<int, int> m_sceneryTypes;
    std::map<int, int> m_addedSceneryTypesCount;

    long m_columnIndex;

    wxRadioButton *m_backLevelRadioButton;
    wxRadioButton *m_middleLevelRadioButton;
    wxRadioButton *m_frontLevelRadioButton;

    wxCheckBox *m_rotateCheckBox;
    wxCheckBox *m_scaleCheckBox;
    
};

#endif // SCENERYFRAME_HPP
