using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Core : YmirComponent
{
	public GameObject reference = InternalCalls.GetGameObjectByName("Test0");;
	public GameObject turret = null;
	public GameObject shootPoint = null;
		
	public float rotationSpeed = 2.0f;
	public float movementSpeed = 35.0f;
    public float mouseSens = 5.0f;

	public bool testBool = true;
	public string testString = "Juan";

    public Vector3 testOtherClass; //Should find a way to tell if the class is a gameobject or not


	public void Update(/*int x*/)
	{

 
        testString = "Still an Error";

		if (this.reference == null)
			return;

        testString = "MarcoAulerio";    
 
        if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
            reference.localPosition += reference.GetForward() * movementSpeed * Time.deltaTime;
        if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
            reference.localPosition += reference.GetForward() * -movementSpeed * Time.deltaTime;
        if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
            reference.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, rotationSpeed * Time.deltaTime);
        if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
            reference.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, -rotationSpeed * Time.deltaTime);

        //Destroy current GameObject - It works
        if (Input.GetKey(YmirKeyCode.X) == KeyState.KEY_REPEAT)
            InternalCalls.Destroy(reference);

        //Create a GameObject - Not working
        if (Input.GetKey(YmirKeyCode.C) == KeyState.KEY_REPEAT)
            InternalCalls.CreateGameObject("Cube", reference);

        //if (Input.GetMouseX() != 0 && turret != null)
        //    turret.localRotation = Quaternion.RotateAroundAxis(Vector3.up, -Input.GetMouseX() * mouseSens * Time.deltaTime) * turret.localRotation;

        ////if (Input.GetMouseY() != 0 && turret != null)
        ////    turret.localRotation = turret.localRotation * Quaternion.RotateAroundAxis(Vector3.right, -Input.GetMouseY() * Time.deltaTime);

        //if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_DOWN)
        //{
        //    InternalCalls.CreateBullet(shootPoint.globalPosition, shootPoint.globalRotation, shootPoint.globalScale);
        //}

        return;
	}
}




