# HMMR 部分

## HMMR结果输出

对于HMMR([我的fork](https://github.com/Zju-George/human_dynamics))，![image.png](http://pfp.ps.netease.com/kmpvt/file/5d664bb22dcade90fdd133e5UjVyQStM01?sign=UUwHkiWQ_Mv0fXijHxbinI6Wb0c=&expire=1567097465)，主要修改在 `$hmmr/demo_video.py` 中（`$hmmr`代表自己环境下的hmmr项目的根目录），将旋转以 3乘3矩阵 的形式存进json，预测的结果保存在`$hmmr/demo_output/$视频名称/rot_output/rot_output.json`。

除此之外，还写了`$hmmr/src/cam_rot.py` 脚本，用来预测 cam 的旋转。
## Unity将结果导入

`Assets/Scripts/HMMR/JsonReader.cs` 脚本负责将  `rot_output.json`读入,挂在 m_avg_root ![image.png](http://pfp.ps.netease.com/kmpvt/file/5d67862a68d864e28ab75e8eW0Bw8mmw01?sign=sXls-uye3BfRum18l0pxLXktqAc=&expire=1567097465)下的脚本![image.png](http://pfp.ps.netease.com/kmpvt/file/5d67859b68d864aca6a16213qb7M05iw01?sign=aneC01hEtYdANzIhJcJ9IgYoW4s=&expire=1567097465)负责管理所有关节。
挂在每个子关节下有一个RotateTest脚本，这个由JointManager控制，负责真正驱动每个关节。

## 恢复根关节位移
2D 关键点保存在 `$hmmr/demo_output/$视频名称/AlphaPose_output/alphapose-results-forvis-tracked.json` 中，需要借助 2D 关键点恢复根关节。

名为 Train 的游戏对象挂的 `KeyPointManager`  就是负责梯度下降恢复根关节的脚本。(这里其实解耦没做好，管理2D点渲染应该和训练应该分开，这里图省事写一起了。。）
![image.png](http://pfp.ps.netease.com/kmpvt/file/5d6792cf68d864ef809c7d7bGXBx8dFz01?sign=Prhh44TWkbJ0kct3qKSRZMSAexU=&expire=1567097465)

##### 具体使用方法：
```
     private void Start()
    {
        string path = @"C:\Users\sunxiaohan01\Desktop\sxh_git\human_dynamics\demo_output\paoku\video_frames\camrot.json";
        /// 以下省略
    }
```
`KeyPointManager.cs` 的 46 行 path 即为保存摄像机旋转的json路径。
```
private bool isCamRotate = false;
```
默认是摄像机无旋转的。如果有的话将它改为true。
至于摄像机旋转，可以通过`$hmmr/src/cam_rot.py`获得，会生成`camrot.json`。

在 `KeyPointManager.cs` 的第 84 到 86 行，![image.png](http://pfp.ps.netease.com/kmpvt/file/5d679fc02dcadef4e70a1de6w5zYI3Pb01?sign=RMX17NxiHF9aDuwC9v5EF0K6I5A=&expire=1567097465)
`StartCoroutine(RepeatGD());` 表示优化第 0 帧的摄像机的位置和旋转。
`StartCoroutine(Train());` 表示优化位移，优化完会自动保存在 `$hmmr/demo_output/$视频名称/rot_output/rootpos.json` 

恢复了根节点位移后，在 `JointManager` 下勾选
![image.png](http://pfp.ps.netease.com/kmpvt/file/5d67a6da2dcadef4e70a1e03Xqut8ZUa01?sign=SGQhTYK887E_m73vaj-kiHhOG6w=&expire=1567097465)
即可加载位移。

## Retargeting

这边的功能基本就是由 `HMMR2DM.cs` 实现。

要可视化 HMMR 骨架和 DeepMimic 骨架，
勾选 **Issync** 
要将 Retargeting 结果保存下来，勾选 **Iswrite**，这里默认的路径是在 `$DeepMimic/data/motions/` 下。
![image.png](http://pfp.ps.netease.com/kmpvt/file/5d67ae1a68d864ef809c7ddexdSYxyeE01?sign=YfNIeeIm7NwgtCznxn4WBtwByCs=&expire=1567097465)

##### 动作平滑

**smoothfactor** 代表了指数平滑的系数，但是这里 **1.0** 是不平滑， **0** 是平滑到最大（所有帧都是第0帧的动作了）

建议取 **0.1** 到 **0.6** 之间。在写文件的时候会自动平滑。

* * *

# DeepMimic 部分
对于DeepMimic ([我的 fork](https://github.com/Zju-George/DeepMimic))，修改有**训练过程图表可视化**、**录制动作**、**恢复多地形**和**坐标轴可视化**。以及在Unity 中可视化最后的结果。

### 训练过程图表可视化
训练结果输出写在` rl_agent.py` 中![image.png](http://pfp.ps.netease.com/kmpvt/file/5d67b80a68d864ef809c7de9OqBdaAtP01?sign=dSxHVaUi-_XLO9l6becmj1fDf8E=&expire=1567097465)，要可视化它，运行 `showplot.py `![image.png](http://pfp.ps.netease.com/kmpvt/file/5d67b8ae68d864ef809c7debSy9ROpmX01?sign=GyWhE6qksuuywOUuvQ8ZB4a2shY=&expire=1567097465)

### 录制动作
代码实现在C++部分 ![image.png](http://pfp.ps.netease.com/kmpvt/file/5d67b9732dcadef4e70a1e17Y8fYcBbC01?sign=njy7ounT75dZMThfw6mRDBd92xU=&expire=1567097465)
具体使用：按大写R键开始录制，再按一次R，就会把这之间的骨骼动画全部 record下来，保存在`output/motion.json`（output文件夹要自己创建，gitignore了）这个可以在 unity 中播放。 

### 恢复多地形
原始的DeepMimic只有 plane ,现在支持
![image.png](http://pfp.ps.netease.com/kmpvt/file/5d67bab02dcadef4e70a1e19v06WcKFx01?sign=L_euNieX8WlENVc8_ubJjmGAXqg=&expire=1567097465)，在 `args/` 下改变参数即可。（这边涉及的代码改动太多，这里略去，可以参考github原版和我的fork之间src/sim/下的.cpp的多处区别）

### Unity衔接

由 `DMJsonReader` 实现。![image.png](http://pfp.ps.netease.com/kmpvt/file/5d67bbc32dcadebdc9136ab2gEdWIAGu01?sign=8zRtuRKBcVL_Vh_gU_R7N5nmtbc=&expire=1567097465)
（这里解析动作文件和控制关节运动包括motion blend都写在这一个脚本里了）