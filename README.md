Made with Juce 8.09 and the VST3 SDK - 

  Basic  Plugin - one is built as an effect the other as instrument.  Common manufacturer ID and Different Plugin ID was used intially due to earlier attempt to use shared instance memory
  but it didn't work so windows shared memory was used instead.    
  
HOW IT WORKS

1. Load the Send Effect VST3 plugin to a mixer track - this was made for FL Studio due to FL Studio blocking some types of loopback routing.
2. Load the Return Vsti plugin in fl studio into a channel rack channel (or possibly patcher havn't tested it)

3. Once loaded whatever audio is reaching the Send Effect point will be relayed to the Return Plugin you can then assign a mixer track to that generator to send it to a different route
so it  essentially works like a patch cable.

4. It was built with a buffer and it still can have runaway feedback.  The delay is there to prevent overloading the signal with very rapid runaway feedback.

5. It was built for windows using windows shared memory - it probably will not work on Mac I havn't tested or built this for Mac OS.

6. Future add ons I was going to do but chose sleep instead - a knob to adjust the buffer - it likely needs some type of limiter or soft clipper it is suggested to load some type of clipper or limiter in chain be preared to control the feedback you can add effects in the signal chain.  A knob for a feedback reduction or shaping algorythm might also be useful.
I am also going to look into building a return effect I though the generator would be best suited. If using something like senderella you can insert the channel it is assigned to.
A delay knob to adjust the buffer to less than 3 seconds or more was not added to safety considerations while testing. 



VST3 license https://steinbergmedia.github.io/vst3_dev_portal/pages/VST+3+Licensing/VST3+License


This is working for me on windows 11, if it isn't working for you check your routing there is a delay of 3 seconds. Audio quality likely needs to be smoothed still.
