# 2021华为软挑

## 思路参考
* https://zhuanlan.zhihu.com/p/356155386
* https://zhuanlan.zhihu.com/p/356518467
* 判题器 https://github.com/B1ACK917/2021HWAutoGrader

## 题目分析
* 离线题目，可以一次性读入和输出
* 用尽可能少的成本，满足用户的虚拟机需求
  * 成本取决于：
    * 硬件成本
      * 取决于虚拟机需求最大的一天，怎样最佳分配服务器
    * 能耗成本
      * 服务器一定，怎么分配服务器使能耗成本最低
* 单双节点部署如何选择？

### 约束条件
* 容量约束：服务器可以用来容纳用户的虚拟机，但是服务器上的任意一个节点(A和B)上的资源负载(CPU和内存)均不能超过其容量上限。
* 请求类型：用户的请求共分为两种类型：创建请求和删除请求。创建请求表示用户向公有云平台提交的一个创建虚拟机的请求；删除请求表示用户提交的删除一台之前创建的虚拟机的请求。
* 请求序列：由一系列请求构成的序列。题目会给出接下来若干天中每一天用户的请求序列，根据每天的请求序列，你需要进行相应的资源规划和调度。
* 数据中心扩容：在得知了一天的请求序列后，你可以在实际进行调度前进行一次数据中心扩容。即购买一些新的服务器来容纳后续用户请求的虚拟机，同时你需要付出所购买服务器相应的硬件成本。你需要指定购买哪些类型的服务器以及购买的数量。初始时你没有任何服务器。
* 虚拟机迁移：在完成扩容后，在处理每一天的新请求之前，你还可以对当前存量虚拟机进行一次迁移，即把虚拟机从一台服务器迁移至另一台服务器。对于单节点部署的虚拟机，将其从一台服务器的A节点迁移至B节点(或反之)也是允许的。迁移的目的服务器和节点必须有足够的资源容纳所迁移的虚拟机。迁移的虚拟机总量不超过当前存量虚拟机数量的千分之五。即假设当前有n台存量虚拟机，每天你可以迁移的虚拟机总量不得超过5n/1000向下取整。
  * 迁移是否有用？
* 部署虚拟机：在完成扩容和迁移之后，你需要按顺序处理当天所有的新请求。对于每一个创建虚拟机的新请求，你要为虚拟机指定一台服务器进行部署。若虚拟机是单节点部署的，你还需要指明部署在服务器的A节点还是B节点。处理请求的过程中，任意一台服务器上每个节点容纳的虚拟机资源总和都不能超出节点本身的资源容量（指CPU和内存两个维度）。

### 评分规则
1. __总成本低__
   1. 总成本 = 硬件成本+ 能耗成本
   2. 硬件成本：一次性成本，将方案中所有购买的服务器的硬件成本相加
   3. 能耗成本：有负载(至少部署了一台虚拟机)的服务器视为开机状态，没有任何负载的服务器视为关机状态，以此计算当天的能耗成本。整体能耗成本即每一天的能耗成本的总和。
2. 迁移数少
3. 程序用时
4. 对于每组测试数据，选手的程序必须在规定时间90秒内输出合法的解，若程序运行超时，运行出错或者输出了不合法的解，则记无成绩。对于多组测试数据，选手的程序在任意一组数据上无成绩则记整体无成绩。

## 注意事项
- 做好版本管理
  - branch
    - main 开发版本
    - release 稳定版本
  - tag
    - AC版本都可以加上tag
- 代码可拓展性
  - 只有正式比赛的测试集才算成绩，不仅测试集会变，甚至题目也会变，所以对代码的可拓展性和维护性有要求