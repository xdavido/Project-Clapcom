using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class UI_Upgrade_Station : YmirComponent
{
    private GameObject _focusedGO;
    public int currentScore;// Put in player

    public GameObject description, cost, coins;

    public void Start()
    {
        _focusedGO = UI.GetFocused();
        description = InternalCalls.GetChildrenByName(gameObject, "Description");
        cost = InternalCalls.GetChildrenByName(gameObject, "Cost");        
        coins = InternalCalls.GetChildrenByName(gameObject, "Coins");
        UI.TextEdit(coins, currentScore.ToString());
    }

    public void Update()
    {
        //_focusedGO = UI.GetFocused();// call this when menu starts or when changed, not efficient rn

        //if (_focusedGO != null)
        //{
        //    if (Input.GetGamepadButton(GamePadButton.A) == KeyState.KEY_DOWN)
        //    {
        //        if (_focusedGO.GetComponent<UI_Upgrade_Button>().upgrade.cost <= currentScore &&
        //            !_focusedGO.GetComponent<UI_Upgrade_Button>().upgrade.isUnlocked)
        //        {
        //            currentScore -= _focusedGO.GetComponent<UI_Upgrade_Button>().upgrade.cost;
        //            _focusedGO.GetComponent<UI_Upgrade_Button>().upgrade.isUnlocked = true;

        //            GameObject go = UI.UpgradeToUnlock(_focusedGO);
        //            if (go != null)
        //            {
        //                UI.SetUIState(go, (int)UI_STATE.NORMAL);
        //            }
        //        }
        //    }
        //}

        if (Input.GetGamepadButton(GamePadButton.B) == KeyState.KEY_DOWN)
        {
            gameObject.SetActive(false);
        }

        return;
    }

    public void UpdateCoins()
    {
        UI.TextEdit(coins, currentScore.ToString());
    }
}