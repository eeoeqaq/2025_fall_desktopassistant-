# 12/7
在vscode上的多文件程序运行方面发现问题。搜索得一篇博客园上的博客:https://www.cnblogs.com/xiaml/articles/17425759.html 。研究后学会修改task.json和用内部命令行直接编译两种办法。

# 12/8
用自定义的类作map的key报错，map用rb tree实现需要key有’<‘运算符重载

在diaryfile部分因为cin不能保留空格，同时对file内数据结构混乱的隐忧，更改了load和save的逻辑部分。现在改用行首的标志词区分diary和content

bf_ss.str(bf_str);
bf_ss.clear();  // 重置流状态标志

# 12/9
用ai配置了cmake。遭遇的问题主要是文件路径有中文和空格、默认使用nmake而非mingw等问题。`这个问题仍未解决（12/22）`

# 12/20
重置了文件系统

# 12/22
继续完善文件系统，继续修bug