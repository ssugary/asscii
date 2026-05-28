# asscii

A project developed for fun using a simple computer graphics and image processing application.

<div align="center">
  <table>
    <tr>
      <td align="center">
        <video src="OIIA-Cat.mp4" autoplay loop muted width="350px"></video>
      </td>
      <td align="center">
        <video src="assciiOIIA.mp4" autoplay loop muted width="350px"></video>
      </td>
    </tr>
  </table>
</div>

## Features

* **real-time video processing**: Dynamic RGB video conversion to ASCII characters
* **webcam support**: Support for real-time webcam video
* **custom palettes support**: Support for customizing ASCII palettes
* **frame export**: Auto-saving of frames from the converted video

## Requirements

* **CMake** (min version: 3.16) 
* **C++17 compiler**
* **OpenCV4** lib

## Build

To build execute the following:
```
cmake -S . -B build
cmake --build build
```

Or, make it more convenient:
```bash
alias build_rt="cmake -S . -B build; cmake --build build"
```

## Run

The executable `asscii` will be generate inside the `/build` directory, so is highly recommended to:
```bash
alias asscii="./build/asscii"
```

Use one of the commands available below:

```
❯ asscii -h

Usage: asscii [params] 

        --co, --correction (value:0.5)
                Aspect ratio
        -h, --help (value:true)
                Print the help message
        --he, --height (value:12)
                Cell's height
        -p, --palette
                Custom ASCII character palette
        -s, --show (value:false)
                Shows the video frames in a new window (always true when wc = true)
        --saveframes, --sf (value:false)
                Save the video frames in a directory
        -v, --video
                The input file path
        -w, --width (value:8)
                Cell's width
        --wc, --webcam (value:false)
                Use the webcam instead of a video file

```

## Licence

Copyright (c) 2026 ssugary

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE