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
		if (Input.GetKey(YmirKeyCode.SPACE) == KeyState.KEY_DOWN)
		{
			string keys = "Nombre:,Probabilidad:";

            string output = InternalCalls.CSVToString("Assets/Loot Tables/loot_table.csv", keys);
			//Debug.Log("Output :" + output);

            List<List<string>> result = DeconstructString(output, 2);

            Debug.Log("Result:");
            foreach (var sublist in result)
            {
                Debug.Log("(" + string.Join(", ", sublist) + ")");
            }
        }
    }

    private static List<List<string>> DeconstructString(string input, int numberOfFields)
    {
        List<List<string>> output = new List<List<string>>();
        string[] parts = input.Split(';');

        List<string> currentList = new List<string>();

        for (int i = 0; i < parts.Length; i++)
        {
            currentList.Add(parts[i]);

            // Check if currentList has reached the desired number of fields
            if (currentList.Count == numberOfFields)
            {
                output.Add(currentList);
                currentList = new List<string>(); // Reset currentList for next set of fields
            }
        }

        // If there are any remaining elements in currentList, add them as a last incomplete sublist
        if (currentList.Count > 0)
        {
            output.Add(currentList);
        }

        return output;
    }
}


