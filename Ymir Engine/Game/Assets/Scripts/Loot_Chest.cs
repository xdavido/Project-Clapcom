using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Loot_Chest : YmirComponent
{
	public GameObject item;
	public void Start()
	{
		List<GameObject> list = new List<GameObject>();
		
	}

	public void Update()
	{
        if(Input.GetKey(YmirKeyCode.SPACE) == KeyState.KEY_DOWN)
		{
            string[] fields = new string[] { "Name:,", "Probability:," };
            Debug.Log(InternalCalls.CSVToString("/Assets/Loot_Tables/loot_table.csv", fields[0], 1));
		}

    }
}