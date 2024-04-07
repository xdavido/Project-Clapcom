using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class UI_Item_Button : YmirComponent
{
    public Item item;
    public ITEM_SLOT currentSlot;
    public ITEM_SLOT itemType;

    public string noHayEnumItem = "";
    public string noHayEnumSlot = "";

    private GameObject _text;

    public void Start()
    {
        item = new Item(currentSlot, itemType);
        _text = InternalCalls.GetChildrenByName(gameObject, "Text");
        SetType(noHayEnumItem, itemType);
        SetType(noHayEnumSlot, currentSlot);
    }

    public void Update()
    {

        return;
    }

    public void ShowInfo(bool isShowing)
    {
        _text.SetActive(isShowing);
    }

    private void SetType(string type, ITEM_SLOT elementChanged)
    {
        switch (type)
        {
            case "ARMOR":
                elementChanged = ITEM_SLOT.ARMOR;
                break;
            case "CHIP1":
                elementChanged = ITEM_SLOT.CHIP1;
                break;
            case "CHIP2":
                elementChanged = ITEM_SLOT.CHIP2;
                break;
            case "CONSUMABLE1":
                elementChanged = ITEM_SLOT.CONSUMABLE1;
                break;
            case "CONSUMABLE2":
                elementChanged = ITEM_SLOT.CONSUMABLE2;
                break;
            case "SAVE":
                elementChanged = ITEM_SLOT.SAVE;
                break;
            case "NONE":
                elementChanged = ITEM_SLOT.NONE;
                break;
            default:
                break;
        }
    }
}