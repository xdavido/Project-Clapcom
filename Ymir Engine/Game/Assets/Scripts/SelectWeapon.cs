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
        GameObject gameObject = InternalCalls.GetGameObjectByName("Screen");
        if (gameObject != null)
        {
            levelSelector = gameObject.GetComponent<BaseTeleporter>();
        }
    }

    public void Update()
    {
        return;
    }

    public void OnClickButton()
    {
        if (levelSelector.selectedWeapon != (WEAPON_TYPE)weapon)
        {
            levelSelector.selectedWeapon = (WEAPON_TYPE)weapon;
        }
        else
        {
            levelSelector.selectedWeapon = WEAPON_TYPE.NONE;
        }

        Debug.Log("Weapon: " + levelSelector.selectedWeapon.ToString());

        //UI.TextEdit(levelSelector.weaponText, levelSelector.weaponDescriptions[(int)levelSelector.selectedWeapon]);
    }

    public void OnHoverButton()
    {
        switch ((WEAPON_TYPE)weapon)
        {
            case WEAPON_TYPE.NONE:
                {
                    UI.TextEdit(levelSelector.weaponText, "");
                }
                break;
            case WEAPON_TYPE.SHOTGUN:
                {
                    UI.TextEdit(levelSelector.weaponText, "SHOTGUN");
                }
                break;
            case WEAPON_TYPE.SMG:
                {
                    UI.TextEdit(levelSelector.weaponText, "SHOTGUN");
                }
                break;
            case WEAPON_TYPE.PLASMA:
                {
                    UI.TextEdit(levelSelector.weaponText, "PLASMA");
                }
                break;

        }
    }
}