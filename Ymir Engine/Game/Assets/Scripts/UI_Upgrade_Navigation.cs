using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class UI_Upgrade_Navigation : YmirComponent
{
    private bool _canTab;
    public string stationName = "";
    private GameObject _focusedGO;
    public void Start()
	{
        _focusedGO = UI.GetFocused();
        _canTab = false;
    }

	public void Update()
	{
        _focusedGO = UI.GetFocused();

        if (!_canTab && Input.GetLeftAxisX() == 0)
        {
            _canTab = true;
        }

        if (Input.GetLeftAxisX() > 0 && _canTab)
        {
            _canTab = false;
        }

        else if (Input.GetLeftAxisX() < 0 && _canTab)
        {
            NavigateLeft();
            _canTab = false;
        }
        return;
	}

    private void NavigateLeft()
    {
      
        //    if (_focusedGO != null)
        //    {
        //    if (Equals(_focusedGO.name, "Upgrade 3"))
        //    {

        //    }
        //        switch (stationName)
        //        {
        //            case 1:
        //                {

        //                }
        //                break;
        //            case 2:
        //                {
                            
        //                }
        //                break;
        //            case 4:
        //                {
                           
        //                }
        //                break;
        //        }
        //    }
        //}

}
}