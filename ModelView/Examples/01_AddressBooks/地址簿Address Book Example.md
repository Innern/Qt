## 简述

Address Book Example 展示了怎样使用代理模型在单个模型的数据上显示不同的视图。

应用程序窗口界面如下：

![应用程序主窗口界面](https://gitee.com/Innern/typroa-pictures/raw/master/img/应用程序主窗口界面.png)


该示例提供一个通讯薄，允许联系人按字母顺序分为9组：ABC、DEF、GHI、...、VW、...、XYZ。

通过在同一模型上使用多个视图来实现，每个视图使用QSortFilterProxyModel类的实例进行过滤。

应用程序框架如下，以QMainWindow为基础框架，AddressWidget作为应用程序的中心部件，使用QTableView显示TableModel模型的内容，并且使用了QSortFilterProxyModel代理模型过滤源模型的数据。

![应用程序结构](https://gitee.com/Innern/typroa-pictures/raw/master/img/应用程序结构.png)


* TableModel是QAbstractTableModel的一个子类，它提供了访问数据的标准模型/试图API，包含已经添加联系人的列表。
* AddressWidget是QTabWidget子类，用于操作10个选项卡：9个字母组选项卡和NewAddressTab的一个实例。NewAddressTab类是QWidget的一个子类，仅在通讯录为空时使用，提供用户添加一些联系人。AddressWidget与TableModel的实例交互，以添加、编辑和删除通讯簿中的条目。
* QSortFilterProxyModel为每组选项卡筛选联系人。每个代理模型都是用QRegExp来过滤不属于相应字母组的联系人。
* AddDialog类用于从用户处获取通讯簿的信息。由NewAddressTab实例添加联系人，由AddressWidget的实例以添加和编辑联系人。
## 数据模型TableModel实现

TableModel类对QAbstractTableModel类子类化，为访问联系人列表中的数据提供标准的API。

TableModel中必须实现的基本函数有：rowCount()模型行数量、columnCount()列数量、data()获取模型数据、headerData()标头。要使模型可以编辑，还需要实现：flags()返回项标志、setData()设置数据、insertRows()插入行、removeRows()删除行。

## 选项卡部件AddressWidget类实现

AddressWidget类提供了添加、编辑和删除联系人、将联系人保存到文件以及从文件加载联系人的功能。

AddressWidget扩展了QTabWidget来容纳10个选项卡：NewAddressTab和9个字母组选项卡

，另外还操作了QTableView、TableModel对象、QSortFilterProxyModel对象用于过滤条目。

### NewAddressTab类实现

NewAddressTab类提供了一个信息性的选项卡，提示地址簿是空的，根据地址簿的内容出现和消失。

## AddDialog类实现

AddDialog类扩展了QDialog，为用户提供QLineEdit和QTextEdit，以便用户将数据输入到通讯簿。

