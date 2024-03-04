using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Core : YmirComponent
{
    public GameObject reference = null;
	public GameObject turret = null;
	public GameObject shootPoint = null;
		
	public float rotationSpeed = 2.0f;
	public float movementSpeed = 35.0f;
    public float mouseSens = 5.0f;

	public bool testBool = true;
	public string testString = "Juan";
    public bool start = true;

    private Vector3 zero = Vector3.up;

    public Vector3 testOtherClass; //Should find a way to tell if the class is a gameobject or not

	public void Update(/*int x*/)
	{
        //Hardcoceado para que sea una especie de "Start()"
        if ( start )
        {
            Debug.Log("[WARNING] testString: " + testString);
            reference = InternalCalls.GetGameObjectByName("Test1");
            reference.name = InternalCalls.GetGameObjectByName("Test1").Name;
            Debug.Log("[WARNING] Reference str: " + reference.name);
            Debug.Log("juan " + testString);


           start = false;
         
            Debug.Log("[WARNING] START");

            testString = reference.Tag;
            testString = "Update string";

        }


		if (this.reference == null)
        {
            Debug.Log("[ERROR] Reference on Core.cs was 'null'");
            return;
        }
       
      
 
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
            InternalCalls.CreateGameObject("Cube", zero);

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




