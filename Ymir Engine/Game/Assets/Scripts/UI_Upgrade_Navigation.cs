using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class UI_Upgrade_Navigation : YmirComponent
{
    private bool _canTab;
    //public string stationName = "";
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
            NavigateRight();
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
        if (_focusedGO != null)
        {
            if (Equals(_focusedGO.name, "Upgrade 4"))
            {
                GameObject go = InternalCalls.GetChildrenByName(gameObject, "Upgrade 3");
                UI.SetUIState(go, (int)UI_STATE.FOCUSED);
            }

            else
            {
                switch (gameObject.name)
                {
                    case "Sub Upgrade":
                        {
                            GameObject go = InternalCalls.GetGameObjectByName("Plasma Upgrade");
                            UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                        }
                        break;
                    case "Shotgun Upgrade":
                        {
                            GameObject go = InternalCalls.GetGameObjectByName("Sub Upgrade");
                            UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                        }
                        break;
                    case "Plasma Upgrade":
                        {
                            GameObject go = InternalCalls.GetGameObjectByName("Shotgun Upgrade");
                            UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                        }
                        break;
                }
            }
        }
    }

    private void NavigateRight()
    {
        if (_focusedGO != null)
        {
            if (Equals(_focusedGO.name, "Upgrade 3"))
            {
                GameObject go = InternalCalls.GetChildrenByName(gameObject, "Upgrade 4");
                UI.SetUIState(go, (int)UI_STATE.FOCUSED);
            }

            else
            {
                switch (gameObject.name)
                {
                    case "Sub Upgrade":
                        {
                            GameObject go = InternalCalls.GetGameObjectByName("Shotgun Upgrade");
                            UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                        }
                        break;
                    case "Shotgun Upgrade":
                        {
                            GameObject go = InternalCalls.GetGameObjectByName("Plasma Upgrade");
                            UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                        }
                        break;
                    case "Plasma Upgrade":
                        {
                            GameObject go = InternalCalls.GetGameObjectByName("Sub Upgrade");
                            UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                        }
                        break;
                }
            }
        }
    }
}
