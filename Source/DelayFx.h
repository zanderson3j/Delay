/*
  ==============================================================================

    DelayFx.h
    Created: 15 May 2021 1:23:38pm
    Author:  Zachary Anderson

  ==============================================================================
*/

#pragma once

#include "CircularBuffer.h"


class DelayFx
{
public:
    DelayFx ()
    {
        delayBufferL.createCircularBuffer(88200);
        delayBufferL.flushBuffer();
        
        delayBufferR.createCircularBuffer(88200);
        delayBufferR.flushBuffer();
        
        delaySamples = 0;
    }
    
    void effect (AudioBuffer<float> &outputBuffer, int sample)
    {
        if (delaySamples == 0) return;
        
        // Ping pong tap delays for left and right outputs.
        
        float ynL = delayBufferL.readBuffer(delaySamples);
        float y1L = delayBufferL.readBuffer(8000)*0.5;
        float y2L = delayBufferL.readBuffer(2000)*0.5;
        
        float ynR = delayBufferR.readBuffer(delaySamples);
        float y1R = delayBufferR.readBuffer(4000)*0.5;
        float y2R = delayBufferR.readBuffer(1000)*0.5;
        
        float dnL = outputBuffer.getSample(0, sample)*0.3 + (0.7*ynL);
        float dnR = outputBuffer.getSample(1, sample)*0.3  + (0.7*ynR);
        
        delayBufferL.writeBuffer(dnR);
        delayBufferR.writeBuffer(dnL);
        
        auto* output0 = outputBuffer.getWritePointer(0, sample);
        auto* output1 = outputBuffer.getWritePointer(1, sample);
        
        *output0 = outputBuffer.getSample(0, sample) + (ynL + y1L + y2L);
        *output1 = outputBuffer.getSample(1, sample) + (ynR + y1R + y2R);
    }
    
    void setDelaySamples (int delaySamples)
    {
        if (delaySamples != this->delaySamples)
        {
            // Flush buffer on parameter change to remove crackling noises.
            delayBufferL.flushBuffer();
            delayBufferR.flushBuffer();
        }
        this->delaySamples = delaySamples;
    }
    
private:
    CircularBuffer<float> delayBufferL;
    CircularBuffer<float> delayBufferR;
    int delaySamples;
};
