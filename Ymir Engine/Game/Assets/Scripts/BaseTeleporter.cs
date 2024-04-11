using System.Collections.Generic;
using System.Linq;
using YmirEngine;

public enum LEVELS
{
    WAREHOUSE,
    LAB,
    HATCHERY,

    NONE
}

public enum WEAPON_TYPE
{
    SMG,
    SHOTGUN,
    PLASMA,

    NONE
}

public class BaseTeleporter : YmirComponent
{
    public LEVELS selectedLvl = LEVELS.NONE;
    public WEAPON_TYPE selectedWeapon = WEAPON_TYPE.NONE;

    public GameObject button, lvlText, weaponText;

    private bool _setNormal = false;

    //public string[] lvlDescriptions = new string[3];
    //public string[] weaponDescriptions = new string[3];

    //public List<string> lvlDescriptions = new List<string>();
    //public List<string> weaponDescriptions = new List<string>();

    public void Start()
    {
        button = InternalCalls.GetGameObjectByName("Go to raid");
        lvlText = InternalCalls.GetGameObjectByName("Lvl description");
        weaponText = InternalCalls.GetGameObjectByName("Weapon description");

        selectedLvl = LEVELS.NONE;
        selectedWeapon = WEAPON_TYPE.NONE;

        //lvlDescriptions.Add("WAREHOUSE");
        //lvlDescriptions.Add("LAB");
        //lvlDescriptions.Add("HATCHERY");

        //weaponDescriptions.Add("SHOTGUN");
        //weaponDescriptions.Add("SMG");
        //weaponDescriptions.Add("PLASMA");

        //Debug.Log("aaa" + lvlDescriptions.ElementAt(0));
        //Debug.Log("bbb" + weaponDescriptions.ElementAt(0));
    }

    public void Update()
    {
        if (!_setNormal && selectedLvl != LEVELS.NONE && selectedWeapon != WEAPON_TYPE.NONE)
        {
            Debug.Log("Lvl: " + selectedLvl.ToString() + " Weapon: " + selectedWeapon.ToString());

            UI.SetUIState(button, (int)UI_STATE.NORMAL);
            _setNormal = true;

            string scene = "";

            switch (selectedLvl)
            {
                case LEVELS.WAREHOUSE:
                    {
                        scene = "WAREHOUSE";
                    }
                    break;

                case LEVELS.LAB:
                    {
                        scene = "LAB";
                    }
                    break;

                case LEVELS.HATCHERY:
                    {
                        scene = "HATCHERY";
                    }
                    break;
            }

            button.GetComponent<Button_Navigation>().sceneName = "Assets/Scenes/" + scene + ".yscene";

            Debug.Log("scene: " + button.GetComponent<Button_Navigation>().sceneName);
        }
        else if ((UI_STATE)UI.GetUIState(button) != UI_STATE.DISABLED &&
            (selectedLvl == LEVELS.NONE || selectedWeapon == WEAPON_TYPE.NONE))
        {
            Debug.Log("Lvl: " + selectedLvl.ToString() + " Weapon: " + selectedWeapon.ToString());

            UI.SetUIState(button, (int)UI_STATE.DISABLED);
            _setNormal = false;
        }

        return;
    }
}