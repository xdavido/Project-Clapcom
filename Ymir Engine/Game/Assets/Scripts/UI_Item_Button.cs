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

	private GameObject _text;

	public void Start()
	{
		item = new Item(currentSlot, itemType);
        _text = InternalCalls.GetChildrenByName(this, "Text");
    }

    public void Update()
	{

		return;
	}

	public void ShowInfo(bool isShowing)
	{
        _text.SetActive(isShowing);
	}
}