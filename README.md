# MC-STL-generator
![](media/preview.gif)

This example program can make any Minecraft skin 3D printable!

Here's the example skin's head, 3D printed on my makeshift abomination of a 3D printer:

...Poor quality is makeshift 3D printer's fault, not software's.

![](media/printed.jpg)

## Dependencies
You will need [Visual Studio](https://visualstudio.microsoft.com) configured for C++ development, [Python](https://www.python.org/downloads/) and the following [pip](https://pypi.org/project/pip/) libraries:
* opencv-python
* NumPy
* PIL
* sty
* scikit-learn


## Usage

Typically, your multicolor 3D printer should support more colors than your skin has.

If you are certain that's correct, you can skip Step 1.

### Step 1

If your printer supports **LESS** colors than your skin has - you can merge them!
  1. open `Settings.h`
  2. Set `merge_colors` to `true`,
  3. set `max_colors` to the maximum amount of colors your printer supports
  
### Step 2
  
  1. Load this Visual Studio 2022 project
  2. Because this project depends on Python, you will also need to "add" it to your Visual Studio project.
  See these resources ("quick" setup is enough for this use case):
  * [quick setup tutorial](https://medium.datadriveninvestor.com/how-to-quickly-embed-python-in-your-c-application-23c19694813)
  * [detailed setup tutorial](https://devblogs.microsoft.com/python/embedding-python-in-a-cpp-project-with-visual-studio/)
  3. replace `STL generator/skin.png` with your own minecraft skin,
  4. Build and run the project!
  
  You should find your `.3mf` file in the project directory!
  
  If you want separate `.stl` files for each color instead, check out this [instruction](stl_instructions.md)
  
  Happy Printing!
