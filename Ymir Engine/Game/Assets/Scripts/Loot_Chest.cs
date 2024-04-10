using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Loot_Chest : YmirComponent
{
    public string keys = "Nombre:,Probabilidad:";
    public string path = "Assets/Loot Tables/loot_table.csv";
    public int numFields = 2;

    public void Start()
	{
		
	}

	public void Update()
	{

    }

    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Player" && (Input.IsGamepadButtonAPressedCS() || Input.GetKey(YmirKeyCode.SPACE) == KeyState.KEY_DOWN))
        {
            string output = InternalCalls.CSVToString(path, keys);
            //Debug.Log("Output :" + output);

            List<List<string>> result = DeconstructString(output, numFields);

            Debug.Log("Result:");
            foreach (var sublist in result)
            {
                Debug.Log("(" + string.Join(", ", sublist) + ")");

                // Check if sublist has at least two values
                if (sublist.Count >= 2)
                {
                    // Extract the first two values
                    string name = sublist[0];
                    int probability;

                    // Try parsing the probability value
                    if (int.TryParse(sublist[1], out probability))
                    {
                        // Call SpawnPrefab with extracted values
                        SpawnPrefab(name, probability);
                    }
                    else
                    {
                        Debug.Log("[ERROR] Invalid probability value in sublist: " + string.Join(", ", sublist));
                    }
                }
                else
                {
                    Debug.Log("[ERROR] Sublist does not contain enough elements: " + string.Join(", ", sublist));
                }
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

    private void SpawnPrefab(string name, int probability)
    {
        Random random = new Random();
        int randNum = random.Next(0, 101);  //Generate a random number between 0 and 100

        if (randNum <= probability)
        {
            InternalCalls.CreateGOFromPrefab("Assets/Prefabs", name);
        }
    }
}


