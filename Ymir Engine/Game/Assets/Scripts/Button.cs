using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using YmirEngine;

//enum UI_STATE
//{
//    DISABLED,
//    NORMAL,
//    FOCUSED,
//    PRESSED,
//    RELEASE,
//    SELECTED,

//    NONE
//};

public class Button : YmirComponent
{
    public void OnClickButton()
    {
        Debug.Log("CLICK");
    }

    public void OnHoverButton()
    {
        Debug.Log("HOVER");
    }
}

