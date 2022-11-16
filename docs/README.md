# Magic WiFi Handler
### An ethernet frame handle for developers working on [Magic WiFi](https://github.com/bluebandit21/magic-wifi)

This library was built for developers to modify their software work with the Magic WiFi device as published in [github.com/bluebandit21/magic-wifi](https://github.com/bluebandit21/magic-wifi).
It adds priorities to sent ethernet frames using the boost.asio library such that higher priority data can be sent
over lower quality signals.


## Using magicWiFi
This library depends on boost.asio, you can either decide to install the entire boost library (see [https://www.boost.org/](https://www.boost.org/))
or you can install only the boost.asio library before installing this one.

Once boost.asio is installed you can download this library to your device and run the install.sh file to install it to your device.

**Note: As this project is still in development the install.sh file may not be functional at the moment.
if this is the case, please install the contents of the lib folder manually to your c++ base folder.**

After installation, on any project just use
```cpp
#include <magicWiFi>
```
You can then refer to the boost.asio library for all functions. The only difference is when sending ethernet frames,
a priority bit is now required.