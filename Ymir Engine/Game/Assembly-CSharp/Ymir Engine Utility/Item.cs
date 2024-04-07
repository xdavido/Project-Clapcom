using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace YmirEngine
{
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
}
