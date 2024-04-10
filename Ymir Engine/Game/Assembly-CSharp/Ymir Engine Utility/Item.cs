using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace YmirEngine
{
    enum UI_STATE
    {
        DISABLED,
        NORMAL,
        FOCUSED,
        PRESSED,
        RELEASE,
        SELECTED,

        NONE
    };

    public enum ITEM_SLOT
    {
        ARMOR,
        CHIP1,
        CHIP2,
        CONSUMABLE1,
        CONSUMABLE2,
        SAVE,
        NONE
    }

    // WIP
    public class Item
    {
        public ITEM_SLOT currentSlot;
        public ITEM_SLOT itemType;
        public float defense, healingEficiency, speed, reloadSpeed, skillDamage;

        public Item(ITEM_SLOT currentSlot, ITEM_SLOT itemType)
        {
            this.currentSlot = currentSlot;
            this.itemType = itemType;
        }
    }

    public class Upgrade
    {
        public string name, description;
        public int cost;
        public bool isUnlocked;

        public Upgrade(string name, string description, int cost, bool isUnlocked)
        {
            this.name = name;
            this.description = description;
            this.cost = cost;
            this.isUnlocked = isUnlocked;
        }
    }
}
