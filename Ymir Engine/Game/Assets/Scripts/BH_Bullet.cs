using System;
using YmirEngine;

public class BH_Bullet : YmirComponent
{
    public float speed = 130.0f;
    public float maxLifeTime = 2.0f;
    public float currentLifeTime = 0.0f;
    public float yVel = 0.0f;

    private bool destroyed = false;
    private bool hit = false;

    Vector3 vSpeed = Vector3.zero;
    public void Start()
    {
        vSpeed = GetDirection() * speed;
    }

    public void Update()
    {
        currentLifeTime += Time.deltaTime;

        //Para mover la bala sin rigidbody
        //thisReference.transform.localPosition += thisReference.transform.GetForward() * (speed * Time.deltaTime);

        //Velocidad a la que se dispara la bala hacia delante
        //vSpeed *= speed;

        //Se añade la velocidad al game object
        gameObject.SetVelocity(vSpeed);

        ////Caida de bala (no hace falta si la bala tiene rigidbody)
        //yVel -= Time.deltaTime / 15.0f;
        //thisReference.transform.localPosition += (Vector3.up * yVel);

        //Se destruye
        if (currentLifeTime >= maxLifeTime && destroyed == false)
        {
            InternalCalls.Destroy(gameObject);
            destroyed = true;
        }
    }

    private Vector3 GetDirection()
    {
        GameObject gameObject = InternalCalls.GetGameObjectByName("Player");
        //Debug.Log(gameObject);
        if (gameObject != null)
        {
            //Player player = gameObject.GetComponent<Player>();
            return gameObject.transform.GetForward();
        }
        else return new Vector3(0, 0, 0);
    }

    public void OnCollisionStay(GameObject other)
    {
        if(other.Name == "Facehugger" && !hit)
        {
            FaceHuggerBaseScript script;
            //Do damage AND take into account armor
           script = other.GetComponent<FaceHuggerBaseScript>();
            script.life -= (5 * (1 - script.armor));
            
            //If health 0 then destroy enemy
            if (script.life <= 0)
            {
                InternalCalls.Destroy(other);
            }
            Debug.Log("[ERROR] Life: " + script.life);
            hit = true;
            //Destroy bullet when hit, REMOVE THIS IF THE BULLET SHOULD PIERCE
            currentLifeTime = maxLifeTime;
        }
        else if (other.Name == "Boss" && !hit)
        {
            QueenXenomorphBaseScript script;
            //Do damage AND take into account armor
            script = other.GetComponent<QueenXenomorphBaseScript>();
            script.life -= (5 * (1 - script.armor));

            //If health 0 then destroy enemy
            if (script.life <= 0)
            {
                InternalCalls.Destroy(other);
            }
            Debug.Log("[ERROR] Life: " + script.life);
            hit = true;
            //Destroy bullet when hit, REMOVE THIS IF THE BULLET SHOULD PIERCE
            currentLifeTime = maxLifeTime;
        }

    }
}


