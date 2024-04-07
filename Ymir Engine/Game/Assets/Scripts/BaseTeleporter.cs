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
    SHOTGUN,
    SMG,
    PLASMA,

    NONE
}

public class BaseTeleporter : YmirComponent
{
    public LEVELS selectedLvl = LEVELS.NONE;
    public WEAPON_TYPE selectedWeapon = WEAPON_TYPE.NONE;

    public GameObject button;

    private bool _setNormal = false;

    public void Start()
    {
        Debug.Log("HelloWorld");

        button = InternalCalls.GetGameObjectByName("Go to raid");
        UI.SetUIState(button, (int)UI_STATE.DISABLED);

        selectedLvl = LEVELS.NONE;
        selectedWeapon = WEAPON_TYPE.NONE;
    }

    public void Update()
    {
        if (!_setNormal && selectedLvl != LEVELS.NONE && selectedWeapon != WEAPON_TYPE.NONE)
        {
            UI.SetUIState(button, (int)UI_STATE.NORMAL);
            _setNormal = true;

            Debug.Log("Lvl: " + selectedLvl.ToString() + " Weapon: " + selectedWeapon.ToString());
        }
        else if ((UI_STATE)UI.GetUIState(button) != UI_STATE.DISABLED &&
            (selectedLvl == LEVELS.NONE || selectedWeapon == WEAPON_TYPE.NONE))
        {
            UI.SetUIState(button, (int)UI_STATE.DISABLED);
            _setNormal = false;
        }

        return;
    }
}