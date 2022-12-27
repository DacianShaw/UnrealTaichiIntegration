## 场景构建Task
#### 1.布料与场景交互
在ASCharacter类的BeiginPlay()函数，将场景中静态球体的半径，球心坐标传入taichi接口

<br/>

#### 2.布料与人物交互
在ASCharacter类的在Tick()函数，实时计算包围盒，传入taichi接口

<br/>

#### 3.布料与风场交互
在ASCharacter类的OnOffWindpower()函数，改变风力，传入taichi接口

<BR>

#### TODO 4.skeletalmesh显示布料
- (1) 布料位置初始化，传入Taichi接口
- (2) 在TICK()调用Taichi接口获取新的位置，进而更新布料位置
- (3)布料采用skeletalmesh, 需要找到引擎中的布料系统在哪里更新skeletalmesh的节点位置
<BR>

## 场景代码使用
#### 1.UE编辑器中新建一个关卡

<br/>

#### 2.创建一个继承于Character的C++类， SCharacter， 将本分支中的代码复制到SCharacter.h 和 SCharacter.cpp中
<br/>

#### 3.在UE编辑器中新建一个蓝图类BP_Pawn， 继承于上一步创建的c++类 SCharacter。打开BP_Pawn的编辑面板，设置人物的mesh，动画等资源。查看Capsule Component的Capsule Half Height属性， 将skeletalmesh的Location设置为[0, 0, - Capsule Half Height] (即调整mesh位置、朝向，使得mesh完全在胶囊体内)。添加弹簧臂，相机，调整角色视角。
![image](SceneBuild/img/BP_Pawn.png)

<br/>

#### 4.将蓝图类BP_Pawn拖入关卡中，在BP_Pawn实列的细节面板中，将Auto Possess Player 设置为Player0
![image](SceneBuild/img/setplayer.png)

<br/>

#### 5.拖入几个球形的StaticMeshActor到关卡中，在BP_Pawn实列的细节面板中，找到Ball Msg List属性，将场景中的球体属性添加到该属性中。
![image](SceneBuild/img/setsceneball.png)

<br/>

#### 6.UE编辑器->编辑->项目设置->Engine->Input， 设置角色的输入
![image](SceneBuild/img/Input.png)
<br/>


#### 7.保存，运行关卡.'F'开关场景中的风场
