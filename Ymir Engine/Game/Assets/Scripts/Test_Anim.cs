using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Test_Anim : YmirComponent
{
	public void Start()
	{
		Animation.SetLoop(gameObject, "Idle", true);
        Animation.SetLoop(gameObject, "Walk", true);
        Animation.SetLoop(gameObject, "Run", true);

        Animation.AddBlendOption(gameObject, "Idle", "Walk", 20.0f);
        Animation.AddBlendOption(gameObject, "Idle", "Run", 20.0f);
        Animation.AddBlendOption(gameObject, "Idle", "Die", 20.0f);


        Animation.AddBlendOption(gameObject, "Walk", "Idle", 20.0f);
        Animation.AddBlendOption(gameObject, "Walk", "Run", 20.0f);
        Animation.AddBlendOption(gameObject, "Walk", "Die", 20.0f);

        Animation.AddBlendOption(gameObject, "Run", "Idle", 20.0f);
        Animation.AddBlendOption(gameObject, "Run", "Walk", 20.0f);
        Animation.AddBlendOption(gameObject, "Run", "Die", 20.0f);


        Animation.PlayAnimation(gameObject, "Idle");

        Animation.SetResetToZero(gameObject, "Die", false);


    }

	public void Update()
	{
        if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_DOWN)
            Animation.PlayAnimation(gameObject, "Idle");

        if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_DOWN)
            Animation.PlayAnimation(gameObject, "Walk");

        if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_DOWN)
            Animation.PlayAnimation(gameObject, "Die");

        if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_DOWN)
            Animation.PlayAnimation(gameObject, "Run");
    }
}