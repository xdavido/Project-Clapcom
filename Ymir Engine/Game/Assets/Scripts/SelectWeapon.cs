using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class SelectWeapon : YmirComponent
{
    public int weapon = 0;  // TODO: Sara --> change to enum
    private BaseTeleporter levelSelector;

    public void Start()
    {
        Debug.Log("HelloWorld");
    }

    public void Update()
    {
        return;
    }

    public void OnExecuteButton()
    {
        if (levelSelector.selectedWeapon != (WEAPON_TYPE)weapon)
        {
            levelSelector.selectedWeapon = (WEAPON_TYPE)weapon;
        }
        else
        {
            levelSelector.selectedWeapon = WEAPON_TYPE.NONE;
        }
    }
}