# Stop Motion #
## Photographic Video Installation ##

_Ole Kristensen and Jonas Jongejan, Spring 2008_

An interactive photo installation created for a 4,5 x 8 meter LED monolith that works fine on a projection. Combining infrared tracking of a normal light bulb and still images, to index photos of audience in a database according to bulb position within the picture frame. When participating audience moves the bulb within the field of view, the screen flickers through previous images of other people having had the bulb at the same position. When someone moves the bulb to an unihabited spot with no previous images, it will flash and that person will now be shown when the bulb is put in the same position.

Jonas and i sketched up the installation in a few days for re:new 2008, on a very short notice. First iteration was done in [processing](http://processing.org), but with memory issues. Despite that it ended up having a playful and informal presence in the Copenhagen Main Square.

We have since then been invited to [New Media Meeting 03](http://newmediameeting.se), where we decided to port the software into open frameworks, and spent two days bashing a Nikon D80 into submission. The nikon has an insanely buggy ptp implementation, but it was the best we could get.

The source code made available for non-commercial around-fooling and improvement only here at Google Code.
It’s messy, i’ve never done c++ before, but it’s there. Run at your own risk and feel free to improve.

Stop Motion compiles on a Mac using Xcode. You also need [open frameworks](http://openframeworks.cc) and [Macports](http://macports.org) with libgphoto2.

**Please consult the artists before exhibiting this work publicly.**

**_Commercial use of this software is not allowed_**