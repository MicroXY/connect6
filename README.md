# connect6
二、	核心算法的设计思路和实现要点
//开局使用TSS检测迫着是否成功
	//是
		//采用TSS迫着
	//否
		//采用alpha-beta剪枝博弈树结合局部静态启发式搜索进行对局面的情况的搜索，最终做出决策
//若上述策略都没有决策结果则采用随机方法决策


六、	参考文献
明确说明所参考的论文、书籍、网络资料、开源代码或其他研究者的程序或成果等详细信息。
例如：
[1]. 六子棋博弈中搜索技术的研究与实现
[2].中国人工智能系列白皮书---机器博弈
[3].六子棋中基于路的双评价参数评估函数的研究与应用_齐祎霏
[4]. 一种新的连珠棋局面表示法及其在六子棋中的应用
[5]. 基于连珠模式的六子棋机器博弈关键技术研究_徐长明
[6]. 计算机博弈问题的复杂性、理论解及相关搜索算法研究
[7]. New Simulation Strategy of MCTS for Connect6
[8]. https://github.com/livingsu/Gobang-ai/blob/master/Sources/widget.cpp
[9]. http://web.cs.ucla.edu/~rosen/161/notes/alphabeta.html
[10]. https://blog.csdn.net/livingsu/article/details/104536005
