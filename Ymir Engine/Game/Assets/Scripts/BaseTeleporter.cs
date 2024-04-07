using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

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

    public void Start()
    {
        Debug.Log("HelloWorld");
    }

    public void Update()
    {
        return;
    }
}