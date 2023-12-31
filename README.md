# Huffman decoder

哈夫曼译码器 (Huffman decoder) 是一种使用哈夫曼树来压缩文件的工具

支持压缩，解压，打印哈夫曼树，打印字符映射关系

用户可以将打印数据自定义输入到文件中，或者终端上

# Environment

操作系统： Arch Linux x86_64

开发语言： C++(C11标准)

开发平台： Visual Studio Code

# Usage

## 编译运行
```sh
g++ main.cc -o huf
```

## 文档说明

```
用法：./huf [选项]... [文件]... [输出格式]...
例如：./huf en/de yourfile.txt --tree 
将指定的文件按照输出格式进行压缩与解压。

[选项]
 en              接收类型参考 [文件] 选项
                 将文件进行压缩操作，输出压缩率，生成(.code)文件

 de               接收(.code)文件
                  将文件进行解压操作，生成(.decode)文件

```

详细说明请参考 [文档](./include/manual.txt)

或者在终端运行
```sh
./buf --help
```

## Contributing

欢迎请求请求。 对于重大更改，请先打开 issue
讨论您想要改变的内容。

请确保适当更新测试。

## License

[MIT](https://choosealicense.com/licenses/mit/)

# Authors and acknowledgment

这是我数据结构的课设作业，一个方面是为了熟悉使用C++的语法，由于时间原因，并没有结合太多C++特色，之前可以用智能指针重写一下，其次更多的是为了掌握各种数据结构的使用，比如本项目中使用了C++的 `priority_queue`,`map`,以及模拟栈，非递归打印树的结构，个人感觉非常巧妙。

非递归打印树参考 [二叉树的树型结构打印](https://www.cnblogs.com/evenleee/p/11901818.html)

