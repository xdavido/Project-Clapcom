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
    public bool pingpongWIP = false;
    public bool backwards = false;
    public bool animState = true;

    public float delay = 1f;
    public int loopCount = 0;
    public int totalFrames = 1;
    public int currentIndex = 0;

    public GameObject image = null;

    private Vector2 rowXcolumn, currentFrame;

    private float _timer = 0;

    public void Start()
    {
        image = gameObject;
        rowXcolumn = new Vector2(UI.GetImageRows(image), UI.GetImageColumns(image));
        currentFrame = new Vector2(UI.GetImageCurrentFrameX(image), UI.GetImageCurrentFrameY(image));
    }

    public void Update()
    {
        if (animState)
        {
            if ((loop || loopCount == 0) && (_timer += Time.deltaTime) >= delay)
            {
                _timer = 0;

                if (!backwards)
                {
                    if (currentFrame.y < rowXcolumn.y) { ++currentFrame.y; }
                    else if (currentFrame.x >= rowXcolumn.x)
                    {
                        currentFrame.x = 0;
                        currentFrame.y = 0;
                    }
                    else
                    {
                        ++currentFrame.x;
                        currentFrame.y = 0;
                    }

                    ++currentIndex;
                    if (currentIndex >= totalFrames)
                    {
                        if (!pingpongWIP)
                        {
                            IncreaseLoopCount();
                        }
                        else
                        {
                            Reset();
                            backwards = !backwards;
                            if (!loop) { pingpongWIP = false; }
                        }
                    }
                }
                else
                {
                    if (currentFrame.y > 0) { --currentFrame.y; }
                    else if (currentFrame.x == 0)
                    {
                        currentFrame.x = rowXcolumn.x;
                        currentFrame.y = rowXcolumn.y;
                    }
                    else
                    {
                        --currentFrame.x;
                        currentFrame.y = rowXcolumn.y;
                    }

                    ++currentIndex;
                    if (currentIndex >= totalFrames)
                    {
                        if (!pingpongWIP)
                        {
                            IncreaseLoopCount();
                        }
                        else
                        {
                            Reset();
                            backwards = !backwards;
                            if (!loop) { pingpongWIP = false; }
                        }
                    }
                }

                UI.SetImageCurrentFrame(image, (int)currentFrame.x, (int)currentFrame.y);
            }
        }

        return;
    }

    public void SetCurrentFrame(int x, int y)
    {
        currentFrame.x = x; currentFrame.y = y;
        UI.SetImageCurrentFrame(image, (int)currentFrame.x, (int)currentFrame.y);
    }

    public void SetAnimationState(bool anim)
    {
        animState = anim;
    }

    bool HasFinished()
    {
        return (!loop && !pingpongWIP && currentIndex >= totalFrames) && loopCount > 0;
    }

    void IncreaseLoopCount()
    {
        loopCount++;

        if (loop)
        {
            currentIndex = 0;

            currentFrame.x = 0;
            currentFrame.y = 0;
        }
    }

    public void Reset()
    {
        currentIndex = 0;
        loopCount = 0;
        _timer = 0;
    }
}