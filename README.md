# Constrained-Texture-Mapping
Attach different face images onto different 3D models with corresponding features points

**Operating System：** Windows 7

**IDE：** Visual Studio 2012

**Library：** OpenGL、OpenCV(version 2.4.7)

**How To Use：** 

After open the project, the result of 3D model is on the left, and stickers and corresponding UV map of the model is on the left. 
Dragging the feature point(red point) can change UV map.
The bottoms on the right can loading the .obj file and sticker(1024*1024 .png file) or opening camera to take picture.
The appearance can be switched by clicking or uncliking the list.

***Important***

Every .obj file need to be accompanied with a .txt file which defines the feature points. This .txt file name is identical to .obj file name with an additional "+" character. (i.e. example.obj and example+.txt).

The .txt file define which points are feature points and the relation between feature poins and mesh.


----------------------------------------------------------------------------------------------------------------------------
**作業系統：** Windows 7

**IDE：** Visual Studio 2012

**Library：** OpenGL、OpenCV(板本2.4.7)

**操作方法說明：**

開啟程式後，左邊畫面為3D model結果，右邊畫面為貼圖及model的UV map對應畫面，可用滑鼠拖動特徵點(紅色點)來改變UV map。
右邊按鍵可讀取obj檔、讀取貼圖(貼圖需為1024*1024大小的png檔)及開啟相機取像。畫面呈現可使用勾選選單來切換顯示。
另外，obj檔需附上一個定義特徵點的txt檔，檔名為obj的檔名+.txt。檔案內為定義哪些點為特徵點，及特徵點的mesh關係。
