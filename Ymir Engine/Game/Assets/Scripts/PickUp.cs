using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class PickUp : YmirComponent
{

	public void Start()
	{

	}

	public void Update()
	{

	}

	public void OnCollisionStay(GameObject other)
	{
		if (other.Tag == "Player" && (Input.IsGamepadButtonAPressedCS() || Input.GetKey(YmirKeyCode.SPACE) == KeyState.KEY_DOWN))
		{
			//TODO: Hacer que el item se destruya/elimine
			gameObject.SetActive(false);

			//TODO: Hacer que se sumen al inventario o algo para mantener la cuenta
		}
	}
}