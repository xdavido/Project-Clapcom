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
        GameObject gameObject = InternalCalls.GetGameObjectByName("Background");
        if (gameObject != null)
        {
            levelSelector = gameObject.GetComponent<BaseTeleporter>();
        }

        locked = InternalCalls.GetChildrenByName(this.gameObject, "Lock");
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
            GameObject gameObject = InternalCalls.GetGameObjectByName("Lvl (" + ((int)levelSelector.selectedLvl + 1) + ")");
            GameObject gameObject1 = InternalCalls.GetGameObjectByName("Weapon (" + ((int)levelSelector.selectedWeapon + 1) + ")");

            //Debug.Log("Lvl (" + ((int)levelSelector.selectedLvl + 1) + ")");

            LEVELS selectedLvlPrev = levelSelector.selectedLvl;
            WEAPON_TYPE selectedWeaponPrev = levelSelector.selectedWeapon;

            if (levelSelector.selectedLvl != (LEVELS)lvl)
            {
                levelSelector.selectedLvl = (LEVELS)lvl;
            }
            else
            {
                levelSelector.selectedLvl = LEVELS.NONE;
            }

            if (gameObject != null)
            {
                switch (selectedLvlPrev)
                {
                    case LEVELS.NONE:
                        {
                            break;
                        }
                    case LEVELS.WAREHOUSE:
                        {
                            UI.ChangeImageUI(gameObject, "Assets/UI/Teleport Buttons/BotonUnselected.png", (int)UI_STATE.NORMAL);
                            break;
                        }
                    case LEVELS.LAB:
                        {
                            UI.ChangeImageUI(gameObject, "Assets/UI/Teleport Buttons/BotonUnselected.png", (int)UI_STATE.NORMAL);
                            break;
                        }
                    case LEVELS.HATCHERY:
                        {
                            UI.ChangeImageUI(gameObject, "Assets/UI/Teleport Buttons/BotonUnselected.png", (int)UI_STATE.NORMAL);
                            break;
                        }
                }
            }

            if (gameObject1 != null/* && levelSelector.selectedWeapon != selectedWeaponPrev*/)
            {
                switch (selectedWeaponPrev)
                {
                    case WEAPON_TYPE.NONE:
                        {
                            break;
                        }
                    case WEAPON_TYPE.SMG:
                        {
                            UI.ChangeImageUI(gameObject1, "Assets/UI/Teleport Buttons/SmgHover.png", (int)UI_STATE.NORMAL);
                            break;
                        }
                    case WEAPON_TYPE.SHOTGUN:
                        {
                            UI.ChangeImageUI(gameObject1, "Assets/UI/Teleport Buttons/ShotgunHover.png", (int)UI_STATE.NORMAL);
                            break;
                        }
                    case WEAPON_TYPE.PLASMA:
                        {
                            UI.ChangeImageUI(gameObject1, "Assets/UI/Teleport Buttons/LaserHover.png", (int)UI_STATE.NORMAL);
                            break;
                        }
                }
            }


            Debug.Log("Lvl: " + levelSelector.selectedLvl.ToString());
        }
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
                    UI.TextEdit(levelSelector.lvlText, "WAREHOUSE - Donde estan \nlas descripciones?");
                }
                break;
            case LEVELS.LAB:
                {
                    UI.TextEdit(levelSelector.lvlText, "LAB - En el gdd no estan");
                }
                break;
            case LEVELS.HATCHERY:
                {
                    UI.TextEdit(levelSelector.lvlText, "HATCHERY - AAA");
                }
                break;

        }
    }
}