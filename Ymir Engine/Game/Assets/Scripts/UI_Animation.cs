using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using YmirEngine;

public class UI_Animation : YmirComponent
{
    public bool loop = false;
    public bool pingpong = false;
    public bool backwards = false;

    public float speed = 1f;
    public int loopCount = 0;
    public int totalFrames = 1;
    public int currentIndex = 0;

    public GameObject image = null;

    private Vector2 rowXcolumn, currentFrame;

    private float _timer = 0;

    public void Start()
    {
        image = InternalCalls.GetGameObjectByName("Image");
        rowXcolumn = new Vector2(UI.GetImageRows(image), UI.GetImageColumns(image));
        currentFrame = new Vector2(UI.GetImageCurrentFrameY(image), UI.GetImageCurrentFrameY(image));
    }

    public void Update()
    {
        //_timer += Time.deltaTime * speed;

        if ((loop || loopCount == 0) && (_timer += Time.deltaTime) >= speed)
        {
            _timer = 0;

            if (!backwards)
            {
                if (currentFrame.x < rowXcolumn.x) { ++currentFrame.x; }
                else if (currentFrame.x >= rowXcolumn.x) { currentFrame.x = 0; }
                else { ++currentFrame.y; }

                if (currentFrame.y >= rowXcolumn.y) { currentFrame.y = 0; }

                ++currentIndex;
                if (currentIndex >= totalFrames) 
                { 
                    loopCount++; currentIndex = 0;
                    currentFrame.x = 0; currentFrame.y = 0;
                }

                UI.SetImageCurrentFrame(image, (int)currentFrame.x, (int)currentFrame.y);
            }
        }

        return;
    }

    bool HasFinished()
    {
        return ((!loop && !pingpong && currentIndex >= totalFrames) || (pingpong && currentIndex >= totalFrames * 2)) && loopCount > 0;
    }

    void Reset()
    {
        currentIndex = 0;
        loopCount = 0;
        _timer = 0;
    }
}