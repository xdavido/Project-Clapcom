using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class SelectLvl : YmirComponent
{
    public int lvl = 0;  // TODO: Sara --> change to enum
    private BaseTeleporter levelSelector;

    public GameObject locked;
    public bool isLocked = false;

    public void Start()
    {
        GameObject gameObject = InternalCalls.GetGameObjectByName("Screen");
        if (gameObject != null)
        {
            levelSelector = gameObject.GetComponent<BaseTeleporter>();
        }

        locked = InternalCalls.GetChildrenByName(this.gameObject, "Image");
        locked.SetActive(isLocked);
    }

    public void Update()
    {
        return;
    }

    public void OnClickButton()
    {
        if (!isLocked)
        {
            if (levelSelector.selectedLvl != (LEVELS)lvl)
            {
                levelSelector.selectedLvl = (LEVELS)lvl;
            }
            else
            {
                levelSelector.selectedLvl = LEVELS.NONE;
            }

            Debug.Log("Lvl: " + levelSelector.selectedLvl.ToString());
        }

        //UI.TextEdit(levelSelector.lvlText, levelSelector.lvlDescriptions[(int)levelSelector.selectedLvl]);
    }

    public void OnHoverButton()
    {
        switch ((LEVELS)lvl)
        {
            case LEVELS.NONE:
                {
                    UI.TextEdit(levelSelector.lvlText, "");
                }
                break;
            case LEVELS.WAREHOUSE:
                {
                    UI.TextEdit(levelSelector.lvlText, "WAREHOUSE");
                }
                break;
            case LEVELS.LAB:
                {
                    UI.TextEdit(levelSelector.lvlText, "LAB");
                }
                break;
            case LEVELS.HATCHERY:
                {
                    UI.TextEdit(levelSelector.lvlText, "HATCHERY");
                }
                break;

        }
    }
}