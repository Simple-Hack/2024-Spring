## 基于Python的岭回归与LASSO回归模型介绍及实践

 - [ ] *这是一篇学习的总结笔记*
 - [ ] 参考自《从零开始学数据分析与挖掘》 [中]刘顺祥 著 
 - [ ] *完整代码及实践所用数据集等资料放置于：[Github](https://github.com/Yangyi001/Data_analysis/tree/master/%E5%B2%AD%E5%9B%9E%E5%BD%92%E4%B8%8ELASSO%E5%9B%9E%E5%BD%92%E6%A8%A1%E5%9E%8B)*

岭回归与LASSO回归模型是线性回归模型的延申，在多元线性回归模型中我们知道，回归模型的参数估计公式推导的结果是：$\beta=(X'X)^{-1}X'y$，可知，得到$\beta$的前提是矩阵$X'X$可逆，但我们又有一个关于可使用线性回归模型的假设前提：多个自变量之间不存在多重共线性。但是在实际应用中，此种情况不可避免（一个实际应用的例子：家庭收入与支出，由于支出占收入的一部分，描绘成数值可想而知，存在共线性）。  
如果自变量的个数多于样本量（可以认为是样本量不足，通常我们要求样本量远大于自变量个数）或者自变量之间存在多重共线性，此时将无法根据公式计算回归系数的估计值$\beta$。为了解决这类问题，统计学家提出了基于线性回归模型进行扩展的岭回归与LASSO回归模型，下面我们对以下内容进行介绍：

- 岭回归与LASSO回归的系数的求解
- 系数求解的几何意义
- LASSO回归的变量选择
- 岭回归与LASSO回归的预测

### 岭回归模型

前面提到的如果自变量的个数多于样本量或者自变量之间存在多重共线性，此时将无法根据公式计算回归系数的估计值$\beta$。这里我们不妨通过下面两个例子来进一步理解该问题。  

**第一种：自变量个数多于样本量**  
设有矩阵：$X=\begin{bmatrix} 1&2&5 \\ 6&1&3 \end{bmatrix}$  
计算乘积：$X'X=\begin{bmatrix} 1&6 \\ 2&1 \\ 5&3\end{bmatrix} \begin{bmatrix} 1&2&5 \\ 6&1&3\end{bmatrix} = \begin{bmatrix} 37&8&23 \\ 8&5&13 \\ 23&13&34 \end{bmatrix}$  
计算行列式|$X'X$|$=37\times5\times34+8\times13\times23+23\times8\times13$  
$\qquad\qquad\qquad\qquad-37\times13\times13-8\times8\times34-23\times5\times23=0$  
**第二种：当列之间存在多重共线性时**  
设有矩阵：$X=\begin{bmatrix} 1&2&2 \\ 2&5&4 \\ 2&3&4 \end{bmatrix}$  
计算乘积：$X'X=\begin{bmatrix} 1&2&2 \\ 2&5&3 \\ 2&4&4 \end{bmatrix}\begin{bmatrix} 1&2&2 \\ 2&5&4 \\ 2&3&4 \end{bmatrix} = \begin{bmatrix} 9&18&18 \\ 18&38&36 \\ 18&36&36 \end{bmatrix}$  
计算行列式：|$X'X$|$=9\times38\times36 + 18\times36\times18 + 18\times18\times36$  
$\qquad\qquad\qquad\qquad -9\times36\times36 - 18\times18\times36 - 18\times38\times18=0$  
如上例子所示，如果自变量的个数多于样本量或者自变量之间存在多重共线性，最终得到的行列式都等于0（或者近似为0），类似于上述两种情况都会导致线性回归模型的偏回归系数无解或者说求出来的解是无意义的（若计算出来的$X'X$行列式结果接近于0，其逆矩将偏向于无穷大，从而使得回归系数也被放大）。针对该问题的解决办法，岭回归模型可以非常巧妙地解决这个问题，具体做法是在线性回归的目标函数之上添加一个$l2$的正则项，进而使得模型的回归系数有解。  
#### 参数求解
岭回归模型加上$l2$正则项之后，其目标函数可以表示成：$$J(\beta)=\sum(y-X\beta)^2+\lambda||\beta||^2_2=\sum(y-X\beta)^2+\sum\lambda\beta^2$$其中，$\lambda$为非负数，当$\lambda=0$时，该目标函数就退化为线性回归模型的目标函数；当$\lambda$→$+\infty$时，$\sum\lambda\beta^2$也会趋于无穷大，为了使目标函数$J(\beta)$达到最小，只能通过缩减回归系数使$\beta$趋近于0。（式子中的$||\beta||^2_2$表示回归系数$\beta$的平方和）。  
为了求解目标函数$J(\beta)$的最小值，我们的做法同求解线性回归方程的目标函数最小值一样，需要对其求导，并令导函数为0，具体的推导过程如下：  
第一步：展开目标函数平方项$$J(\beta)=(y-X\beta)'(y-X\beta)+\lambda\beta'\beta \\ \qquad \quad = (y'-\beta'X')(y-X\beta)+\lambda\beta'\beta \\ \qquad \qquad \qquad \quad=y'y-y'X\beta-\beta'X'y+\beta'X'X\beta+\lambda\beta'\beta$$第二步：对展开的目标函数求导：$$\frac{\partial J(\beta)}{\partial \beta}=0-X'y-X'y+2X'X\beta+2\lambda\beta \\ =2(X'X+\lambda I)\beta-2X'y$$第三步：令导函数为0，计算回归系数$\beta$$$2(X'X+\lambda I) \beta-2X'y=0  \\ \beta=(X'X+\lambda I)^{-1}X'y$$通过上面的推导，我们最终可以获得回归系数$\beta$的估计值，但估计值中仍含有未知的$\lambda$值，观察原来的目标函数$J(\beta)$，我们知道$\lambda$是$l2$正则项的系数，其作用是用来平衡模型的方差（回归系数的方差）和偏差（真实值与预测值之间的差异）。  

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200420163243936.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NjMwMjQ4Nw==,size_16,color_FFFFFF,t_70#pic_center)如上图，横坐标为模型的复杂度，纵坐标为模型的误差，两条线分别表示方差与偏差随模型复杂度的变化趋势，观察图中我们可知：随着模型复杂度的提升，方差会升高，而偏差会下降（事实上这一点我们可以感性地认识到，若模型复杂度提升，求解的回归系数就越多，则一定程度上方差会增大，而复杂度提升带来的直接效果就是可用于预测的自变量提供的信息增多，所以偏差会减小），再观察**泛化误差**曲线，是先降低后升高的，方差与偏差会对模型的泛化误差产生影响，而我们要求泛化误差的最小值，实际上就是要希望通过平衡方差与偏差来选择一个比较理想的模型。  
对于岭回归模型来说随着$\lambda$的增大，模型的方差会减小（如果$\lambda$增大，矩阵$(X'X+\lambda I)$的行列式随$\lambda$的增加在增加，使得矩阵的逆就会逐渐减小，进而岭回归系数被“压缩”而变小）而偏差会增大（因为此时岭回归系数由$\lambda$贡献的比较多，减小了$X'X$对岭回归系数的贡献）。  
在接着讲解如何求解系数$\lambda$时，我们首先从几何的角度如手讨论一下该系数数求解的意义。  

**泛化误差**
>学习方法的泛化能力（Generalization Error）是由该方法学习到的模型对未知数据的预测能力，是学习方法本质上重要的性质。现实中采用最多的办法是通过测试泛化误差来评价学习方法的泛化能力。泛化误差界刻画了学习算法的经验风险与期望风险之间偏差和收敛速度。
>一个机器学习的泛化误差（Generalization Error），是一个描述学生机器在从样品数据中学习之后，离教师机器之间的差距的函数。
>——百度词条

#### 系数求解的几何意义
根据**凸优化**的知识，我们可以将岭回归模型的目标函数$J(\beta)$最小化问题等价于下面的式子：$$\begin{cases} argmin\begin{Bmatrix} \sum(y-X\beta)^2\end{Bmatrix} \\ 附加约束 \sum \beta^2 \leq t\end{cases}$$ 其中，$t$为一个常数。上式可以理解为：在确保残差平方和最小的情况下，限定所有回归系数的平方和不超过常数$t$。  
为什么要对回归系数的平方和进行约束？其实该做法是限定了回归系数的绝对值不能太大（太大将影响模型的拟合效果），我们从如下例子进行理解：假设我们要预测一个家挺每月的存款，影响存款的因素中包含收入和支出，在前面我们已经讲过，很明显，收入和支出之间存在比较高的线性关系。在利用线性回归进行建模的时候，我们求解出来的回归系数绝对值可能会非常大，最终导致模型的拟合效果不佳（我们可以这么考虑：对于收入和支出，假设该家庭某一个月份由于突发事件，支出较多，但依旧有存款，由于回归系数过大，可能导致预测结果为：存款为负数，但实际是这并不是我们想要的结果，所以就应该对回归系数进行大小的约束）。如果我们给线性回归模型的系数添加平方和的约束，就可以减小甚至避免该类情况的发生。  
为了进一步理解上述式子中的$argmin\begin{Bmatrix} \sum(y-X\beta)^2\end{Bmatrix} $和$\beta^2 \leq t$，我们从几何的角度如手，考虑仅有两个自变量的回归模型的情况，目标函数两个部分在空间中所形成的图形如下：  

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200420163331424.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NjMwMjQ4Nw==,size_16,color_FFFFFF,t_70#pic_center)如图，左图为立体图形，右图为对应的二维投影图。左图中的半椭圆体代表了$\sum^n_{i=1}(y_i-\beta_0-\sum^2_{j=1}x{ij}\beta_j)^2$的部分，它是关于两个系数的二次函数；圆柱体代表了$\beta^2_1+\beta^2_2 \leq t$的部分。对于线性回归模型来讲，图中抛物面的最低点（即图二中标注“最小二乘解”的小红点）代表模型的最小二乘解（误差平方和最小）。而对于岭回归模型来说，此时我们对回归系数进行了约束，当附加了条件$\beta^2_1+\beta^2_2 \leq t$时，抛物面与圆面构成的交点就是岭回归模型的系数解。从图中我们可以看出，岭回归模型的回归系数相比于线性回归模型会偏小，从而达到“压缩”的效果。  
虽然岭回归模型可以求解线性回归中由于$X'X$值为0而不可逆的问题（也可能由于$X'X$的值太小而导致回归系数绝对值过大，模型稳定性差），但是付出的代价是对回归系数进行了“压缩”（此时误差平方和并非最小），从而使得模型更加稳定和可靠。  
由于惩罚项$\sum\lambda\beta^2$是关于回归系数$\beta$的二次函数，所以求目标函数的极小值时，对其偏导总会保留自变量本身。一个更直观的理解是在如上所示的图中，抛物面与圆面的交点很难发生在轴上，即某个变量的回归系数$\beta$很难为0，所以岭回归模型只是对回归系数进行了“压缩”，并不能从真正意义上实现变量的选择。（在线性回归中如果两个自变量之间有高度的线性关系，我们可以考虑将其中一个删除以保证模型的稳定性，而岭回归模型很难做到这一点）。接下来我们通过实际应用来介绍如何确定$\lambda$值  

**凸优化**  
>凸函数最优化，或叫做凸最优化，凸最小化，是数学最优化的一个子领域，研究定义于凸集中的凸函数最小化的问题。凸最佳化在某种意义上说较一般情形的数学最佳化问题要简单，譬如在凸最佳化中局部最佳值必定是全局最佳值。凸函数的凸性使得凸分析中的有力工具在最佳化问题中得以应用，如次导数等。
>凸最佳化应用于很多学科领域，诸如自动控制系统，信号处理，通讯和网络，电子电路设计，数据分析和建模，统计学（最佳化设计），以及金融。在近来运算能力提高和最佳化理论发展的背景下，一般的凸最佳化已经接近简单的线性规划一样直捷易行。许多最佳化问题都可以转化成凸最佳化（凸最小化）问题，例如求凹函数f最大值的问题就等同于求凸函数 -f最小值的问题。
>———维基百科

### 岭回归模型的应用
我们以治疗糖尿病数据集为例，将岭回归模型的理论知识应用到实践中。该数据集包含442条观测值、10个自变量和1个因变量。自变量分别为患者的年龄、性别、体质指数、平均血压及六个血清观测值；因变量为糖尿病指数，其数值越小，说明糖尿病的治疗效果越好。根据相关资料及文献显示，对于胰岛素治疗糖尿病的效果表明，性别和年龄对治疗效果并没有显著的影响，所以在接下来的建模过程中，我们将丢弃这两个变量。  
由前文可知，岭回归模型的系数表达式为$\beta=(X'X+\lambda I)^{-1}X'y$，所以关键点就是找到一个合理的$\lambda$值来平衡模型的方差和偏差，进而得到更加符合实际的岭回归系数（前面我们讲过，该做法会降低预测准确性，但提高模型稳定性）。关于$\lambda$的确定，通常可以使用两种方法，一种是可视化的方法，另一种是交叉验证法。  
#### 可视化方法确定$\lambda$值
由于岭回归模型的系数是关于$\lambda$值的函数，因此可以通过绘制不同的$\lambda$值和对应的回归系数的折线图确定合理的$\lambda$值。一般而言，当回归系数随$\lambda$值得增加而趋近于稳定的点就是所要寻找的$\lambda$值（回归系数趋近于稳定，即方差变小）。  
由于折线图中涉及岭回归模型的系数，因此第一步要根据不同的$\lambda$值计算相应的回归系数。在Python中，可以用sklearn子模块linear_model中的Ridge类实现模型系数的求解，接下来介绍一下该“类”的语法和参数含义。
```
Ridge(alpha=1.0, fit_intercept=True, normalize=False, copy_X=True,
        max_iter=None, tol=0.001, solver='auto', random_state=None)
```
- alpha：用于指定$\lambda$值的参数，默认该参数为1；
- fit_intercept：bool类型参数，是否需要拟合截距项，默认为True；
- normalize：bool类型参数，建模时是否需要对数据做标准化处理，默认为False；
- copy_X：bool类型参数，是否需要复制自变量X的数值，默认为True；
- max_iter：用于指定模型的最大迭代次数；
- tol：用于指定模型收敛的阈值；
- solver：用于指定模型求解最优化问题的算法，默认为'auto'，表示模型根据数据集自动选择算法；
- random_state：用于指定随机数生成器的种子。

通过Ridge‘类’完成岭回归模型求解的参数设置，然后基于fit‘方法’实现模型偏回归系数的求解。下面利用糖尿病数据集绘制不同的$\lambda$值下对应回归系数的折线图，具体代码如下：
```
# 导入第三方模块
import pandas as pd
import numpy as np
from sklearn import model_selection
from sklearn.linear_model import Ridge,RidgeCV
import matplotlib.pyplot as plt

# 读取糖尿病数据集
diabetes = pd.read_excel(r'diabetes.xlsx', sep = '')
```
```
# 查看数据集前五条数据：
In [4]: diabetes.head()
Out[4]:
   AGE  SEX   BMI     BP   S1     S2    S3   S4      S5  S6    Y
0   59    2  32.1  101.0  157   93.2  38.0  4.0  4.8598  87  151
1   48    1  21.6   87.0  183  103.2  70.0  3.0  3.8918  69   75
2   72    2  30.5   93.0  156   93.6  41.0  4.0  4.6728  85  141
3   24    1  25.3   84.0  198  131.4  40.0  5.0  4.8903  89  206
4   50    1  23.0  101.0  192  125.4  52.0  4.0  4.2905  80  135
```
前面我们提到，根据相关资料及文献显示，对于胰岛素治疗糖尿病的效果表明，性别和年龄对治疗效果并没有显著的影响，所以在建模过程中，我们应丢弃这两个变量。同时拆分数据集为训练集和测试集。
```
# 构造自变量（剔除患者性别、年龄和因变量）
predictors = diabetes.columns[2:-1]
# 将数据集拆分为训练集和测试集
X_train, X_test, y_train, y_test = model_selection.train_test_split(
                                    diabetes[predictors], 
                                    diabetes['Y'], 
                                    test_size = 0.2, 
                                    random_state = 1234 )
```
```
# 构造不同的Lambda值
Lambdas = np.logspace(-5, 2, 200)
# 构造空列表，用于存储模型的偏回归系数
ridge_cofficients = []
# 循环迭代不同的Lambda值
for Lambda in Lambdas:
    ridge = Ridge(alpha = Lambda, normalize=True)
    # 系数求解函数
    ridge.fit(X_train, y_train)
    # 权重向量
    ridge_cofficients.append(ridge.coef_)
```
```
# 绘制Lambda与回归系数的关系
# 中文乱码和坐标轴负号的处理
plt.rcParams['font.sans-serif'] = ['Microsoft YaHei']
plt.rcParams['axes.unicode_minus'] = False
# 设置绘图风格
plt.style.use('ggplot')
plt.plot(Lambdas, ridge_cofficients)
# 对x轴作对数变换
plt.xscale('log')
# 设置折线图x轴和y轴标签
plt.xlabel('Lambda')
plt.ylabel('Cofficients')
# 图形显示
plt.show()
```
如上图代码所示，我们先导入模块，随后读取数据集，查看其前五行数据，构造自变量（剔除了患者性别、年龄和因变量），然后将数据集拆分为训练集和测试集；接着构造一个在[-5,2]之间的200个等比数列作为$\lambda$值，对于每一个$\lambda$值求解出自变量的权重向量（即回归系数所构成的向量$\beta$），然后在图中绘制出正则项系数$\lambda$与回归系数\beta$之间的关系。结果如下图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200420163605339.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NjMwMjQ4Nw==,size_16,color_FFFFFF,t_70#pic_center)
如上图所示，展现了不同的$\lambda$值与回归系数之间的折线图，初始迭代的$\lambda$值落在$10^{-5}$至$10^2$之间，然后根据不同的$\lambda$值绘制出有关回归系数的折线图，图中每条直线代表了不同的变量，对于比较突出的喇叭形折线，一般代表该变量存在多重共线性。当$\lambda$值逼近于0时，各变量对应的回归系数应该与线性回归模型的最小二乘解完全一致；随着$\lambda$值的不断增加，各回归系数的取值会迅速缩减为0.最后，按照$\lambda$值得选择标准，我们发现$\lambda$值在0.01附近时，绝大多数变量得回归系数趋于稳定，故可认为$\lambda$值可以选择在0.01附近。
#### 交叉验证法确定$\lambda$值
可视化方法只能大概确定$\lambda$值的范围，为了能够定量地找到最佳的$\lambda$值，需要使用$k$重交叉验证的方法。该方法的具体思想我们借助下图来进行说明：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200420165458726.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NjMwMjQ4Nw==,size_16,color_FFFFFF,t_70#pic_center)
如上图所示，首先将数据集拆分成$k$个样本量大体相当的数据组，并且每个数据组与其他数据组都没有重叠的观测值；然后从$k$组数据中挑选$k-1$组数据作为模型的训练集，剩下的一组数据作为测试集（图中每一行表示数据总集，每一格表示拆分所得的数据组，标蓝色的数据组作为测试集，其他数据组作为训练集），以此类推，将会得到$k$种训练集和测试集。在每一种训练集和测试集下，都会对应一个模型及模型得分（模型得分用于衡量模型的预测效果，通常可以是均方误差）。  
所以，在构造岭回归模型的$k$重交叉验证时，对于每一个给定的$\lambda$值都会得到$k$个模型及对应的模型得分，最终我们可以以平均得分来评估模型的优良。  
实现岭回归模型的$k$重交叉验证，可以使用sklearn子模块linear_model中的RidgeCV类，下面首先介绍有关该“类”的语法及参数含义：
```
RidgeCV(alphas=(0.1, 1.0, 10.0), fit_intercept=True, normalize=False,
        scoring=None, cv=None, gcv_mode=None, store_cv_values=False)
```
- alphas：用于指定多个$\lambda$值的元组或数组对象，默认该参数包含0.1、1.0和10.0三种值；
- fit_intercept：bool类型参数，是否需要拟合截距项，默认为True；
- normalize：bool类型参数，建模时是否需要对数据做标准化处理，默认为False；
- scoring：指定用于评估模型的度量方法；
- cv：指定交叉验证的重数；
- gcv_mode：用于指定执行广义交叉验证的方法，当样本量大于特征数或自变量矩阵$X$为稀疏矩阵时，该参数选用'auto'；当该参数为'svd'时，表示通过矩阵$X$的奇异值分解方法执行广义交叉验证；当该参数为'engin'时，则表示通过矩阵$X'X$的特征根分解方法执行广义交叉验证；
- store_cv_values：bool类型参数，是否在每一个Lambda值下都保存交叉验证得到的评估信息。默认为False，只有党参数cv为None时有效。

为了得到岭回归模型的最佳$\lambda$值，下面使用RidgeCV类对糖尿病数据集做10重交叉验证，具体代码如下：
```
# 岭回归模型的交叉验证
# 设置交叉验证的参数，对于每一个Lambda值，都执行10重交叉验证
ridge_cv = RidgeCV(alphas = Lambdas, normalize=True, scoring='neg_mean_squared_error', cv = 10)
# 模型拟合
ridge_cv.fit(X_train, y_train)
# 返回最佳的lambda值
ridge_best_Lambda = ridge_cv.alpha_
```
查看最佳的$\lambda$值，结果如下
```
In [6]: ridge_best_Lambda
Out[6]: 0.013509935211980266
```
如上输出所示，运用10重交叉验证方法得到的最佳$\lambda$值为0.013509935211980266，与我们用可视化的方法确定的$\lambda$值在0.01附近保持一致。该值的评判标准是（scoring参数）：对于每一个$\lambda$计算平均均方误差（MSE），然后从中挑选出最小的平均均方误差，并将对应的$\lambda$值挑选出来，最为最佳的惩罚项系数$\lambda$的值。

#### 模型的预测

接下来我们运用岭回归模型对测试集进行预测，进而比对预测值与真实值之间的差异，评估模型的拟合能力。根据上面我们所探讨的，通过交叉验证法获得了最佳的$\lambda$值，并根据该值构建岭回归模型，输出模型的偏回归系数，进而可以使用该模型对测试数据集进行预测，具体代码及输出结果如下：
```
In [9]: # 导入第三方包中的函数
   ...: from sklearn.metrics import mean_squared_error
   ...: # 基于最佳的Lambda值建模
   ...: ridge = Ridge(alpha = ridge_best_Lambda, normalize=True)
   ...: ridge.fit(X_train, y_train)
   ...: # 返回岭回归系数
   ...: pd.Series(index = ['Intercept'] + X_train.columns.tolist(),
   							data = [ridge.intercept_] + ridge.coef_.tolist())

Out[9]:
Intercept   -324.753274
BMI            6.210754
BP             0.927874
S1            -0.503593
S2             0.223521
S3             0.045903
S4             4.252818
S5            52.075449
S6             0.383615
dtype: float64
```
如上输出所示，运用最佳的$\lambda$值得到岭回归模型的回归系数，故可以将岭回归模型表示成如下式子：$$Y=-324.753274+6.210754BMI+0.927874BP-0.503593S1+0.223521S2\\+0.045903S3+4.252818S4+52.075449S5+0.383615S6$$上式中对岭回归系数的解释方法与多元线性回归模型一致，以体质指数BMI为例，在其他变量不变的情况下，体质指数每提升一个单位，将促使糖尿病指数Y提升6.210754个单位。  
接下来我们使用该模型对测试集中的数据进行预测，预测代码及输出的结果如下：
```
In [7]: # 预测
   ...: ridge_predict = ridge.predict(X_test)
   ...: # 预测效果验证
   ...: RMSE = np.sqrt(mean_squared_error(y_test,ridge_predict))
   ...: RMSE

Out[7]: 53.1236169466197
```
如上结果所示，通过预测后，使用均方根误差RMSE对模型的预测效果做了定量统计，结果为53.1236169466197。  
有关RMSE的计算公式如下：$$EMSE=\sqrt \frac{\sum^n_{i=1}(y_i-\hat y_i)^2}{n}$$其中，$n$代表观测集中的样本量，$y_i$代表因变量的实际值。$\hat y_i$为因变量的预测值。对于该统计量，值越小，说明模型对数据的拟合效果越好。

### LASSO回归模型

前面对于岭回归模型的介绍中提到，它可以解决线性回归模型中矩阵$X'X$不可逆的问题，解决的办法是添加$l2$惩罚项，最终导致偏回归系数的缩减。但不管怎么缩减，都会始终保留建模时的所有变量（对于岭回归的几何投影来讲，其交点很难在坐标轴上），无法降低模型的复杂度，LASSO回归模型克服了这一缺点。  
与岭回归模型类似，LASSO回归同样属于缩减性估计，而且在回归系数的缩减过程中，可以将一些不重要的回归系数直接缩减为0，从而达到变量选择的效果。之所以LASSO回归可以实现该功能，是因为原本在岭回归模型中的惩罚项由平方变成了绝对值，虽然只是稍作修改，但其结果却大相径庭。  
首先，对比岭回归模型的目标函数，可以将LASSO回归模型的目标函数表示为下方的公式：$$J(\beta)=\sum (y-X\beta)^2 + \lambda ||\beta||_1=\sum(y-X\beta)^2 + \sum\lambda|\beta|$$其中，$\lambda||\beta||_1$为目标函数的惩罚项，并且$\lambda$作为惩罚项系数，与岭回归模型的惩罚项系数一致，需要迭代估计出一个最佳值，$||\beta||_1$为回归系数$\beta$的$l1$正则，表示所有回归系数绝对值的和。  
#### 参数求解  
由于目标函数的惩罚项是关于回归系数$\beta$的绝对值之和，因此惩罚项在零点处是不可导的，那么应用在岭回归上的最小二乘法在此失效，不仅如此，梯度下降法也无法估算出LASSO回归的拟合系数。为了能够得到LASSO的回归系数，可以使用坐标轴下降法。此处仅对坐标轴下降法做介绍，而不给出如何利用该方法求解LASSO的回归系数。  
坐标轴下降法与梯度下降法类似，都属于迭代算法，所不同的是坐标轴下降法是是沿着坐标轴（维度）下降，而梯度下降法是沿着梯度的负方向下降。坐标下降法的数学精髓是：对于$p$维参数的可微凸函数$J(\beta)$而言，如果存在一点$\hat \beta$，使得函数$J(\beta)$在每个坐标轴上均达到最小值，则$\hat{J(\beta)}$就是点$\hat \beta$上的全局最小值。  
*如果读者要详细了解系数的推导过程，请自行查阅网上资料。*

**梯度下降法**
>梯度下降法（英语：Gradient descent）是一个一阶最优化算法，通常也称为最陡下降法，但是不该与近似积分的最陡下降法（英语：Method of steepest descent）混淆。 要使用梯度下降法找到一个函数的局部极小值，必须向函数上当前点对应梯度（或者是近似梯度）的反方向的规定步长距离点进行迭代搜索。如果相反地向梯度正方向迭代进行搜索，则会接近函数的局部极大值点；这个过程则被称为梯度上升法。
>——维基百科

#### 系数求解的几何意义
在LASSO回归模型中，同理，依据凸优化的原理，将LASSO回归模型的目标函数$J(\beta)$的最小化问题等价转换为下面的式子：$$\begin{cases} argmin \begin{Bmatrix} \sum (y-X\beta)^2\end{Bmatrix} \\ 附加约束\sum|\beta|\leq t\end{cases}$$其中，$t$为常数，可以将上面公式理解为：在残差平方和最小的情况下，限定所有回归系数的绝对值之和不超过常数$t$。  
为了更好的理解LASSO回归目标函数中$argmin \begin{Bmatrix} \sum (y-X\beta)^2\end{Bmatrix}$和$\sum|\beta|\leq t$的几何意义，这里仅以两个自变量的回归模型为例，将目标函数中的两个部分在二维坐标图中的投影如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200420170046281.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NjMwMjQ4Nw==,size_16,color_FFFFFF,t_70#pic_center)
如图，椭圆投影代表$\sum^n_{i=1}(y_i-\beta_0-\sum^2_{j=1}x_{ij}\beta_j)^2$部分，矩形代表$|\beta_1|+|\beta_2|\leq t$的部分。  
从图可知，将LASSO的惩罚项映射到二维空间的话，就会形成“角”，一旦“角”与椭圆相交，就会导致$\beta_i$为0（图中所示$\beta_1$为0），进而实现变量的删除，而且相比于岭回归模型的圆面，$l1$正则项的方框顶点更容易与抛物面相交，起到变量筛选的效果。  
综上，LASSO回归模型不仅可以实现变量系数的缩减（如图，抛物面的最小二乘解由红点转移到了相交的黄点，且$\beta_2$系数明显被“压缩”了），而且还可以完成变量的筛选，对于无法影响因变量的自变量，LSAAO回归都将其过滤掉。

### LASSO回归模型的应用
由于LASSO回归模型的目标函数包含惩罚项$\lambda$，因此在计算回归系数之前，仍然需要得到最理想的$\lambda$值。与岭回归模型类似，$\lambda$值的确定可以通过定性的可视化方法和定量的交叉验证法，下面我们就来对以上两种方法做介绍。

#### 可视化方法确定$\lambda$值
可视化方法依旧通过绘制不同的$\lambda$值与回归系数的折线图，然后根据$\lambda$值的选择标准，判断出合理的$\lambda$值。  
折线图中涉及的LASSO回归模型系数的计算，可以使用sklearn子模块linear_model中的Lasso类来求解，有关该“类”的语法和参数含义如下：
```
Lasso(alpha=1.0, fit_intercept=True, normalize=False, precompute=Dalse, copy_X=True,
        max_iter=1000, tol=0.0001, warm_start=False, positive=False, random_state=None,
        selection='cyclic')
```
- alpha：用于指定lambda值的参数，默认该参数为1；
- fit_intercept：bool类型参数，是否需要拟合截距项，默认为True；
- normalize：bool类型参数，建模时是否需要对数据做标准化处理，默认为False；
- precompute：bool类型参数，是否在建模前通过计算Gram矩阵提升运算速度，默认为False；
- copy_X：bool类型参数，是否复制自变量X的数值，默认为True；
- max_iter：用于指定模型的最大迭代次数，默认为1000；
- tol：用于指定模型收敛的阈值，默认为0.0001；
- warm_start：bool类型参数，是否要将前一次的训练结果用作后一次的训练，默认为False；
- positive：bool类型参数，是否将回归系数强制为正数，默认为False
- random_state：用于指定随机数生成的种子；
- selection：指定每次迭代时所选择的回归系数，如果为'random'，表示每次迭代中将随机更新回归系数；如果为'cyclic'，则表示每次迭代时回归系数的更新都基于上一次运算。

为了比较岭回归模型与LASSO回归模型的拟合效果，我们继续使用糖尿病数据集绘制$\lambda$值与回归系数的折线图，代码如下：
```
# 导入第三方模块中的函数
from sklearn.linear_model import Lasso,LassoCV
# 构造空列表，用于存储模型的偏回归系数
lasso_cofficients = []
for Lambda in Lambdas:
    lasso = Lasso(alpha = Lambda, normalize=True, max_iter=10000)
    lasso.fit(X_train, y_train)
    lasso_cofficients.append(lasso.coef_)

# 绘制Lambda与回归系数的关系
plt.plot(Lambdas, lasso_cofficients)
# 对x轴作对数变换
plt.xscale('log')
# 设置折线图x轴和y轴标签
plt.xlabel('Lambda')
plt.ylabel('Cofficients')
# 显示图形
plt.show()
```
生成的图形如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200420170317515.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NjMwMjQ4Nw==,size_16,color_FFFFFF,t_70#pic_center)
如上图所示，初始迭代的$\lambda$值落在$10^{-5}$至$10^2$之间，然后根据不同的$\lambda$值绘制出有关回归系数的折线图，图中每条直线代表了不同的变量。  
与岭回归模型绘制的折线图类似，出现了喇叭形折线，说明该变量存在多重共线性。由图可知，当$\lambda$值落在0.05附近时，绝大多数变量的回归系数趋于稳定，所以我们基本可以锁定$\lambda$值的范围，接下来我们使用定量的交叉验证法获得更为准确的$\lambda$值。

#### 交叉验证法确定$\lambda$值

如果需要实现LASSO回归模型的交叉验证，可以使用Python的sklearn模块提供的现成的接口，只需要调用子模块linear_model中的LassoCV类。有关该“类”的语法和参数说明如下：
```
LassoCV(eps=0.001, n_alphas=100, alphas=None, fit_intercept=True, normalize=False,
        precompute='auto', max_iter=1000, tol=0.001, copy_X=True, cv=None, verbose=False,
        n_jobs=1, positive=False, random_state=None, selection='cyclic')
```
- eps：指定正则化路径长度，默认为0.001，指代Lambda的最小值与最大值之商；
- n_alphas：指定正则项系数Lambda的个数，默认为100个；
- alphas：指定具体的Lambda值列表用于模型的运算；
- fit_intercept：bool类型参数，是否需要拟合截距项，默认为True；
- normalize：bool类型参数，建模时是否需要对数据坐标准化处理，默认为False；
- precompute：bool类型参数，是否在建模前，通过计算Gram矩阵提升运算的速度，默认为False；
- max_iter：指定模型最大迭代次数，默认为1000次；
- tol：用于指定模型收敛的阈值，默认为0.001；
- copy_X：bool类型参数，是否复制自变量X的数值，默认为True；
- cv：指定交叉验证的重数；
- verbose：bool类型参数，是否返回模型运行的详细信息，默认为False；
- n_jobs：指定交叉验证过程中使用的CPU数量，即是否需要并行处理，默认为1表示不并行运行，如果为-1，表示将所有的CPU用于交叉验证的运算；
- positive：bool类型参数，是否将回归系数强制为正数，默认为False；
- random_state：用于指定随机数生成器的种子；
- selection：指定每次迭代时所选择的回归系数，如果为'random'，表示每次迭代重将随机更新回归系数；如果为'cyclic'，则表示每次迭代时回归系数的更新都基于上一次的结果。

接下来我们使用以上类，采用10重交叉验证的方法，得到最佳的$\lambda$值，具体代码如下：
```
In [11]: # LASSO回归模型的交叉验证
    ...: lasso_cv = LassoCV(alphas = Lambdas, normalize=True, cv = 10, max_iter=10000)
    ...: lasso_cv.fit(X_train, y_train)
    ...: # 输出最佳的lambda值
    ...: lasso_best_alpha = lasso_cv.alpha_
    ...: lasso_best_alpha

Out[11]: 0.06294988990221888
```
如上结果所示，通过迭代$10^{-5}$至$10^2$之间的$\lambda$值，并结合10重交叉验证，最终得到合理的$\lambda$值为0.06294988990221888，与可视化方法确定的$\lambda$值范围基本保持一致。接下来，需要基于这个最佳的$\lambda$值重新构建LASSO回归模型，并进行预测。

#### 模型的预测
当确定最佳的$\lambda$值之后，可以借助于Lasso类重新构建LASSO回归模型，具体的构建代码如下：
```
In [12]: # 基于最佳的lambda值建模
    ...: lasso = Lasso(alpha = lasso_best_alpha, normalize=True, max_iter=10000)
    ...: lasso.fit(X_train, y_train)
    ...: # 返回LASSO回归的系数
    ...: pd.Series(index = ['Intercept'] + X_train.columns.tolist(),
    						data = [lasso.intercept_] + lasso.coef_.tolist())

Out[12]:
Intercept   -278.560358
BMI            6.188602
BP             0.860826
S1            -0.127627
S2            -0.000000
S3            -0.488408
S4             0.000000
S5            44.487738
S6             0.324076
dtype: float64
```
如上结果所示，返回的是LASSO回归模型的系数。值得注意的是，系数中含有两个0，分别是S2变量对应的系数和S4变量对应的系数，说明这两个变量对糖尿病指数Y并没有显著意义，故岭回归模型可表示如下：$$Y=-278.560358+6.188602BMI+0.860826BP \\ -0.127627S1-0.488408S3+44.487738S5+0.324076S6$$接下来基于上模型，对测试集中的数据进行预测，同时利用均方根误差RMSE来评估模型的好坏，具体代码如下：
```
In [13]: # 预测
    ...: lasso_predict = lasso.predict(X_test)
    ...: # 预测效果验证
    ...: RMSE = np.sqrt(mean_squared_error(y_test,lasso_predict))
    ...: RMSE

Out[13]: 53.061437258225745
```
如上输出结果所示，LASSO回归模型在测试集上得到的RMSE值为53.061437258225745，相比于岭回归模型的RMSE值，下降了0.062179688393953825。  
在降低模型复杂度的情况下（模型中删除了S2变量和S4变量），进一步提升了模型的拟合效果。所以，在绝大多数情况下，LASSO回归得到的的系数比岭回归模型更加可靠和易于理解。  
接下来我们来对比多元线性回归模型、岭回归模型和LASSO回归模型在糖尿病数据集上的拟合效果，首先需要构建多元线性回归模型，具体代码如下：
```
# 导入第三方模块
from statsmodels import api as sms

# 为自变量X添加常数列1，用于拟合截距项
X_train2 = sms.add_constant(X_train)
X_test2 = sms.add_constant(X_test)

# 构建多元线性回归模型
linear = sms.OLS(y_train, X_train2).fit()
```
查看线性回归模型的系数
```
In [19]: # 返回线性回归模型的系数
    ...: linear.params
Out[19]:
const   -406.699716
BMI        6.217649
BP         0.948245
S1        -1.264772
S2         0.901368
S3         0.962373
S4         6.694215
S5        71.614661
S6         0.376004
dtype: float64
```
如上结果所示，得到了多元线性回归模型的变量系数。此处构建模型使用了OLS类，该类在建模时不拟合截距项，为了得到回归模型的截距项，需要在训练集和测试集的自变量矩阵中添加常数列1.最终，根据上面的输出结果，多元线性回归模型可以表示成：$$Y=-406.699716+6.217649BMI+0.948245BP-1.264772S1+0.901368S2 \\ +0.962373S3+6.694215S4+71.614661S5+0.376004S6$$进一步我们使用如上得到的多元线性回归模型对测试集进行预测，得到糖尿病指数的预测值，然后与测试集中的实际值做对比，计算RMSE值，具体代码如下：
```
# 模型的预测
linear_predict = linear.predict(X_test2)
# 预测效果验证
RMSE = np.sqrt(mean_squared_error(y_test,linear_predict))
```
查看RMSE值
```
In [21]: RMSE
Out[21]: 53.42623939722992
```
如上输出所示，在对模型不做任何假设检验以及拟合诊断的情况下，多元线性回归模型的拟合效果在三个模型中是最差的（根据各自的RMSE值可以看出）。（若对模型做假设检验以及拟合诊断，可能得出不一样的结果）。  

### 小结
我们通过对糖尿病数据集的分析、建模，最终得到了三种可以对糖尿病指数进行预测的模型，分别是多元线性回归模型、岭回归模型以及LASSO回归模型，并对三者进行了比较。

本篇重点介绍了有关线性回归模型的两个扩展模型，分别是岭回归模型与LASSO回归模型，主要讲解了模型的参数求解、目标函数几何意义的理解以及基于糖尿病数据集的实战应用。  
当自变量间存在多重共线性或数据集中自变量的个数多于观测数时，会导致$X'X$矩阵不可逆的问题，进而无法通过最小二乘法得到多元线性回归模型的系数解，而岭回归模型与LASSO回归模型就是为了解决这类问题。  
相比于岭回归模型来说，LASSO回归可以比较方便地实现自变量的筛选，但是付出的代价是增加了模型运算的复杂度。

### 后记
*本篇博文是笔者学习刘顺祥老师所著书籍《从零开始学Python数据分析与挖掘》后整理的部分笔记，文中引用了大量的原书内容，修正了书中部分错误代码，并加入了笔者自己的理解。*    

笔者在整理过程中，对书中作者提及但没有详细解释的概念尽可能地进行了解释，以及加入自己的理解，学习一个模型、明白如何构建模型以及用于预测过程比较简单，但是要理解模型背后的数学意义及原理是十分困难的，笔者尽可能地进行了介绍，但由于笔者才疏学浅，无法完全理解各个参数背后的数学原理及意义，还请有兴趣的读者自行上网查找相关资料。  

 - [ ] 参考自《从零开始学数据分析与挖掘》 [中]刘顺祥 著 
 - [ ] *完整代码及实践所用数据集等资料放置于：[Github](https://github.com/Yangyi001/Data_analysis/tree/master/%E5%B2%AD%E5%9B%9E%E5%BD%92%E4%B8%8ELASSO%E5%9B%9E%E5%BD%92%E6%A8%A1%E5%9E%8B)   