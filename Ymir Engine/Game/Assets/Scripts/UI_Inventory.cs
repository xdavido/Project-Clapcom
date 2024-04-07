using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class UI_Inventory : YmirComponent
{
    private GameObject _selectedGO;
    private GameObject _focusedGO;
    private bool _show;
    public void Start()
    {
        _focusedGO = UI.GetFocused();
        _selectedGO = UI.GetSelected();
        _show = false;
    }

    public void Update()
    {
        _focusedGO = UI.GetFocused();// call this when menu starts or when changed, not efficient rn

        if (_focusedGO != null)
        {
            if (Input.GetGamepadButton(GamePadButton.A) == KeyState.KEY_DOWN)
            {
                SwitchItems();
            }

            if (Input.GetGamepadButton(GamePadButton.RIGHTSHOULDER) == KeyState.KEY_DOWN)
            {
                _show = !_show;
                _focusedGO.GetComponent<UI_Item_Button>().ShowInfo(_show);
            }
        }

        return;
    }

    private void SwitchItems()
    {
        _selectedGO = UI.GetSelected();

        if (_selectedGO != null)
        {
            if (_selectedGO.GetComponent<UI_Item_Button>().item.itemType == _focusedGO.GetComponent<UI_Item_Button>().item.currentSlot ||
                _focusedGO.GetComponent<UI_Item_Button>().item.currentSlot == ITEM_SLOT.NONE)
            {
                UI.SwitchPosition(_selectedGO, _focusedGO);
                ITEM_SLOT aux = _selectedGO.GetComponent<UI_Item_Button>().item.currentSlot;
                _selectedGO.GetComponent<UI_Item_Button>().item.currentSlot = _focusedGO.GetComponent<UI_Item_Button>().item.currentSlot;
                _focusedGO.GetComponent<UI_Item_Button>().item.currentSlot = aux;
            }
        }
    }

}