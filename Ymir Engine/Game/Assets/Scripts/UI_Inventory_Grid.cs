using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;


public class UI_Inventory_Grid : YmirComponent
{
    public int rows;
    public int cols;
    private bool _canTab;

    private GameObject leftGrid;
    private GameObject rightGrid;
    public string leftGridName = " ";
    public string rightGridName = " ";
    public bool naviagteGrids = false;

    public void Start()
    {
        leftGrid = InternalCalls.GetGameObjectByName(leftGridName);
        rightGrid = InternalCalls.GetGameObjectByName(rightGridName);
    }         

    public void Update()
    {
        if (!_canTab && Input.GetLeftAxisX() == 0)
        {
            _canTab = true;
        }

        if (Input.GetLeftAxisX() > 0 && _canTab)
        {
            _canTab = false;
            UI.NavigateGrid(gameObject,rows, cols, true, naviagteGrids, leftGrid, rightGrid);
        }

        else if (Input.GetLeftAxisX() < 0 && _canTab)
        {
            _canTab = false;
            UI.NavigateGrid(gameObject, rows, cols, false, naviagteGrids, leftGrid, rightGrid);
        }

        return;
    }
}
