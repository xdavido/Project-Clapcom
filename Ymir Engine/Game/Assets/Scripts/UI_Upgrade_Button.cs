using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class UI_Upgrade_Button : YmirComponent
{
    public Upgrade upgrade;

    public string name = "", description = "";
    public int cost;
    public bool isUnlocked;

    public UI_Upgrade_Station upgradeStation;

    public void Start()
    {
        upgrade = new Upgrade(name, description, cost, isUnlocked);

        if (!upgrade.isUnlocked && cost != 1)
        {
            UI.SetUIState(gameObject, (int)UI_STATE.DISABLED);
        }

        GameObject go = InternalCalls.GetGameObjectByName("UpgradeStation");
        if (go != null)
        {
            upgradeStation = go.GetComponent<UI_Upgrade_Station>();
        }

    }

    public void Update()
    {
        return;
    }
    public void OnClickButton()
    {
        if (!upgrade.isUnlocked && upgradeStation.currentScore >= upgrade.cost)
        {
            GameObject go = InternalCalls.GetGameObjectByName("UpgradeStation");

            switch (cost)
            {
                case 1:
                    {
                        GameObject go2 = InternalCalls.GetChildrenByName(go, "Upgrade 2");
                        UI.SetUIState(go2, (int)UI_STATE.NORMAL);
                        upgradeStation.currentScore -= upgrade.cost;
                        upgrade.isUnlocked = true;
                    }
                    break;
                case 2:
                    {
                        GameObject go3 = InternalCalls.GetChildrenByName(go, "Upgrade 3");

                        GameObject go4 = InternalCalls.GetChildrenByName(go, "Upgrade 4");

                        UI.SetUIState(go3, (int)UI_STATE.NORMAL);
                        UI.SetUIState(go4, (int)UI_STATE.NORMAL);

                        upgradeStation.currentScore -= upgrade.cost;
                        upgrade.isUnlocked = true;
                    }
                    break;
                case 4:
                    {
                        upgradeStation.currentScore -= upgrade.cost;
                        upgrade.isUnlocked = true;
                    }
                    break;
            }
        }
    }

    public void OnHoverButton()
    {
        UI.TextEdit(upgradeStation.description, upgrade.description);
        UI.TextEdit(upgradeStation.cost, upgrade.cost.ToString());
    }
}