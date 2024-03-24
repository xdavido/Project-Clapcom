using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using YmirEngine;

public class CameraRot : YmirComponent
{
    //--------------------- Controller var ---------------------\\

    //position difference between camera and player
    //public Vector3 difPos = new Vector3(-15, 20, -15);
    public Vector3 difPos = new Vector3(-66, 139, 72);

    //camera velocity
    private float followStrenght;

    private GameObject target;

    //private bool scriptStart = true;

    public void Start()
    {
        target = InternalCalls.GetGameObjectByName("Player");
        if (target != null)
        {
            followStrenght = target.GetComponent<Player>().movementSpeed;
        }

        //Audio.PlayAudio(gameObject, "LV2_Inter");
    }

    //private void StartFake()
    //{
    //    _gameObject = InternalCalls.GetGameObjectByName("Player");
    //    if (_gameObject != null)
    //    {
    //        followStrenght = _gameObject.GetComponent<Player>().movementSpeed;
    //    }
    //}

    // Update is called once per frame
    public void Update()
    {
        //if (scriptStart == true)
        //{
        //    StartFake();
        //    scriptStart = false;
        //}
        Vector3 newpos = target.transform.globalPosition + difPos;

        float dis = Vector3.Distance(gameObject.transform.globalPosition, newpos);

        gameObject.transform.localPosition = Vector3.Lerp(gameObject.transform.globalPosition, newpos, Time.deltaTime * followStrenght * dis);
    }
}

