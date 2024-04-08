using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Caius : YmirComponent
{

	public GameObject name_gameObject;
	public GameObject line1_gameObject;
	public GameObject line2_gameObject;
	public GameObject line3_gameObject;
	public GameObject ui_gameObject;

	private bool talked = false;
	private bool dialogue_ui = false;

	private Player csPlayer;


    public void Start()
	{
        GameObject gameObject = InternalCalls.GetGameObjectByName("Player");
        if (gameObject != null)
        {
            csPlayer = gameObject.GetComponent<Player>();
        }
    }

	public void Update()
	{
		//TODO: Show the dialogue UI when the bool is true
		if (dialogue_ui)
		{
			UI.TextEdit(name_gameObject, "Lorem ipsum");
			ui_gameObject.SetActive(true);


            //TODO: Set de dialogue_ui = false; y csPlayer.inputsList.Add(Player.INPUT.I_IDLE); cuando acabe el dialogo

        }
    }

    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Player" && talked == false && Input.GetKey(YmirKeyCode.SPACE) == KeyState.KEY_DOWN)
        {
			//TODO: Descomentar Don't let the player move
			//csPlayer.inputsList.Add(Player.INPUT.I_STOP);

			//Show the dialogue UI
			dialogue_ui = true;

			//Don't let repeat the dialogue
			talked = true;
        }
    }
}