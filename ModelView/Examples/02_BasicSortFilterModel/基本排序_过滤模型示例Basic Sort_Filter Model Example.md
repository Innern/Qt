官方提供的基本排序/过滤模型示例说明了如何使用QSortFilterProxyModel执行基本的排序和过滤。

窗口界面显如下：

![Basic_SortFiltered_Model](https://gitee.com/Innern/typroa-pictures/raw/master/img/Basic_SortFiltered_Model.png)


在继承QWidget类的子类Widget中设计显示。从界面上可以看出总共有两个QGroupBox。上方的QGroupBox中是显示原始模型的树试图，下方的QGroupBox中是显示代理模型的树视图，并且可以交互式地选择使用的过滤模式、语法、哪一列、大小写是否敏感。

代码中，Widget类提供一个公共的函数setSourceModel()，在创建Widget对象后未显示窗口前，调用该函数，为两个树视图设置数据模型。

在main.cpp中向模型中添加数据。

参考官方示例编写的代码参考：[https://github.com/Innern/Qt/tree/master/ModelView/Examples/02_BasicSortFilterModel](https://github.com/Innern/Qt/tree/master/ModelView/Examples/02_BasicSortFilterModel)。

官方示例中Sorted/filtered Model组合框中使用的是网格布局，参考代码中使用垂直布局。

