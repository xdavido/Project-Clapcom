using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class SelectLvl : YmirComponent
{
    public int lvl = 0;  // TODO: Sara --> change to enum
    //private BaseTeleporter levelSelector;

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
        //if (levelSelector.selectedLvl != (LEVELS)lvl)
        //{
        //    levelSelector.selectedLvl = (LEVELS)lvl;
        //}
        //else
        //{
        //    levelSelector.selectedLvl = LEVELS.NONE;
        //}
    }
}