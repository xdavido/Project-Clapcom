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

    public void Start()
    {

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
            UI.NavigateGrid(rows, cols, true);
        }

        else if (Input.GetLeftAxisX() < 0 && _canTab)
        {
            _canTab = false;
            UI.NavigateGrid(rows, cols, false);
        }

        return;
    }
}
