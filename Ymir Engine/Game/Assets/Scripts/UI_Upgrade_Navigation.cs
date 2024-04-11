using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class UI_Upgrade_Navigation : YmirComponent
{
    private bool _canTab;
    public bool active;
    public int stationName;
    private GameObject _focusedGO;

    private float _time;
    private float _timer;

    public void Start()
    {
        _focusedGO = UI.GetFocused();
        _canTab = false;
        active = true;

        _timer = 0.0f;
        _time = 0.3f;
    }

    public void Update()
    {
        _focusedGO = UI.GetFocused();

        if (!_canTab)
        {
            if (_time > _timer)
            {
                _timer += Time.deltaTime;
            }

            else
            {
                _timer = 0.0f;
                EnableScripts(true, stationName);
                _canTab = true;
            }
        }

        if (active)
        {
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
        }

        return;
    }

    public void NavigateLeft()
    {

        if (_focusedGO != null && InternalCalls.CompareGameObjectsByUID(InternalCalls.CS_GetParent(_focusedGO), gameObject))
        {

            if (InternalCalls.CompareGameObjectsByUID(_focusedGO, InternalCalls.GetChildrenByName(gameObject, "Upgrade 4")))
            {
                GameObject go = InternalCalls.GetChildrenByName(gameObject, "Upgrade 3");
                UI.SetUIState(go, (int)UI_STATE.FOCUSED);
                EnableScripts(false, stationName);
            }

            else
            {
                if (stationName == 1)
                {
                    GameObject go = InternalCalls.GetGameObjectByName("Plasma Upgrade");
                    UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                    EnableScripts(false, stationName);
                    //Debug.Log("para Plasma");
                }

                else if (stationName == 2)
                {
                    GameObject go = InternalCalls.GetGameObjectByName("Sub Upgrade");
                    UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                    EnableScripts(false, stationName);
                    //Debug.Log("para Sub");
                }

                else if (stationName == 3)
                {
                    GameObject go = InternalCalls.GetGameObjectByName("Shotgun Upgrade");
                    UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                    EnableScripts(false, stationName);
                    //Debug.Log("para Shotgun");
                }
            }
        }
    }

    public void NavigateRight()
    {

        if (_focusedGO != null && InternalCalls.CompareGameObjectsByUID(InternalCalls.CS_GetParent(_focusedGO), gameObject))
        {
            if (InternalCalls.CompareGameObjectsByUID(_focusedGO, InternalCalls.GetChildrenByName(gameObject, "Upgrade 3")))
            {
                GameObject go = InternalCalls.GetChildrenByName(gameObject, "Upgrade 4");
                UI.SetUIState(go, (int)UI_STATE.FOCUSED);
                EnableScripts(false, stationName);
            }

            else
            {
                if (stationName == 1)
                {
                    GameObject go = InternalCalls.GetGameObjectByName("Shotgun Upgrade");
                    UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                    EnableScripts(false, stationName);
                    //Debug.Log("para Shotgun");
                }

                else if (stationName == 2)
                {
                    GameObject go = InternalCalls.GetGameObjectByName("Plasma Upgrade");
                    UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                    EnableScripts(false, stationName);
                    //Debug.Log("para Plasma");
                }

                else if (stationName == 3)
                {
                    GameObject go = InternalCalls.GetGameObjectByName("Sub Upgrade");
                    UI.SetUIState(InternalCalls.GetChildrenByName(go, "Start"), (int)UI_STATE.FOCUSED);
                    EnableScripts(false, stationName);
                    //Debug.Log("para Sub");
                }
            }
        }
    }

    private void EnableScripts(bool enable, int stationType)
    {
        if (stationType == 1)
        {
            GameObject go1 = InternalCalls.GetGameObjectByName("Shotgun Upgrade");
            GameObject go2 = InternalCalls.GetGameObjectByName("Plasma Upgrade");
            go1.GetComponent<UI_Upgrade_Navigation>().active = enable;
            go2.GetComponent<UI_Upgrade_Navigation>().active = enable;
        }

        else if (stationType == 2)
        {
            GameObject go1 = InternalCalls.GetGameObjectByName("Sub Upgrade");
            GameObject go2 = InternalCalls.GetGameObjectByName("Plasma Upgrade");
            go1.GetComponent<UI_Upgrade_Navigation>().active = enable;
            go2.GetComponent<UI_Upgrade_Navigation>().active = enable;
        }

        else if (stationType == 3)
        {
            GameObject go1 = InternalCalls.GetGameObjectByName("Shotgun Upgrade");
            GameObject go2 = InternalCalls.GetGameObjectByName("Sub Upgrade");
            go1.GetComponent<UI_Upgrade_Navigation>().active = enable;
            go2.GetComponent<UI_Upgrade_Navigation>().active = enable;
        }
    }
}
