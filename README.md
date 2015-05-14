# argcv

[![Join the chat at https://gitter.im/argcv/argcv](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/argcv/argcv?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

## Motivation

该工程是一个轻便的的c++库,使用cmake管理.里面主要存放个人可能会被多次使用到的代码,当然,行文也是以个人的审美趣味来书写.它被期望以submodule的方式出现在某个工程的子目录下,比如[这样](https://github.com/argcv/argcv-seed ).

造轮子并不经常被工程所推荐.但是,一个不造轮子的程序员是违背自己的本性的.在实际产品中,我尽可能按照规则办事,但是这儿,我在寻找我自己的爱情.


## Content

工程绝大多数代码以C++编写,尽可能以头文件方式被引用.代码涉及:

* __系统接口__  根据UNP中介绍的方法,做一些简单的包装,使程序与系统交互,避免重复常用设置.多线程,多进程之间的交互.
* __网络接口__  TCP和UDP的接口,各种有兴趣的RFC的实现.
* __常用第三方代码的包装__ leveldb的接口包装和衍生开发,mongodb的包装.
* __机器学习模型__ 感知机,贝叶斯,kdd,crf,svm,deep learning,各种好玩的模型都会考虑抽象下写进去.

## Progress

正在写....



